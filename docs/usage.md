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

The status page also reports ESP32 diagnostics when the active Arduino-ESP32 core exposes the required APIs:

- **Free heap** is the currently available heap in bytes. It will change as web or serial clients connect and disconnect; watch for a continuing downward trend across refreshes rather than a single low sample.
- **Chip temperature** is shown in degrees Celsius only on Arduino-ESP32 target/core combinations that support `temperatureRead()` via the SoC temperature sensor capability. Treat it as an internal chip reading for trend monitoring, not an ambient air temperature. Unsupported builds show an explicit unsupported message instead of a number.
- **AP client count** is the number of stations associated with the NanoBMC access point. This can include a browser, a telnet/netcat serial client host, or any device still connected to the AP.
- **WiFi transmit power** is the configured ESP32 radio transmit-power setting in dBm. It is the requested/configured power level reported by the WiFi stack, not a measurement of received signal strength at the client.

## Serial bridge

Connect with netcat or telnet-style tools:

```sh
nc 192.168.4.1 23
```

The bridge is a raw byte stream, with minimal telnet option negotiation on connect so telnet clients disable local echo and suppress go-ahead. This avoids doubled characters and lets the remote serial console handle erase keys instead of the local telnet client printing `^?` or `^H`. It does not implement full terminal emulation. Only one active serial client is supported; additional clients are rejected while the bridge is busy.

## Reset the Raspberry Pi

Open `http://192.168.4.1/` in a browser and press **Pulse Pi reset**. NanoBMC drives the configured reset GPIO high for 500 ms by default, then returns it low.
