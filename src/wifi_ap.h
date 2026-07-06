#pragma once

#ifdef ARDUINO
// Starts the configured ESP32 soft AP and applies low-power AP settings.
void startWifiAp();
const char *wifiApIpAddress();
#endif
