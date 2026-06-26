# Hardware

NanoBMC v0.1.0 targets an ESP32-C3 development board connected to a Raspberry Pi UART and an optional Pi RUN/reset circuit.

## Default wiring

| ESP32-C3 | Raspberry Pi | Purpose |
| --- | --- | --- |
| GND | GND | Common reference |
| RX | Pi TX | Pi console output to NanoBMC |
| TX | Pi RX | NanoBMC input to Pi console |
| GPIO 4 | RUN/reset circuit | Active-low reset pulse |

## Electrical safety

- Use 3.3 V UART logic.
- Do not connect 5 V signals to ESP32-C3 GPIOs.
- Do not drive the Raspberry Pi RUN pin directly high from the ESP32-C3.
- Use open-drain behaviour or an external transistor/MOSFET circuit for reset.
- Keep grounds connected between the ESP32-C3 and Raspberry Pi.

## Changing pins

Copy `include/config.example.h` to `include/config.h`, then edit `PI_RESET_GPIO`. UART pin assignment uses the board's Arduino `Serial1` defaults for the initial simple firmware. If your board needs explicit UART pins, update `Serial1.begin(...)` in `src/serial_bridge.cpp`.
