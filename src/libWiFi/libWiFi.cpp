#include "libWiFi.h"

void initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(my_ssid, my_password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }
    Serial.println(WiFi.localIP());
}

void scanNetworks() {
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++) {
        Serial.print(WiFi.SSID(i));
        Serial.print(WiFi.RSSI(i));
        Serial.println(WiFi.encryptionType(i));
        delay(10);
    }
}

void reconnectWiFi() {
    static const unsigned long interval = 1000;
    static unsigned long previous = 0;
    unsigned long current = millis();
    if ((WiFi.status() != WL_CONNECTED) && 
        (current - previous >= interval)) {
        WiFi.disconnect();
        WiFi.reconnect();
        previous = current;
    }
}
