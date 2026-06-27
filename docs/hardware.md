# Hardware

NanoBMC v0.1.0 targets an ESP32-C3 development board connected to a Raspberry Pi UART and an optional Pi RUN/reset circuit.

## Connection map

Use this table as the single source of truth for the default wiring. The firmware default reset output is ESP32-C3 `GPIO4`; UART uses the board's Arduino `Serial1` pins unless you customize `src/serial_bridge.cpp`.

| NanoBMC signal | ESP32-C3 assignment | Common ESP32-C3 D1-style silkscreen | Raspberry Pi assignment | Raspberry Pi 40-pin header | Purpose |
| --- | --- | --- | --- | --- | --- |
| Ground | `GND` | `GND` | `GND` | Physical pin 6, 9, 14, 20, 25, 30, 34, or 39 | Required common reference |
| UART RX | Board `Serial1` RX | Often `RX` / `RXD`; verify your board pinout | `GPIO14` / `TXD0` | Physical pin 8 | Pi console output to NanoBMC |
| UART TX | Board `Serial1` TX | Often `TX` / `TXD`; verify your board pinout | `GPIO15` / `RXD0` | Physical pin 10 | NanoBMC input to Pi console |
| Pi reset output | `GPIO4` by default | Often `D2` on D1-mini-style boards; verify your board pinout | `RUN` / reset header or pad through safe circuit | Dedicated RUN header/pads; not a normal 40-pin GPIO header pin | Active-low reset pulse |

UART TX/RX must be crossed: NanoBMC RX connects to Pi TXD0, and NanoBMC TX connects to Pi RXD0.

## Raspberry Pi UART notes

- Enable the primary UART/serial console on the Pi before relying on NanoBMC for console access.
- Default serial settings are `115200 8N1` with no flow control.
- On the Pi 40-pin header, `GPIO14` is the Pi transmit pin and `GPIO15` is the Pi receive pin.
- Use any convenient Pi ground pin, but keep the ESP32-C3 and Pi grounds connected.

## Reset circuit safety

- Use 3.3 V UART logic only.
- Do not connect 5 V signals to ESP32-C3 GPIOs.
- Do not drive the Raspberry Pi `RUN` pin directly high from the ESP32-C3.
- Treat reset as active-low: NanoBMC should only pull the reset circuit low, then release it.
- Use open-drain behaviour or an external transistor/MOSFET circuit for reset isolation.

## Changing pins

Copy `include/config.example.h` to `include/config.h`, then edit `PI_RESET_GPIO` for your board. If your ESP32-C3 board does not route Arduino `Serial1` to the pins you wired, update `Serial1.begin(...)` in `src/serial_bridge.cpp` to pass explicit RX/TX GPIOs for your board.
