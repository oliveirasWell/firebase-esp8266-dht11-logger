#include "ntpUtils.h"
#include "authUtils.h"

#include <FirebaseArduino.h>
#include <DHT.h>

#define DHTPIN D2
#define FANPIN D6
#define LIGHTPIN D5
#define SENSORPIN D8

#define REDPIN D7
#define GREENPIN D0
#define BLUEPIN D3

#define DHTTYPE DHT11
#define DELAY_TIME_TO_SEND  360
#define TABLE_LEITURA "leitura"
#define TABLE_MOTION "motion"
#define CLIENT_EMAIL "well.oliveira.snt@gmail.com"  
#define DEVICE_PATH "/devices/mkW3jbNvyKMzC8FLODaQd7GAum023/"

DHT dht(DHTPIN, DHTTYPE);

StaticJsonBuffer<2000> jsonBuffer;
JsonObject &leitura = jsonBuffer.createObject();
JsonObject &motion = jsonBuffer.createObject();

int r = 0;
int g = 0;
int b = 0;

int errorCount = 0;

int count = DELAY_TIME_TO_SEND;
bool motionSensorRead = false;

void setup() {
    Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    udp.begin(UDP_NTS_PORT);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(FANPIN, OUTPUT);
    pinMode(LIGHTPIN, OUTPUT);
    pinMode(SENSORPIN, INPUT);
    pinMode(REDPIN, OUTPUT);
    pinMode(GREENPIN, OUTPUT);
    pinMode(BLUEPIN, OUTPUT);
    
    motionSensorRead = !digitalRead(SENSORPIN);
    digitalWrite(LED_BUILTIN, !Firebase.getBool(String(DEVICE_PATH) + "light"));

}

void(* resetFunc) (void) = 0;

inline String getErrorString(String table) {
    return String(TABLE_LEITURA) + " failed to send:";
}

void inline readMotionSensorAndSendToFirebase() {
    if (digitalRead(SENSORPIN) != motionSensorRead) {
        motionSensorRead = digitalRead(SENSORPIN);
        motion["state"] = String(motionSensorRead);
        motion["date"] = getEpoch();
        Firebase.push(TABLE_MOTION, motion);
        Serial.println("sensor=" + String(motionSensorRead));
        delay(1000);
    }

    if (Firebase.failed()) {
        Serial.print(getErrorString(TABLE_MOTION));
        Serial.println(Firebase.error());
        return;
    }
}

void inline setLeitura(String temperatura, String umidade, String cliente, String data, String motion) {
    leitura["temperatura"] = temperatura;
    leitura["umidade"] = umidade;
    leitura["cliente"] = cliente;
    leitura["data"] = data;
    leitura["motion"] = motion;
}

void inline readDataToSend() {
    String temperatura = String(dht.readTemperature());
    String umid = String(dht.readHumidity());
    String cliente = CLIENT_EMAIL;
    String data = getEpoch();
    String motion = String(digitalRead(SENSORPIN));
    delay(1000);
    setLeitura(temperatura, umid, cliente, data, motion);
    Serial.println(
            "temp=" + temperatura + " umid=" + umid + " clnt=" + cliente + " dateTime=" + data + " motion=" + motion);
}

void inline readLeituraAndSendToFirebase() {
    readDataToSend();
    if (count > DELAY_TIME_TO_SEND) {
        if (leitura["temperatura"] == "nan" || leitura["umidade"] == "nan" || getEpoch().toInt() <= 1000) {
            Serial.print("Leitura falha");
            return;
        }

        count = 0;
        Firebase.push(TABLE_LEITURA, leitura);
        Serial.print("Enviou leitura");
        return;
    }
    if (Firebase.failed()) {
        Serial.print(getErrorString(TABLE_LEITURA));
        Serial.println(Firebase.error());
        errorCount++;
        return;
    }
}

void inline updateLighAndFan() {
    digitalWrite(FANPIN, !Firebase.getBool(String(DEVICE_PATH) + "fan"));
    digitalWrite(LIGHTPIN, !Firebase.getBool(String(DEVICE_PATH) + "light"));
    digitalWrite(LED_BUILTIN, !Firebase.getBool(String(DEVICE_PATH) + "light"));
    
    int rAux = Firebase.getInt(String(DEVICE_PATH) + "r");
    int bAux = Firebase.getInt(String(DEVICE_PATH) + "b");
    int gAux = Firebase.getInt(String(DEVICE_PATH) + "g");
    
    r = rAux != 0 ? rAux : r ;
    b = bAux != 0 ? bAux : b ;
    g = gAux != 0 ? gAux : g ;
    
    Serial.println( "r: " + String(r) + ", " +
                    "b: " + String(b) + ", " +
                    "g: " + String(g) ); 
    analogWrite(REDPIN, r);
    analogWrite(GREENPIN, g);
    analogWrite(BLUEPIN, b);
}

//TODO verify wheter is this okay or not
void inline updateNtp() {
    WiFi.hostByName(ntpServerName, timeServer);
    setSyncProvider(getNtpTime);
    setSyncInterval(300);
}

void inline updateCountAndPrint() {
    delay(100);
    count++;
    Serial.println("/-----Count-------");
    Serial.println(count);
    Serial.println("------Count------/");
}

void inline updateCountErrosAndPrint() {
    if(errorCount > 1000) {
        Serial.println("/-----errorCount-------");
        Serial.println(errorCount);
        Serial.println("------errorCount------/");
        errorCount = 0;
        resetFunc();
    }
}

void loop() {
    updateNtp();
    readMotionSensorAndSendToFirebase();
    readLeituraAndSendToFirebase();
    updateLighAndFan();
    updateCountAndPrint();
}
