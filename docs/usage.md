# Usage

## Start NanoBMC

Flash the ESP32-C3 with the steps in `docs/flash.md`, wire the board with `docs/hardware.md`, and power it. The USB serial boot log should show the firmware name, version, AP name, IP address, TCP serial port, and web port.

## Connect to WiFi

Join the AP configured in `include/config.h`. The default development values are:

- SSID: `NanoBMC`
- Password: `changeme-nanobmc`
- IP: `192.168.4.1`

Change the password before practical use.

## Web status page

Open:

```sh
curl http://192.168.4.1/
```

A browser is recommended for the reset button because it includes a confirmation prompt before POSTing to `/reset`.

## Serial bridge

Connect with netcat or telnet-style tools:

```sh
nc 192.168.4.1 23
```

The bridge is a raw byte stream, with minimal telnet option negotiation on connect so telnet clients disable local echo and suppress go-ahead. This avoids doubled characters and lets the remote serial console handle erase keys instead of the local telnet client printing `^?` or `^H`. It does not implement full terminal emulation. Only one active serial client is supported; additional clients are rejected while the bridge is busy.

## Reset the Raspberry Pi

Open `http://192.168.4.1/` in a browser and press **Pulse Pi reset**. NanoBMC drives the configured reset GPIO high for 500 ms by default, then returns it low.

## Lower-power access point operation

NanoBMC can reduce heat and power draw by lowering the ESP32-C3 soft-AP transmit power. Copy `include/config.example.h` to `include/config.h`, then tune these AP settings before rebuilding:

- `WIFI_AP_CHANNEL` selects the fixed 2.4 GHz AP channel. Keep it on a locally quiet channel when possible.
- `WIFI_AP_TX_POWER` is passed to the ESP32 Arduino `WiFi.setTxPower(...)` API after the AP starts. The example uses `WIFI_POWER_8_5dBm` instead of the maximum radio power to reduce thermal load.
- `WIFI_AP_IDLE_TIMEOUT_SECONDS` asks the WiFi driver to disconnect idle AP clients after the configured number of seconds. Set it to `0` to leave the driver default unchanged.

Lower transmit power can noticeably reduce ESP32-C3 temperature in enclosed builds, but it also reduces WiFi range and link margin. If your serial bridge drops connections, raise `WIFI_AP_TX_POWER`, move closer to NanoBMC, or improve antenna placement and enclosure ventilation.
