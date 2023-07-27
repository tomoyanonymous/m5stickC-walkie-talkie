#include <M5StickCPlus.h>
// #include <M5StickC.h>
#include "Application.h"
#include "config.h"
// our application
Application *application;

void setup()
{

  M5.begin();
  M5.Axp.begin();
 
  M5.Lcd.begin();
  M5.Lcd.setRotation(1);
  delay(50);
  
  // Serial.begin(115200);
  Serial.println("M5 started");

  // start up the application
  application = new Application();
  application->begin();
  Serial.println("Application started");
}
float getBatteryLevel(void)
{
  float vbat= M5.Axp.GetBatVoltage();
  return 100.0 * ((vbat - 3.0) / (4.07 - 3.0));
}
void loop()
{
M5.Lcd.fillScreen(TFT_WHITE);
  M5.Lcd.setTextFont(4);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(TFT_BLACK);
#ifdef SPEAKER_ONLY_MODE
  M5.Lcd.println("speaker_mode");
#else
  M5.Lcd.println("mic_mode");
#endif
M5.Lcd.printf("Battery: %.2f%%\n", getBatteryLevel());
  // nothing to do - the application is doing all the work
  vTaskDelay(pdMS_TO_TICKS(1000));
}
