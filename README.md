# NanoBMC

NanoBMC is a small ESP32-C3 based "poor man's BMC" for a headless Raspberry Pi or Compute Module. It starts a local WiFi access point, exposes a raw TCP-to-UART serial bridge on port 23, serves a minimal status page, and can pulse a configured GPIO to reset the Pi through a safe active-low RUN/reset circuit.

## What it is not

NanoBMC v0.1.0 intentionally does not include MQTT, OTA updates, BLE, captive portal, TLS, station mode, a web terminal, complex config storage, or authentication. Physical proximity to the AP is assumed.

## Documentation

- [Hardware](docs/hardware.md): connection map, pin assignments, Raspberry Pi header pins, and reset circuit safety.
- [Build](docs/build.md): PlatformIO setup, local configuration, and firmware build commands.
- [Flash](docs/flash.md): automatic and manual ESP32-C3 flashing workflows, including BOOT/FLASH plus RESET timing.
- [Usage](docs/usage.md): WiFi AP, web status page, serial bridge, and reset operation.
- [Testing](docs/testing.md): automated tests and manual integration test plan.
- [Specification](docs/spec.md): project goals and design constraints.

## Quick start

```sh
python3 -m venv .venv
. .venv/bin/activate
pip install platformio
cp include/config.example.h include/config.h
pio run -e esp32c3
pio run -e esp32c3 -t upload
pio device monitor -b 115200
```

Before practical use, edit `include/config.h` to change the default AP password and confirm the reset GPIO matches your wiring.

## Hardware summary

Required hardware:

- ESP32-C3 development board
- Raspberry Pi or Compute Module with UART enabled
- Jumper wires and common ground
- Safe Pi RUN/reset interface, such as an open-drain style transistor/MOSFET circuit

See [docs/hardware.md](docs/hardware.md) for the canonical wiring table. Do not drive the Pi RUN pin directly high; use 3.3 V logic only and treat reset as active-low.

## Default access

- SSID: `NanoBMC`
- Password: `changeme-nanobmc`
- IP: `192.168.4.1`
- Serial bridge: TCP port `23`

Change the default password before real use.

## Common commands

```sh
pio test -e native
pio run -e esp32c3
pio run -e esp32c3 -t upload
pio device monitor -b 115200
```

## Security notes

- Change the default AP password.
- Do not expose the AP permanently unless needed.
- The TCP serial bridge has no authentication in v0.1.0.
- Serial console access may allow root access depending on Pi configuration.
- Do not add station mode on untrusted networks without authentication and transport security.

## Conventional commits

Commit messages should follow Conventional Commits, for example:

- `feat: add TCP serial bridge`
- `fix: avoid reset on web page refresh`
- `docs: add wiring diagram`
- `test: add reset control tests`
- `ci: build firmware in GitHub Actions`
