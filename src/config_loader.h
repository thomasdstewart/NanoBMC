#pragma once

#if __has_include("config.h")
#include "config.h"
#else
#include "config.example.h"
#endif

#ifndef WIFI_AP_CHANNEL
#define WIFI_AP_CHANNEL 6
#endif

#ifndef WIFI_AP_TX_POWER
#define WIFI_AP_TX_POWER WIFI_POWER_8_5dBm
#endif

#ifndef WIFI_AP_IDLE_TIMEOUT_SECONDS
#define WIFI_AP_IDLE_TIMEOUT_SECONDS 300
#endif

#ifndef SERIAL_RX_GPIO
#define SERIAL_RX_GPIO 0
#endif

#ifndef SERIAL_TX_GPIO
#define SERIAL_TX_GPIO 1
#endif

#if defined(ARDUINO_ARCH_ESP32) && defined(ARDUINO_USB_CDC_ON_BOOT) && \
    ARDUINO_USB_CDC_ON_BOOT && defined(ARDUINO_USB_MODE) && ARDUINO_USB_MODE && \
    (SERIAL_RX_GPIO == 18 || SERIAL_RX_GPIO == 19 || SERIAL_TX_GPIO == 18 || \
     SERIAL_TX_GPIO == 19)
#error "SERIAL_RX_GPIO/SERIAL_TX_GPIO must not use GPIO18 or GPIO19 when native USB CDC is enabled on ESP32-C3."
#endif
