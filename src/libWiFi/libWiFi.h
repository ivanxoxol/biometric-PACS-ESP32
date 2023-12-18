#ifndef LIBWIFI_H
#define LIBWIFI_H

#include <WiFi.h>

static const char* my_ssid = "ZyXEL_KEENETIC_MF";
static const char* my_password = "zyxel-592-yqz";

void initWiFi();
void scanNetworks();
void reconnectWiFi();

#endif
