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

The bridge is a raw byte stream. It does not implement terminal emulation. Only one active serial client is supported; additional clients are rejected while the bridge is busy.

## Reset the Raspberry Pi

Open `http://192.168.4.1/` in a browser and press **Pulse Pi reset**. NanoBMC pulls the configured reset GPIO active for 500 ms by default, then releases it.
