#pragma once

#include "IndicatorLed.h"

class M5AtomIndicatorLed : public IndicatorLed
{
protected:
  void set_led_rgb(uint32_t color);

public:
  M5AtomIndicatorLed();
};