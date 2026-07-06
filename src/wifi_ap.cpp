#include "wifi_ap.h"
#include "config_loader.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>

void startWifiAp() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD, WIFI_AP_CHANNEL);
  WiFi.setTxPower(WIFI_AP_TX_POWER);

#if WIFI_AP_IDLE_TIMEOUT_SECONDS > 0
  esp_wifi_set_inactive_time(WIFI_IF_AP, WIFI_AP_IDLE_TIMEOUT_SECONDS);
#endif
}

const char *wifiApIpAddress() {
  static String ip;
  ip = WiFi.softAPIP().toString();
  return ip.c_str();
}
#endif
