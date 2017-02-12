#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <DHT.h>
#include "authUtils.h"
#include "ntpUtils.h"

#define DHTPIN D2
#define DHTTYPE DHT11
#define DELAY_TIME_TO_SEND 10000
#define TABLE_NAME "leitura"
#define CLIENT_EMAIL "user@mail.com"

DHT dht(DHTPIN, DHTTYPE);

StaticJsonBuffer<200> jsonBuffer;
JsonObject &root = jsonBuffer.createObject();

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
}

void loop() {
    delay(DELAY_TIME_TO_SEND);
    root["temperatura"] = dht.readTemperature();
    root["umidade"] = dht.readHumidity();
    root["cliente"] = CLIENT_EMAIL;
    root["data"] = getDateTime();
    delay(1000);

    Firebase.push(TABLE_NAME, root);

    if (Firebase.failed()) {
        Serial.print( String(TABLE_NAME) +" failed to send:");
        Serial.println(Firebase.error());
        return;
    }
}