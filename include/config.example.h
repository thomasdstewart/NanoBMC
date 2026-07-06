#pragma once

#define NANOBMC_NAME "NanoBMC"
#define NANOBMC_VERSION "0.1.0"

#define WIFI_AP_SSID "NanoBMC"
#define WIFI_AP_PASSWORD "changeme-nanobmc"
#define WIFI_AP_CHANNEL 6
#define WIFI_AP_TX_POWER WIFI_POWER_8_5dBm
#define WIFI_AP_IDLE_TIMEOUT_SECONDS 300

#define SERIAL_BAUD 115200
#define SERIAL_RX_GPIO 0
#define SERIAL_TX_GPIO 1
#define SERIAL_TCP_PORT 23

#define PI_RESET_GPIO 4
#define PI_RESET_ACTIVE_LOW false
#define PI_RESET_PULSE_MS 500
