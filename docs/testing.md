# Testing

## Automated tests

Create local configuration, run native tests, then build firmware:

```sh
cp include/config.example.h include/config.h
pio test -e native
pio run -e esp32c3
```

## Manual integration test plan

1. Build firmware.
2. Upload to ESP32-C3.
3. Connect phone/laptop to NanoBMC AP.
4. Open `http://192.168.4.1/`.
5. Confirm status page loads.
6. Connect with:

   ```sh
   nc 192.168.4.1 23
   ```

7. Confirm Raspberry Pi serial console appears.
8. Type into console and confirm Pi receives input.
9. Press reset button in web UI.
10. Confirm Pi resets and serial boot messages appear.
