#include "wifi_ap.h"
#include "config_loader.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <WiFi.h>

void startWifiAp() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);
}

const char *wifiApIpAddress() {
  static String ip;
  ip = WiFi.softAPIP().toString();
  return ip.c_str();
}
#endif
