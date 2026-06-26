#pragma once

#include <stddef.h>
#include <stdint.h>

class SerialBridgeIo {
public:
  virtual ~SerialBridgeIo() = default;
  virtual int uartAvailable() = 0;
  virtual int uartRead() = 0;
  virtual size_t uartWrite(uint8_t byte) = 0;
  virtual bool clientConnected() const = 0;
  virtual int clientAvailable() = 0;
  virtual int clientRead() = 0;
  virtual size_t clientWrite(uint8_t byte) = 0;
  virtual void stopClient() = 0;
};

class SerialBridgePump {
public:
  size_t pump(SerialBridgeIo &io);
};

#ifdef ARDUINO
void beginSerialBridge();
void handleSerialBridge();
bool serialBridgeClientConnected();
#endif
