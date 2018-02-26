#include "ntpUtils.h"
#include "authUtils.h"

#include <FirebaseArduino.h>
#include <DHT.h>

#define DHTPIN D2
#define FANPIN D6
#define LIGHTPIN D5
#define DHTTYPE DHT11
#define DELAY_TIME_TO_SEND  5000 // 15 min
#define TABLE_NAME "leitura"
#define CLIENT_EMAIL "well.oliveira.snt@gmail.com"
#define DEVICE_PATH "/devices/mkW3jbNvyKMzC8FLODaQd7GAum02/"

DHT dht(DHTPIN, DHTTYPE);

StaticJsonBuffer<1000> jsonBuffer;
JsonObject &root = jsonBuffer.createObject();

void inline readDataToSend();
void inline setLeitura(String temperatura, String umidade, String cliente, String data);

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
    setSyncProvider(getNtpTime);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(FANPIN, OUTPUT);
    pinMode(LIGHTPIN, OUTPUT);
    digitalWrite(LED_BUILTIN, !Firebase.getBool(String(DEVICE_PATH) + "light"));
}

String errorToPrint = String(TABLE_NAME) + " failed to send:";

int count = 15000;

void loop() {
    readDataToSend();

    count++;
    if(count > 150) {
        delay(100);
        count = 0;
        Firebase.push(TABLE_NAME, root);
    }

    if (Firebase.failed()) {
        Serial.print(errorToPrint);
        Serial.println(Firebase.error());
        return;
    }

    Serial.println(Firebase.getBool(String(DEVICE_PATH) + "light"));

    digitalWrite(FANPIN, !Firebase.getBool(String(DEVICE_PATH) + "fan"));
    digitalWrite(LIGHTPIN, !Firebase.getBool(String(DEVICE_PATH) + "light"));
    digitalWrite(LED_BUILTIN, !Firebase.getBool(String(DEVICE_PATH) + "light"));

}

void inline setLeitura(String temperatura, String umidade, String cliente, String data) {
    root["temperatura"] = temperatura;
    root["umidade"] = umidade;
    root["cliente"] = cliente;
    root["data"] = data;
}

void inline readDataToSend() {
    String temperatura = String(dht.readTemperature());
    String umid = String(dht.readHumidity());
    String cliente = CLIENT_EMAIL;
    String data = "";//String(getDateTime());
    delay(1000);

    setLeitura(temperatura, umid, cliente, data);
    Serial.println("temp=" + temperatura + " umid=" + umid + " clnt=" + cliente + " dateTime=" + data);
}
