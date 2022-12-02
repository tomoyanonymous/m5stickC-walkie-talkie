#include <M5Atom.h>
#include "M5AtomIndicatorLed.h"

M5AtomIndicatorLed::M5AtomIndicatorLed()
{
//  pinMode(BUILT_IN_LED, OUTPUT);
}

void M5AtomIndicatorLed::set_led_rgb(uint32_t color)
{
  M5.dis.drawpix(0, color);
}
