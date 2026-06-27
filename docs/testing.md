# Testing

## Automated tests

Create local configuration, run native tests, then build firmware:

```sh
cp include/config.example.h include/config.h
pio test -e native
pio run -e esp32c3
```

## Manual integration test plan

1. Build firmware with `docs/build.md`.
2. Flash the ESP32-C3 with `docs/flash.md`.
3. Wire the ESP32-C3 to the Raspberry Pi using `docs/hardware.md`.
4. Connect phone/laptop to the NanoBMC AP configured in `include/config.h`.
5. Open `http://192.168.4.1/`.
6. Confirm status page loads.
7. Connect with:

   ```sh
   nc 192.168.4.1 23
   ```

8. Confirm Raspberry Pi serial console appears.
9. Type into console and confirm Pi receives input.
10. Press reset button in web UI.
11. Confirm Pi resets and serial boot messages appear.
