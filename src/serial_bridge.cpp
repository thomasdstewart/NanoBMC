#include "serial_bridge.h"
#include "config_loader.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <WiFi.h>
#endif

size_t SerialBridgePump::pump(SerialBridgeIo &io) {
  if (!io.clientConnected()) {
    io.stopClient();
    return 0;
  }

  size_t moved = 0;
  while (io.clientAvailable() > 0) {
    const int value = io.clientRead();
    if (value < 0) {
      break;
    }
    moved += io.uartWrite(static_cast<uint8_t>(value));
  }

  while (io.uartAvailable() > 0 && io.clientConnected()) {
    const int value = io.uartRead();
    if (value < 0) {
      break;
    }
    moved += io.clientWrite(static_cast<uint8_t>(value));
  }

  return moved;
}

#ifdef ARDUINO
namespace {
WiFiServer server(SERIAL_TCP_PORT);
WiFiClient client;
SerialBridgePump pump;

class ArduinoSerialBridgeIo : public SerialBridgeIo {
public:
  int uartAvailable() override {
    return Serial1.available();
  }
  int uartRead() override {
    return Serial1.read();
  }
  size_t uartWrite(uint8_t byte) override {
    return Serial1.write(byte);
  }
  bool clientConnected() const override {
    return client && client.connected();
  }
  int clientAvailable() override {
    return client.available();
  }
  int clientRead() override {
    return client.read();
  }
  size_t clientWrite(uint8_t byte) override {
    return client.write(byte);
  }
  void stopClient() override {
    if (client) {
      client.stop();
    }
  }
};

ArduinoSerialBridgeIo io;
} // namespace

void beginSerialBridge() {
  Serial1.begin(SERIAL_BAUD, SERIAL_8N1);
  server.begin();
  server.setNoDelay(true);
}

void handleSerialBridge() {
  WiFiClient incoming = server.available();
  if (incoming) {
    if (client && client.connected()) {
      incoming.println("NanoBMC serial bridge is busy; try again later.");
      incoming.stop();
    } else {
      client = incoming;
      client.setNoDelay(true);
    }
  }
  pump.pump(io);
}

bool serialBridgeClientConnected() {
  return client && client.connected();
}
#endif
