#include "web.h"
#include "config_loader.h"

#ifdef ARDUINO
#include "reset_control.h"
#include "serial_bridge.h"
#include "wifi_ap.h"

#include <Arduino.h>
#include <WebServer.h>

namespace {
WebServer server(80);

String htmlHeader(const String &title) {
  return "<!doctype html><html><head><meta charset='utf-8'><meta name='viewport' "
         "content='width=device-width,initial-scale=1'><title>" +
         title +
         "</title><style>body{font-family:sans-serif;max-width:720px;margin:2rem auto;padding:0 "
         "1rem}dt{font-weight:bold}button{font-size:1rem;padding:.5rem 1rem}</style></head><body>";
}

void handleRoot() {
  String page = htmlHeader(NANOBMC_NAME);
  page += "<h1>" NANOBMC_NAME "</h1>";
  page += "<dl>";
  page += "<dt>Firmware version</dt><dd>" NANOBMC_VERSION "</dd>";
  page += "<dt>Uptime</dt><dd>" + String(millis() / 1000) + " seconds</dd>";
  page += "<dt>WiFi AP SSID</dt><dd>" WIFI_AP_SSID "</dd>";
  page += "<dt>IP address</dt><dd>" + String(wifiApIpAddress()) + "</dd>";
  page += "<dt>Serial bridge port</dt><dd>" + String(SERIAL_TCP_PORT) + "</dd>";
  page += "<dt>Serial client connected</dt><dd>" +
          String(serialBridgeClientConnected() ? "yes" : "no") + "</dd>";
  page += "</dl>";
  page += "<form method='post' action='/reset' onsubmit=\"return confirm('Reset the Raspberry "
          "Pi?');\">";
  page += "<button type='submit'>Pulse Pi reset</button></form>";
  page += "</body></html>";
  server.send(200, "text/html", page);
}

void handleReset() {
  pulsePiReset();
  String page = htmlHeader("Reset sent");
  page += "<h1>Reset pulse sent</h1><p>The configured reset GPIO was pulsed.</p>";
  page += "<p><a href='/'>Back to status</a></p></body></html>";
  server.send(200, "text/html", page);
}
} // namespace

void beginWebServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/reset", HTTP_POST, handleReset);
  server.onNotFound([]() { server.send(404, "text/plain", "Not found"); });
  server.begin();
}

void handleWebServer() {
  server.handleClient();
}
#endif
