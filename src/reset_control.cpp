#include "reset_control.h"
#include "config_loader.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

bool resetActiveLevel(const ResetPulseConfig &config) {
  return !config.activeLow;
}

bool resetInactiveLevel(const ResetPulseConfig &config) {
  return config.activeLow;
}

void configureResetPin(ResetPinDriver &driver, const ResetPulseConfig &config) {
  driver.setModeOutput(config.gpio);
  driver.writeLevel(config.gpio, resetInactiveLevel(config));
}

void pulseResetPin(ResetPinDriver &driver, const ResetPulseConfig &config) {
  driver.writeLevel(config.gpio, resetActiveLevel(config));
  driver.delayMs(config.pulseMs);
  driver.writeLevel(config.gpio, resetInactiveLevel(config));
}

#ifdef ARDUINO
namespace {
class ArduinoResetPinDriver : public ResetPinDriver {
public:
  void setModeOutput(int gpio) override {
    pinMode(gpio, OUTPUT);
  }
  void writeLevel(int gpio, bool high) override {
    digitalWrite(gpio, high ? HIGH : LOW);
  }
  void delayMs(uint32_t ms) override {
    delay(ms);
  }
};

ArduinoResetPinDriver driver;
ResetPulseConfig config{PI_RESET_GPIO, PI_RESET_ACTIVE_LOW, PI_RESET_PULSE_MS};
} // namespace

void setupResetControl() {
  configureResetPin(driver, config);
}

void pulsePiReset() {
  pulseResetPin(driver, config);
}
#endif
