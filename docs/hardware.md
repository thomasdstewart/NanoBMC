# Hardware

NanoBMC v0.1.0 targets an ESP32-C3 development board connected to a Raspberry Pi UART and an optional Pi RUN/reset circuit.

## Connection map

Use this table as the single source of truth for the default wiring. The firmware explicitly configures the bridge UART pins from `include/config.h`; the checked-in defaults are ESP32-C3 `GPIO18` for NanoBMC RX and `GPIO19` for NanoBMC TX. Do not assume the board silkscreen pins labeled `RX` and `TX` are correct, because those are often UART0/programming pins rather than the `Serial1` pins used by the TCP bridge.

| NanoBMC signal | ESP32-C3 assignment | ESP32-C3 board label guidance | Raspberry Pi assignment | Raspberry Pi 40-pin header | Purpose |
| --- | --- | --- | --- | --- | --- |
| Ground | `GND` | `GND` | `GND` | Physical pin 6, 9, 14, 20, 25, 30, 34, or 39 | Required common reference |
| UART RX | `GPIO18` by default, configured by `SERIAL_RX_GPIO` | Use the header/pad for `GPIO18`; do not use a silkscreen-only `RX` pin unless your board pinout confirms it is `GPIO18` | `GPIO14` / `TXD0` | Physical pin 8 | Pi console output to NanoBMC |
| UART TX | `GPIO19` by default, configured by `SERIAL_TX_GPIO` | Use the header/pad for `GPIO19`; do not use a silkscreen-only `TX` pin unless your board pinout confirms it is `GPIO19` | `GPIO15` / `RXD0` | Physical pin 10 | NanoBMC input to Pi console |
| Pi reset output | `GPIO4` by default, configured by `PI_RESET_GPIO` | Often `D2` on D1-mini-style boards; verify your board pinout | `RUN` / reset header or pad through safe circuit | Dedicated RUN header/pads; not a normal 40-pin GPIO header pin | Active-low reset pulse |

UART TX/RX must be crossed: NanoBMC `SERIAL_RX_GPIO` connects to Pi `GPIO14`/`TXD0`, and NanoBMC `SERIAL_TX_GPIO` connects to Pi `GPIO15`/`RXD0`.

## Raspberry Pi UART notes

- Enable the primary UART/serial console on the Pi before relying on NanoBMC for console access.
- Default serial settings are `115200 8N1` with no flow control.
- On the Pi 40-pin header, `GPIO14` is the Pi transmit pin and `GPIO15` is the Pi receive pin.
- Use any convenient Pi ground pin, but keep the ESP32-C3 and Pi grounds connected.

## ESP32-C3 board-label notes

Many ESP32-C3 boards expose pins labeled `RX` and `TX` for UART0 flashing/logging. NanoBMC's serial bridge uses `Serial1`, configured by `SERIAL_RX_GPIO` and `SERIAL_TX_GPIO`, so wire to the configured GPIO numbers instead of trusting generic `RX`/`TX` labels.

If your board does not break out `GPIO18` and `GPIO19`, choose two safe 3.3 V GPIOs that are available on your board, update `SERIAL_RX_GPIO` and `SERIAL_TX_GPIO` in `include/config.h`, rebuild, and wire the Pi to those configured pins.

## Reset circuit safety

- Use 3.3 V UART logic only.
- Do not connect 5 V signals to ESP32-C3 GPIOs.
- Do not drive the Raspberry Pi `RUN` pin directly high from the ESP32-C3.
- Treat reset as active-low: NanoBMC should only pull the reset circuit low, then release it.
- Use open-drain behaviour or an external transistor/MOSFET circuit for reset isolation.

## Changing pins

Copy `include/config.example.h` to `include/config.h`, then edit `SERIAL_RX_GPIO`, `SERIAL_TX_GPIO`, or `PI_RESET_GPIO` for your board. Rebuild and flash after changing any pin assignment.
