#include "config_loader.h"
#include "reset_control.h"
#include "serial_bridge.h"
#include "web.h"
#include "wifi_ap.h"

#include <Arduino.h>

#if defined(ARDUINO_ARCH_ESP32)
#include <esp_sleep.h>
#endif

namespace {
void idleDelay() {
#if NANOBMC_IDLE_POWER_SAVE_ENABLED
  const bool idle = serialBridgeIdle() && !webRequestActive();
  if (idle) {
#if NANOBMC_IDLE_LIGHT_SLEEP_ENABLED && defined(ARDUINO_ARCH_ESP32)
    esp_sleep_enable_timer_wakeup(
        static_cast<uint64_t>(NANOBMC_IDLE_LIGHT_SLEEP_MS) * 1000ULL);
    esp_light_sleep_start();
#else
    delay(NANOBMC_IDLE_DELAY_MS);
#endif
    return;
  }
#endif

  delay(NANOBMC_ACTIVE_DELAY_MS);
}
} // namespace

void setup() {
#if defined(ARDUINO_ARCH_ESP32)
  setCpuFrequencyMhz(NANOBMC_CPU_FREQ_MHZ);
#endif

  Serial.begin(115200);
  delay(200);

  Serial.println();
  Serial.print(NANOBMC_NAME);
  Serial.print(" ");
  Serial.println(NANOBMC_VERSION);
#if defined(ARDUINO_ARCH_ESP32)
  Serial.print("CPU frequency: ");
  Serial.print(getCpuFrequencyMhz());
  Serial.println(" MHz");
#endif

  setupResetControl();

  Serial.print("Starting WiFi AP: ");
  Serial.println(WIFI_AP_SSID);
  startWifiAp();

  beginSerialBridge();
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
  idleDelay();
}
