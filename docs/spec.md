NanoBMC Specification

Goal

Build a small ESP32-C3 based “poor man’s BMC” for a headless Raspberry Pi / Compute Module.

The device should provide:

- WiFi access point for local servicing
- TCP-to-UART serial console bridge
- Optional web status page
- GPIO output to trigger Raspberry Pi RUN/reset
- Simple, maintainable PlatformIO project
- Lightweight DevOps: pre-commit, GitHub Actions, tests, docs

This is not intended to become a large reusable framework. Prefer simple, readable code over excessive abstraction.

Target Hardware

Initial target:

- ESP32-C3 development board
- PlatformIO
- Arduino framework unless ESP-IDF is clearly simpler
- Raspberry Pi UART on GPIO header
- Optional Pi RUN/reset connection via GPIO through safe open-drain-style circuit

Default serial settings:

- Baud: 115200
- Data bits: 8
- Parity: none
- Stop bits: 1
- Flow control: none

Suggested wiring:

ESP32-C3 GND  -> Raspberry Pi GND
ESP32-C3 RX   -> Raspberry Pi TX
ESP32-C3 TX   -> Raspberry Pi RX
ESP32-C3 GPIO -> Raspberry Pi RUN/reset circuit

Do not drive the Pi RUN pin directly high. Treat reset as active-low. Use open-drain behaviour or an external transistor/MOSFET circuit.

Project Name

Use "nanobmc".

Core Features

1. WiFi AP mode

On boot, NanoBMC should start a WiFi access point.

Default values:

SSID: NanoBMC
Password: changeme-nanobmc
IP: 192.168.4.1

The password must be configurable.

Station mode can be added later, but AP mode is the priority because this is for local field servicing.

2. TCP serial bridge

Expose a TCP listener that bridges to the Pi UART.

Default:

Port: 23
Protocol: raw TCP / telnet-style byte stream

Requirements:

- A client can connect using "nc", "telnet", or an Android terminal app.
- Bytes received from TCP are written to UART.
- Bytes received from UART are sent to the connected TCP client.
- Only one active serial client is required.
- If a second client connects, either reject it or replace the existing client. Choose the simpler safe behaviour and document it.
- No terminal emulation is required.
- No web terminal is required in v1.

3. Web status page

Expose a minimal HTTP page on port 80.

The page should show:

- Project name and firmware version
- Uptime
- WiFi AP SSID
- IP address
- Serial bridge port
- Whether a serial client is connected
- Reset button

The reset button should trigger a Pi reset action only after a confirmation page or POST request. Avoid accidental resets from page refresh.

4. Pi reset GPIO

Add a function to pulse the configured reset GPIO.

Default behaviour:

Pull reset GPIO active for 500 ms
Release reset GPIO

The reset GPIO must be configurable or easy to change in one header file.

The implementation should clearly separate:

- UART bridge logic
- WiFi setup
- Web server
- Reset GPIO control
- Configuration

5. Configuration

Use a checked-in sample config and an ignored local config.

Required files:

include/config.example.h
include/config.h       # ignored by git

"config.example.h" should include:

#pragma once

#define NANOBMC_NAME "NanoBMC"
#define NANOBMC_VERSION "0.1.0"

#define WIFI_AP_SSID "NanoBMC"
#define WIFI_AP_PASSWORD "changeme-nanobmc"

#define SERIAL_BAUD 115200
#define SERIAL_TCP_PORT 23

#define PI_RESET_GPIO 4
#define PI_RESET_ACTIVE_LOW true
#define PI_RESET_PULSE_MS 500

"README.md" must explain:

cp include/config.example.h include/config.h

Repository Layout

Create this layout:

.
├── .github/
│   └── workflows/
│       └── ci.yml
├── docs/
│   ├── hardware.md
│   ├── build.md
│   ├── flash.md
│   ├── usage.md
│   └── testing.md
├── include/
│   ├── config.example.h
│   └── config.h              # ignored
├── src/
│   ├── main.cpp
│   ├── config_loader.h
│   ├── serial_bridge.cpp
│   ├── serial_bridge.h
│   ├── wifi_ap.cpp
│   ├── wifi_ap.h
│   ├── web.cpp
│   ├── web.h
│   ├── reset_control.cpp
│   └── reset_control.h
├── test/
│   ├── test_serial_bridge/
│   └── test_reset_control/
├── .clang-format
├── .gitignore
├── .pre-commit-config.yaml
├── LICENSE
├── README.md
├── platformio.ini
└── spec.md

Keep the structure simple. If PlatformIO unit testing makes some files awkward, use small pure C++ helper classes/functions that can be tested natively.

