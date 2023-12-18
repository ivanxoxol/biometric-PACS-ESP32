#include <Arduino.h>
#include "libWiFi\libWiFi.h"

void setup() {
  Serial.begin(115200);
  scanNetworks();
  initWiFi();
}

void loop() {
  reconnectWiFi();
}
