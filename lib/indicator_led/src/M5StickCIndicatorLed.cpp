#include <M5StickCPlus.h>
// #include <M5StickC.h>
#include "M5StickCIndicatorLed.h"

M5StickCIndicatorLed::M5StickCIndicatorLed()
{
  // pinMode(M5_LED, OUTPUT);
  M5.Lcd.setRotation(0);
}

void M5StickCIndicatorLed::set_led_rgb(uint32_t color)
{

  uint8_t R8 = (color & 0xFF0000) >> 16;
  uint8_t G8 = (color & 0x00FF00) >> 8;
  uint8_t B8 = (color & 0x0000FF);
  // uint32_t brightness = (R || G || B);
  // //M5StickC's Built-in LED turns on when the pin LOW
  // digitalWrite(M5_LED, brightness > 128 ? LOW : HIGH);
  // Serial.print("setLED ");
  // Serial.print(R8);
  // Serial.print(G8);
  // Serial.println(B8);
  // M5.Lcd.fillRect(0,30,30,30,M5.Lcd.color565(R8,G8,B8));
  // M5.Lcd.fillScreen(M5.Lcd.color565(R8,G8,B8));
  
}