PlatformIO

Use PlatformIO with an ESP32-C3 environment and a native test environment.

Example intent:

[platformio]
default_envs = esp32c3

[env:esp32c3]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
monitor_speed = 115200

[env:native]
platform = native
test_framework = unity

If the exact board differs, document how to change "board =".

Tests

Add unit tests for logic that can run natively.

Minimum tests:

- Reset pulse configuration validates active-low behaviour.
- Reset pulse duration is read from config.
- Serial bridge buffer handling does not drop simple byte sequences.
- Serial bridge handles disconnected client state.

Do not spend too long trying to test ESP WiFi hardware in native tests. Keep hardware integration testing manual and documented.

Integration Testing

Add "docs/testing.md" with a simple manual test plan:

1. Build firmware.
2. Upload to ESP32-C3.
3. Connect phone/laptop to NanoBMC AP.
4. Open "http://192.168.4.1/".
5. Confirm status page loads.
6. Connect with:

nc 192.168.4.1 23

7. Confirm Raspberry Pi serial console appears.
8. Type into console and confirm Pi receives input.
9. Press reset button in web UI.
10. Confirm Pi resets and serial boot messages appear.

Pre-commit

Add pre-commit hooks.

Use simple hooks:

- trailing whitespace
- end of file fixer
- check YAML
- check added large files
- clang-format for C/C++ files

Do not add heavy dependencies unless needed.

GitHub Actions

Create ".github/workflows/ci.yml".

CI should run on:

- push
- pull_request

CI should:

1. Check out repository.
2. Install Python.
3. Install PlatformIO.
4. Copy "include/config.example.h" to "include/config.h".
5. Run native tests.
6. Build ESP32-C3 firmware.

Example commands:

pip install platformio
cp include/config.example.h include/config.h
pio test -e native
pio run -e esp32c3

Conventional Commits

Document that commits should use Conventional Commits.

Examples:

feat: add TCP serial bridge
fix: avoid reset on web page refresh
docs: add wiring diagram
test: add reset control tests
ci: build firmware in GitHub Actions

No commit-msg enforcement is required initially unless it is very easy to add. Mention the convention in "README.md".

README Requirements

"README.md" should include:

- What NanoBMC is
- What it is not
- Hardware required
- Wiring table
- Build environment setup
- Config setup
- Build command
- Upload command
- Serial monitor command
- How to connect to AP
- How to open serial console with "nc"
- How to reset the Pi
- Safety notes about 3.3 V logic and RUN pin
- Troubleshooting

Build examples:

python3 -m venv .venv
. .venv/bin/activate
pip install platformio
cp include/config.example.h include/config.h
pio run -e esp32c3
pio run -e esp32c3 -t upload
pio device monitor -b 115200

Usage examples:

# Join WiFi network:
# SSID: NanoBMC
# Password: changeme-nanobmc

curl http://192.168.4.1/

nc 192.168.4.1 23

Security Notes

This is intentionally simple, but include basic safety:

- Default password must be changed.
- Do not expose NanoBMC AP permanently unless needed.
- Do not connect it to untrusted networks in station mode until authentication is added.
- The TCP serial bridge has no authentication in v1.
- Physical proximity is assumed for v1.
- Serial console may allow root access to the Pi depending on Pi configuration.

Versioning

Start at version "0.1.0".

Expose version in:

- "include/config.example.h"
- web status page
- serial boot log

Firmware Behaviour

On boot:

1. Initialise serial debug output over USB serial.
2. Initialise Pi UART.
3. Configure reset GPIO inactive.
4. Start WiFi AP.
5. Start TCP serial bridge.
6. Start web server.
7. Print status to USB serial.

Example boot log:

NanoBMC 0.1.0
Starting WiFi AP: NanoBMC
AP IP: 192.168.4.1
Serial bridge listening on port 23
Web UI listening on port 80
Ready

Acceptance Criteria

The project is complete when:

- "pio run -e esp32c3" succeeds.
- "pio test -e native" succeeds.
- GitHub Actions builds firmware and runs tests.
- Pre-commit config exists and works.
- README explains build, upload, wiring, and use.
- ESP32-C3 starts an AP.
- Web status page loads.
- TCP client can bridge to UART.
- Reset button pulses configured GPIO.
- Hardware docs warn about 3.3 V logic and safe RUN pin handling.

Avoid

Avoid in v1:

- MQTT
- TLS
- OTA updates
- captive portal
- BLE
- station mode
- multiple users
- complex config storage
- filesystem
- JSON APIs unless needed
- enterprise-grade auth
- Docker unless it genuinely helps CI

Keep v1 small and useful.
