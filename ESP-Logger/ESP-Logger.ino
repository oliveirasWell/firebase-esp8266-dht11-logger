#include "ntpUtils.h"
#include "authUtils.h"

#include <FirebaseArduino.h>
#include <DHT.h>

#define DHTPIN D2
#define DHTTYPE DHT11
#define DELAY_TIME_TO_SEND  900000 // 15 min
#define TABLE_NAME "leitura"
#define CLIENT_EMAIL "user@mail.com"

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
    digitalWrite(LED_BUILTIN, HIGH);
}

String errorToPrint = String(TABLE_NAME) + " failed to send:";

void loop() {
    readDataToSend();
    Firebase.push(TABLE_NAME, root);

    if (Firebase.failed()) {
        Serial.print(errorToPrint);
        Serial.println(Firebase.error());
        return;
    }

    delay(DELAY_TIME_TO_SEND);
}

void inline setLeitura(String temperatura, String umidade, String cliente, String data) {
    root["temperatura"] = temperatura;
    root["umidade"] = umidade;
    root["cliente"] = cliente;
    root["data"] = data;
}

void inline readDataToSend() {
    digitalWrite(LED_BUILTIN, LOW);
    String temperatura = String(dht.readTemperature());
    String umid = String(dht.readHumidity());
    String cliente = CLIENT_EMAIL;
    String data = String(getDateTime());
    delay(5000);

    setLeitura(temperatura, umid, cliente, data);
    Serial.println("temp=" + temperatura + " umid=" + umid + " clnt=" + cliente + " dateTime=" + data);
    digitalWrite(LED_BUILTIN, HIGH);
}
