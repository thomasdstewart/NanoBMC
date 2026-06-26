#include <string>
#include <unity.h>

#include "serial_bridge.h"

class FakeSerialBridgeIo : public SerialBridgeIo {
public:
  std::string uartIn;
  std::string uartOut;
  std::string clientIn;
  std::string clientOut;
  bool connected = true;
  bool stopped = false;

  int uartAvailable() override {
    return static_cast<int>(uartIn.size());
  }
  int uartRead() override {
    if (uartIn.empty()) {
      return -1;
    }
    const unsigned char value = uartIn.front();
    uartIn.erase(0, 1);
    return value;
  }
  size_t uartWrite(uint8_t byte) override {
    uartOut.push_back(static_cast<char>(byte));
    return 1;
  }
  bool clientConnected() const override {
    return connected;
  }
  int clientAvailable() override {
    return static_cast<int>(clientIn.size());
  }
  int clientRead() override {
    if (clientIn.empty()) {
      return -1;
    }
    const unsigned char value = clientIn.front();
    clientIn.erase(0, 1);
    return value;
  }
  size_t clientWrite(uint8_t byte) override {
    clientOut.push_back(static_cast<char>(byte));
    return 1;
  }
  void stopClient() override {
    stopped = true;
  }
};

void test_bridge_moves_simple_byte_sequences() {
  FakeSerialBridgeIo io;
  io.clientIn = "help\r";
  io.uartIn = "login:";
  SerialBridgePump pump;

  const size_t moved = pump.pump(io);

  TEST_ASSERT_EQUAL_STRING("help\r", io.uartOut.c_str());
  TEST_ASSERT_EQUAL_STRING("login:", io.clientOut.c_str());
  TEST_ASSERT_EQUAL(11, moved);
}

void test_bridge_handles_disconnected_client() {
  FakeSerialBridgeIo io;
  io.connected = false;
  io.clientIn = "ignored";
  SerialBridgePump pump;

  const size_t moved = pump.pump(io);

  TEST_ASSERT_EQUAL(0, moved);
  TEST_ASSERT_TRUE(io.stopped);
  TEST_ASSERT_TRUE(io.uartOut.empty());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_bridge_moves_simple_byte_sequences);
  RUN_TEST(test_bridge_handles_disconnected_client);
  return UNITY_END();
}
