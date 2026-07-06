#pragma once

#define NANOBMC_NAME "NanoBMC"
#define NANOBMC_VERSION "0.1.0"
#define NANOBMC_CPU_FREQ_MHZ 160

#define WIFI_AP_SSID "NanoBMC"
#define WIFI_AP_PASSWORD "changeme-nanobmc"
#define WIFI_AP_CHANNEL 6

// WiFi AP TX power presets to try for short-range servicing:
//
// | Preset | When to try |
// | --- | --- |
// | WIFI_POWER_8_5dBm | Current default; start here for reduced heat versus maximum power. |
// | WIFI_POWER_7dBm | Try next when servicing from the same rack or nearby bench. |
// | WIFI_POWER_5dBm | Try for very short-range access when the client stays close to NanoBMC. |
// | WIFI_POWER_2dBm | Lowest short-range candidate; use only after reliability testing. |
//
// Before enclosing the board, test web and serial bridge connection reliability at the
// actual expected service distance, with the intended client device and enclosure position.
#define WIFI_AP_TX_POWER WIFI_POWER_8_5dBm
#define WIFI_AP_IDLE_TIMEOUT_SECONDS 300

#define SERIAL_BAUD 115200
#define SERIAL_RX_GPIO 0
#define SERIAL_TX_GPIO 1
#define SERIAL_TCP_PORT 23

#define PI_RESET_GPIO 4
#define PI_RESET_ACTIVE_LOW false
#define PI_RESET_PULSE_MS 500

#define NANOBMC_IDLE_POWER_SAVE_ENABLED true
#define NANOBMC_IDLE_DELAY_MS 250
#define NANOBMC_ACTIVE_DELAY_MS 10
#define NANOBMC_IDLE_LIGHT_SLEEP_ENABLED false
#define NANOBMC_IDLE_LIGHT_SLEEP_MS 250
#define NANOBMC_WEB_REQUEST_IDLE_GRACE_MS 100
