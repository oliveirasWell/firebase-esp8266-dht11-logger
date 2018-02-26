#ifndef NPT_UTILS
#define NPT_UTILS

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define UDP_NTS_PORT 8888
#define NTP_PACKET_SIZE 48

WiFiUDP udp;
const char* ntpServerName = "time.nist.gov";
IPAddress timeServer;

const int timeZone = -4;

/*-------- NTP code ----------*/
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address) {
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    udp.beginPacket(address, 123); //NTP requests are to port 123
    udp.write(packetBuffer, NTP_PACKET_SIZE);
    udp.endPacket();
}

time_t getNtpTime() {
    while (udp.parsePacket() > 0); // discard any previously received packets
    Serial.println("Transmit NTP Request");
    sendNTPpacket(timeServer);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500) {
        int size = udp.parsePacket();
        if (size >= NTP_PACKET_SIZE) {
            Serial.println("Receive NTP Response");
            udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900 = (unsigned long) packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long) packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long) packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long) packetBuffer[43];
            return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
    }
    Serial.println("No NTP Response :-(");
    return 0; // return 0 if unable to get the time
}

String getFormatedTimeOf(String time) {
    return (time.toInt() < 10 ? String("0") : "") + time;
}

String getDateTime() {
    if (timeStatus() != timeNotSet) {
        String hora = getFormatedTimeOf(String(hour()));
        String minuto = getFormatedTimeOf(String(minute()));
        String segundo = getFormatedTimeOf(String(second()));
        String dia = getFormatedTimeOf(String(day()));
        String mes = getFormatedTimeOf(String(month()));
        String ano = getFormatedTimeOf(String(year()));
        String returnString = hora + ":" + minuto + ":" + segundo + " " + dia + "/" + mes + "/" + ano;
        return returnString;
    }
}

String getEpoch() {
    time_t t = now();
    return String(t);
}

#endif /* NPT_UTILS */
