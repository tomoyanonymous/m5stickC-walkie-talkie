#pragma once

#include "IndicatorLed.h"

class M5StickCIndicatorLed : public IndicatorLed
{
protected:
  void set_led_rgb(uint32_t color);

public:
  M5StickCIndicatorLed();
};