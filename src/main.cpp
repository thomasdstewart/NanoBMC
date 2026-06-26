#include "config_loader.h"
#include "reset_control.h"
#include "serial_bridge.h"
#include "web.h"
#include "wifi_ap.h"

#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(200);

  Serial.println();
  Serial.print(NANOBMC_NAME);
  Serial.print(" ");
  Serial.println(NANOBMC_VERSION);

  setupResetControl();
  beginSerialBridge();

  Serial.print("Starting WiFi AP: ");
  Serial.println(WIFI_AP_SSID);
  startWifiAp();

  beginWebServer();

  Serial.print("AP IP: ");
  Serial.println(wifiApIpAddress());
  Serial.print("Serial bridge listening on port ");
  Serial.println(SERIAL_TCP_PORT);
  Serial.println("Web UI listening on port 80");
  Serial.println("Ready");
}

void loop() {
  handleSerialBridge();
  handleWebServer();
}
