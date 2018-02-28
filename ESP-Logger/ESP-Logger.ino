#include "ntpUtils.h"
#include "authUtils.h"

#include <FirebaseArduino.h>
#include <DHT.h>

#define DHTPIN D2
#define FANPIN D6
#define LIGHTPIN D5
#define SENSORPIN D8
#define DHTTYPE DHT11
#define DELAY_TIME_TO_SEND  5000 // 15 min
#define TABLE_LEITURA "leitura"
#define TABLE_MOTION "motion"
#define CLIENT_EMAIL "well.oliveira.snt@gmail.com"
#define DEVICE_PATH "/devices/mkW3jbNvyKMzC8FLODaQd7GAum02/"

DHT dht(DHTPIN, DHTTYPE);

StaticJsonBuffer<2000> jsonBuffer;
JsonObject &leitura = jsonBuffer.createObject();
JsonObject &motion = jsonBuffer.createObject();

int count = 15000;
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
    motionSensorRead = !digitalRead(SENSORPIN);
    digitalWrite(LED_BUILTIN, !Firebase.getBool(String(DEVICE_PATH) + "light"));

}

inline String getErrorString(String table) {
    return String(TABLE_LEITURA) + " failed to send:";
}

void inline updateMontionSensor() {
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

void inline setLeitura(String temperatura, String umidade, String cliente, String data) {
    leitura["temperatura"] = temperatura;
    leitura["umidade"] = umidade;
    leitura["cliente"] = cliente;
    leitura["data"] = data;
}

void inline readDataToSend() {
    String temperatura = String(dht.readTemperature());
    String umid = String(dht.readHumidity());
    String cliente = CLIENT_EMAIL;
    String data = getEpoch();
    delay(1000);

    setLeitura(temperatura, umid, cliente, data);
    Serial.println("temp=" + temperatura + " umid=" + umid + " clnt=" + cliente + " dateTime=" + data);
}

void inline updateLeitura() {
    if (count > 1500) {
        readDataToSend();
        count = 0;
        Firebase.push(TABLE_LEITURA, leitura);
    }

    if (Firebase.failed()) {
        Serial.print(getErrorString(TABLE_LEITURA));
        Serial.println(Firebase.error());
        return;
    }
}

void inline updateLighAndFan() {
    digitalWrite(FANPIN, !Firebase.getBool(String(DEVICE_PATH) + "fan"));
    digitalWrite(LIGHTPIN, !Firebase.getBool(String(DEVICE_PATH) + "light"));
    digitalWrite(LED_BUILTIN, !Firebase.getBool(String(DEVICE_PATH) + "light"));
}


void loop() {

    WiFi.hostByName(ntpServerName, timeServer);
    setSyncProvider(getNtpTime);
    setSyncInterval(300);

    updateMontionSensor();

    updateLeitura();

    updateLighAndFan();

    count++;
    delay(100);
    Serial.println(count);
}
