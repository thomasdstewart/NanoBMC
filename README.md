# NanoBMC

NanoBMC is a small ESP32-C3 based "poor man's BMC" for a headless Raspberry Pi or Compute Module. It starts a local WiFi access point, exposes a raw TCP-to-UART serial bridge on port 23, serves a minimal status page, and can pulse a configured GPIO to reset the Pi through a safe active-low RUN/reset circuit.

## What it is not

NanoBMC v0.1.0 intentionally does not include MQTT, OTA updates, BLE, captive portal, TLS, station mode, a web terminal, complex config storage, or authentication. Physical proximity to the AP is assumed.

## Hardware required

- ESP32-C3 development board
- Raspberry Pi or Compute Module with UART enabled
- Jumper wires and common ground
- Safe Pi RUN/reset interface, such as an open-drain style transistor/MOSFET circuit

## Wiring

| ESP32-C3 | Raspberry Pi | Notes |
| --- | --- | --- |
| GND | GND | Required common ground |
| RX | Pi TX | 3.3 V UART |
| TX | Pi RX | 3.3 V UART |
| GPIO 4 by default | RUN/reset circuit | Active-low pulse through safe open-drain style circuit |

Do not drive the Pi RUN pin directly high. Use 3.3 V logic only and treat reset as active-low.

## Build environment setup

```sh
python3 -m venv .venv
. .venv/bin/activate
pip install platformio
```

## Configuration

Create your local config before building:

```sh
cp include/config.example.h include/config.h
```

Edit `include/config.h` to change the AP password, reset GPIO, serial baud rate, or TCP port. `include/config.h` is ignored by git so local secrets and board-specific settings are not committed.

Default AP credentials:

- SSID: `NanoBMC`
- Password: `changeme-nanobmc`
- IP: `192.168.4.1`

Change the default password before real use.

## Build, upload, and monitor

```sh
pio run -e esp32c3
pio run -e esp32c3 -t upload
pio device monitor -b 115200
```

If your ESP32-C3 board differs, change `board = esp32-c3-devkitm-1` in `platformio.ini` to the matching PlatformIO board ID.

## Usage

Join the NanoBMC WiFi network, then open the status page:

```sh
curl http://192.168.4.1/
```

Connect to the Pi serial console:

```sh
nc 192.168.4.1 23
```

Only one serial client is supported. If a second client connects while the first is active, NanoBMC rejects the second client with a short busy message.

To reset the Pi, open `http://192.168.4.1/` in a browser and press **Pulse Pi reset**. The button sends a POST request and includes a browser confirmation to avoid accidental resets on refresh.

## Tests

```sh
pio test -e native
```

Native tests cover reset pulse logic and TCP/UART bridge byte-pumping logic. Hardware WiFi, UART pins, and reset wiring are tested manually.

## Pre-commit

```sh
pip install pre-commit
pre-commit install
pre-commit run --all-files
```

Hooks check whitespace, EOF newlines, YAML syntax, large files, and C/C++ formatting.

## Conventional commits

Commit messages should follow Conventional Commits, for example:

- `feat: add TCP serial bridge`
- `fix: avoid reset on web page refresh`
- `docs: add wiring diagram`
- `test: add reset control tests`
- `ci: build firmware in GitHub Actions`

## Security notes

- Change the default AP password.
- Do not expose the AP permanently unless needed.
- The TCP serial bridge has no authentication in v0.1.0.
- Serial console access may allow root access depending on Pi configuration.
- Do not add station mode on untrusted networks without authentication and transport security.

## Troubleshooting

- No AP appears: check the ESP32-C3 board selection and USB power.
- Cannot reach `192.168.4.1`: confirm your laptop or phone joined the NanoBMC AP.
- No serial console: verify Pi UART is enabled and TX/RX are crossed.
- Garbage serial output: confirm both sides use 115200 8N1 by default.
- Reset does not work: verify the configured GPIO and open-drain style RUN/reset circuit.
