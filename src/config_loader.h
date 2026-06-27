#pragma once

#if __has_include("config.h")
#include "config.h"
#else
#include "config.example.h"
#endif

#ifndef SERIAL_RX_GPIO
#define SERIAL_RX_GPIO 18
#endif

#ifndef SERIAL_TX_GPIO
#define SERIAL_TX_GPIO 19
#endif
