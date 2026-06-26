#include <unity.h>

#include "config_loader.h"
#include "reset_control.h"

class FakeResetPinDriver : public ResetPinDriver {
public:
  int modeGpio = -1;
  bool levels[4]{};
  int levelCount = 0;
  uint32_t delayValue = 0;

  void setModeOutput(int gpio) override {
    modeGpio = gpio;
  }
  void writeLevel(int, bool high) override {
    levels[levelCount++] = high;
  }
  void delayMs(uint32_t ms) override {
    delayValue = ms;
  }
};

void test_active_low_levels_are_correct() {
  ResetPulseConfig config{PI_RESET_GPIO, true, PI_RESET_PULSE_MS};
  TEST_ASSERT_FALSE(resetActiveLevel(config));
  TEST_ASSERT_TRUE(resetInactiveLevel(config));
}

void test_pulse_uses_configured_duration() {
  FakeResetPinDriver driver;
  ResetPulseConfig config{PI_RESET_GPIO, PI_RESET_ACTIVE_LOW, PI_RESET_PULSE_MS};

  pulseResetPin(driver, config);

  TEST_ASSERT_EQUAL_UINT32(PI_RESET_PULSE_MS, driver.delayValue);
  TEST_ASSERT_EQUAL(2, driver.levelCount);
  TEST_ASSERT_EQUAL(resetActiveLevel(config), driver.levels[0]);
  TEST_ASSERT_EQUAL(resetInactiveLevel(config), driver.levels[1]);
}

void test_configure_sets_inactive_level() {
  FakeResetPinDriver driver;
  ResetPulseConfig config{PI_RESET_GPIO, PI_RESET_ACTIVE_LOW, PI_RESET_PULSE_MS};

  configureResetPin(driver, config);

  TEST_ASSERT_EQUAL(PI_RESET_GPIO, driver.modeGpio);
  TEST_ASSERT_EQUAL(1, driver.levelCount);
  TEST_ASSERT_EQUAL(resetInactiveLevel(config), driver.levels[0]);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_active_low_levels_are_correct);
  RUN_TEST(test_pulse_uses_configured_duration);
  RUN_TEST(test_configure_sets_inactive_level);
  return UNITY_END();
}
