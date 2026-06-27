# Build

## Install PlatformIO

```sh
python3 -m venv .venv
. .venv/bin/activate
pip install platformio
```

## Create local configuration

```sh
cp include/config.example.h include/config.h
```

Edit `include/config.h` before building:

- `WIFI_AP_SSID` and `WIFI_AP_PASSWORD` set the local service AP credentials.
- `SERIAL_BAUD` defaults to `115200` for the Raspberry Pi serial console.
- `SERIAL_TCP_PORT` defaults to `23` for the raw TCP serial bridge.
- `PI_RESET_GPIO` defaults to `4` for the reset pulse output.
- `PI_RESET_PULSE_MS` defaults to `500` ms.

Change the default AP password before practical use.

## Build firmware

```sh
pio run -e esp32c3
```

The default PlatformIO board is `esp32-c3-devkitm-1`. If your ESP32-C3 D1-style board needs another PlatformIO board ID, update `board = esp32-c3-devkitm-1` in `platformio.ini` before building and flashing.

## Monitor boot logs

```sh
pio device monitor -b 115200
```

The boot log should show the firmware name, version, AP name, AP IP address, TCP serial port, and web port.
