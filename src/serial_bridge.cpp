#include "serial_bridge.h"
#include "config_loader.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <WiFi.h>
#endif

namespace {
constexpr uint8_t kTelnetIac = 255;
constexpr uint8_t kTelnetWill = 251;
constexpr uint8_t kTelnetDo = 253;
constexpr uint8_t kTelnetEcho = 1;
constexpr uint8_t kTelnetSuppressGoAhead = 3;

enum class TelnetParseState {
  Data,
  Command,
  Option,
};

TelnetParseState telnetState = TelnetParseState::Data;

bool shouldForwardClientByte(uint8_t byte) {
  switch (telnetState) {
  case TelnetParseState::Data:
    if (byte == kTelnetIac) {
      telnetState = TelnetParseState::Command;
      return false;
    }
    return true;
  case TelnetParseState::Command:
    if (byte == kTelnetIac) {
      telnetState = TelnetParseState::Data;
      return true;
    }
    if (byte >= 251 && byte <= 254) {
      telnetState = TelnetParseState::Option;
    } else {
      telnetState = TelnetParseState::Data;
    }
    return false;
  case TelnetParseState::Option:
    telnetState = TelnetParseState::Data;
    return false;
  }
  return true;
}
} // namespace

void SerialBridgePump::resetClientSession() {
  telnetState = TelnetParseState::Data;
}

size_t SerialBridgePump::sendTelnetGreeting(SerialBridgeIo &io) {
  static constexpr uint8_t kGreeting[] = {
      kTelnetIac, kTelnetWill, kTelnetEcho,
      kTelnetIac, kTelnetWill, kTelnetSuppressGoAhead,
      kTelnetIac, kTelnetDo, kTelnetSuppressGoAhead,
  };

  size_t moved = 0;
  for (const uint8_t byte : kGreeting) {
    moved += io.clientWrite(byte);
  }
  return moved;
}

size_t SerialBridgePump::pump(SerialBridgeIo &io) {
  if (!io.clientConnected()) {
    io.stopClient();
    resetClientSession();
    return 0;
  }

  size_t moved = 0;
  while (io.clientAvailable() > 0) {
    const int value = io.clientRead();
    if (value < 0) {
      break;
    }
    const uint8_t byte = static_cast<uint8_t>(value);
    if (shouldForwardClientByte(byte)) {
      moved += io.uartWrite(byte);
    }
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
  Serial1.begin(SERIAL_BAUD, SERIAL_8N1, SERIAL_RX_GPIO, SERIAL_TX_GPIO);
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
      pump.resetClientSession();
      pump.sendTelnetGreeting(io);
    }
  }
  pump.pump(io);
}

bool serialBridgeClientConnected() {
  return client && client.connected();
}
#endif
