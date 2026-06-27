# Flash

These steps are written for ESP32-C3 development boards, including D1-mini-style boards with BOOT/FLASH and RESET buttons. Exact button names vary by board.

## Before flashing

1. Build the firmware with the steps in `docs/build.md`.
2. Disconnect the ESP32-C3 from the Raspberry Pi if you are unsure about the wiring.
3. Connect the ESP32-C3 to your computer with a data-capable USB cable.
4. If your board has a boot-mode jumper instead of a BOOT/FLASH button, place the jumper so the board enters the ROM bootloader on reset. On ESP32-C3 boards this usually means holding `GPIO9` low while resetting, but verify your board's pinout before shorting pins.

## Automatic upload

Most ESP32-C3 dev boards auto-reset into the bootloader from USB serial control lines:

```sh
pio run -e esp32c3 -t upload
```

If upload succeeds, PlatformIO writes the firmware and resets the board. Start the monitor with:

```sh
pio device monitor -b 115200
```

## Manual bootloader upload

Use this sequence if automatic upload waits for a connection, times out, or repeatedly prints bootloader sync errors:

1. Start the upload command:

   ```sh
   pio run -e esp32c3 -t upload
   ```

2. Hold the board's **BOOT** or **FLASH** button. On jumper-only boards, install the boot-mode jumper.
3. Press and release **RESET** while still holding **BOOT/FLASH**.
4. Release **BOOT/FLASH** after PlatformIO reports that it is connecting or writing.
5. Wait for the upload to finish.
6. Press **RESET** once more, or remove the boot-mode jumper and then press **RESET**, to boot NanoBMC normally.
7. Open the serial monitor:

   ```sh
   pio device monitor -b 115200
   ```

## After flashing

1. Confirm the boot log reports the NanoBMC AP and `192.168.4.1` address.
2. Power down before connecting the Raspberry Pi UART/reset wiring.
3. Wire the board using `docs/hardware.md`.
4. Power the ESP32-C3 and Pi.
5. Join the NanoBMC WiFi network and continue with `docs/usage.md`.

## Troubleshooting upload

- Use a USB data cable, not a charge-only cable.
- Close any serial monitor before uploading.
- Select the correct serial port if multiple USB serial devices are connected, for example `pio run -e esp32c3 -t upload --upload-port /dev/ttyUSB0`.
- If a D1-style board requires manual boot mode, hold **BOOT/FLASH**, tap **RESET**, run or retry upload, then tap **RESET** again after flashing.
- If uploads work only with the Pi disconnected, re-check common ground, TX/RX crossing, and reset wiring before reconnecting.
