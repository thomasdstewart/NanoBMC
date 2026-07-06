# Hardware

NanoBMC v0.1.0 targets an ESP32-C3 development board connected to a Raspberry Pi UART and an optional Pi RUN/reset circuit.

## Connection map

Use this table as the single source of truth for the default wiring. The firmware explicitly configures the bridge UART pins from `include/config.h`; the checked-in defaults are ESP32-C3 `GPIO0` for NanoBMC RX and `GPIO1` for NanoBMC TX. Do not assume the board silkscreen pins labeled `RX` and `TX` are correct, because those are often UART0/programming pins rather than the `Serial1` pins used by the TCP bridge.

| NanoBMC signal | ESP32-C3 assignment | ESP32-C3 board label guidance | Raspberry Pi assignment | Raspberry Pi 40-pin header | Purpose |
| --- | --- | --- | --- | --- | --- |
| Ground | `GND` | `GND` | `GND` | Physical pin 6, 9, 14, 20, 25, 30, 34, or 39 | Required common reference |
| UART RX | `GPIO0` by default, configured by `SERIAL_RX_GPIO` | Use the header/pad for `GPIO0`; do not use a silkscreen-only `RX` pin unless your board pinout confirms it is `GPIO0` | `GPIO14` / `TXD0` | Physical pin 8 | Pi console output to NanoBMC |
| UART TX | `GPIO1` by default, configured by `SERIAL_TX_GPIO` | Use the header/pad for `GPIO1`; do not use a silkscreen-only `TX` pin unless your board pinout confirms it is `GPIO1` | `GPIO15` / `RXD0` | Physical pin 10 | NanoBMC input to Pi console |
| Pi reset output | `GPIO4` by default, configured by `PI_RESET_GPIO` | Often `D2` on D1-mini-style boards; verify your board pinout | `RUN` / reset header or pad through safe circuit | Dedicated RUN header/pads; not a normal 40-pin GPIO header pin | Idle-low, pulse-high reset output |

UART TX/RX must be crossed: NanoBMC `SERIAL_RX_GPIO` connects to Pi `GPIO14`/`TXD0`, and NanoBMC `SERIAL_TX_GPIO` connects to Pi `GPIO15`/`RXD0`.

## Raspberry Pi UART notes

- Enable the primary UART/serial console on the Pi before relying on NanoBMC for console access.
- Default serial settings are `115200 8N1` with no flow control.
- On the Pi 40-pin header, `GPIO14` is the Pi transmit pin and `GPIO15` is the Pi receive pin.
- Use any convenient Pi ground pin, but keep the ESP32-C3 and Pi grounds connected.

## ESP32-C3 board-label notes

Many ESP32-C3 boards expose pins labeled `RX` and `TX` for UART0 flashing/logging. NanoBMC's serial bridge uses `Serial1`, configured by `SERIAL_RX_GPIO` and `SERIAL_TX_GPIO`, so wire to the configured GPIO numbers instead of trusting generic `RX`/`TX` labels.

When native USB CDC logging is enabled, do not assign the bridge UART to ESP32-C3 `GPIO18` or `GPIO19`; those pins are used by USB D-/D+ on USB-capable C3 boards and can break USB serial monitoring.

If your board does not break out `GPIO0` and `GPIO1`, choose two safe 3.3 V GPIOs that are available on your board, update `SERIAL_RX_GPIO` and `SERIAL_TX_GPIO` in `include/config.h`, rebuild, and wire the Pi to those configured pins.

## Reset circuit safety

- Use 3.3 V UART logic only.
- Do not connect 5 V signals to ESP32-C3 GPIOs.
- Do not connect the Raspberry Pi `RUN` pin directly to the ESP32-C3 unless your interface circuit is designed for the configured reset polarity.
- By default NanoBMC keeps the reset GPIO low at boot and pulses it high only for the reset duration.
- Use an external transistor/MOSFET or other isolation circuit that translates the default idle-low, pulse-high GPIO into the reset behavior your Pi wiring requires.

## Thermal considerations

Small ESP32-C3 development boards can run warm in normal NanoBMC service, especially when operating continuously as a WiFi access point. Treat heat as a system-level issue rather than assuming the ESP32 package is the only source. Common contributors include:

- **Continuous WiFi AP operation:** Keeping the WiFi radio active for long periods raises ESP32 package temperature. If range allows, reduce AP transmit power in firmware configuration and re-test stability.
- **Onboard linear regulators:** Many development boards use small linear regulators that dissipate the voltage drop from USB or other input supplies as heat. A hot regulator can make the nearby ESP32 feel warm even when the chip itself is within limits.
- **USB-to-serial chips:** Boards with a separate USB-UART bridge can have another warm IC near the ESP32 or regulator, particularly while USB is connected for logging or flashing.
- **Enclosure airflow:** Sealed or tight enclosures trap heat from the ESP32, regulator, USB-to-serial chip, and adjacent Raspberry Pi. Test the board outside the enclosure, then compare temperatures in the final case with the intended lid, cable routing, and mounting orientation.
- **Accidental 5 V loading:** Do not power external 5 V loads from NanoBMC board pins unless the board's regulator and USB supply path are explicitly rated for that current. Extra load can overheat traces, protection devices, or regulators.
- **Back-powering risks:** Wiring mistakes can feed power into the Raspberry Pi through UART, reset, or other NanoBMC connections. Verify the Pi is not drawing power through NanoBMC wiring when its normal supply is removed; only the intended logic signals and common ground should be connected.

Practical checks when diagnosing heat:

1. Measure the ESP32 package separately from the onboard regulator and any USB-to-serial chip, using a thermal camera, contact probe, or careful point measurements.
2. Repeat the same workload with the board outside the enclosure to separate enclosure airflow problems from board-level heat sources.
3. Reduce AP transmit power if local access still works reliably, then confirm UART bridge and web interface behavior over a long run.
4. With the Raspberry Pi's normal power disconnected, verify it is not partially powering up or drawing current through NanoBMC UART/reset wiring.

## Changing pins

Copy `include/config.example.h` to `include/config.h`, then edit `SERIAL_RX_GPIO`, `SERIAL_TX_GPIO`, or `PI_RESET_GPIO` for your board. Rebuild and flash after changing any pin assignment.
