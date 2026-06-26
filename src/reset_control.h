#pragma once

#include <stdint.h>

struct ResetPulseConfig {
  int gpio;
  bool activeLow;
  uint32_t pulseMs;
};

class ResetPinDriver {
public:
  virtual ~ResetPinDriver() = default;
  virtual void setModeOutput(int gpio) = 0;
  virtual void writeLevel(int gpio, bool high) = 0;
  virtual void delayMs(uint32_t ms) = 0;
};

bool resetActiveLevel(const ResetPulseConfig &config);
bool resetInactiveLevel(const ResetPulseConfig &config);
void configureResetPin(ResetPinDriver &driver, const ResetPulseConfig &config);
void pulseResetPin(ResetPinDriver &driver, const ResetPulseConfig &config);

#ifdef ARDUINO
void setupResetControl();
void pulsePiReset();
#endif
