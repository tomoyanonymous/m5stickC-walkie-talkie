// #include <M5Atom.h>
#include <M5StickCPlus.h>
// #include <M5StickC.h>

#include <driver/i2s.h>

#include <WiFi.h>

#include "Application.h"
#include "I2SMEMSSampler.h"
#include "ADCSampler.h"
#include "I2SOutput.h"
#include "DACOutput.h"
#include "UdpTransport.h"
#include "EspNowTransport.h"
#include "OutputBuffer.h"
#include "config.h"

#include "M5StickCIndicatorLed.h"

static void application_task(void *param)
{
  // delegate onto the application
  Application *application = reinterpret_cast<Application *>(param);
  application->loop();
}

Application::Application()
{
  btn_last = false;
  toggle = false;
  m_output_buffer = new OutputBuffer(300 * 16);
  pinMode(10, OUTPUT);

#ifndef SPEAKER_ONLY_MODE
  m_input = new I2SMEMSSampler(I2S_NUM_MIC, i2s_mic_pins, i2s_mic_Config, 128);
#endif

#ifndef MIC_ONLY_MODE
#ifdef USE_I2S_SPEAKER_OUTPUT
  m_output = new I2SOutput(I2S_NUM_SPEAKER, i2s_speaker_pins);
#else
  m_output = new DACOutput(I2S_NUM_SPEAKER);
#endif
#endif

#ifdef USE_ESP_NOW
  m_transport = new EspNowTransport(m_output_buffer, ESP_NOW_WIFI_CHANNEL);
#else
  m_transport = new UdpTransport(m_output_buffer);
#endif

  m_transport->set_header(TRANSPORT_HEADER_SIZE, transport_header);

  // m_indicator_led = new M5StickCIndicatorLed();

  if constexpr (I2S_SPEAKER_SD_PIN != -1)
  {
    pinMode(I2S_SPEAKER_SD_PIN, OUTPUT);
  }
}

void Application::begin()
{
  // show a flashing indicator that we are trying to connect
  // m_indicator_led->set_default_color(0);
  // m_indicator_led->set_is_flashing(true, 0xff0000);
  // m_indicator_led->begin();

  Serial.print("My IDF Version is: ");
  Serial.println(esp_get_idf_version());

  // bring up WiFi
  WiFi.mode(WIFI_STA);
#ifndef USE_ESP_NOW
  WiFi.begin(WIFI_SSID, WIFI_PSWD);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  // this has a dramatic effect on packet RTT
  WiFi.setSleep(WIFI_PS_NONE);
  Serial.print("My IP Address is: ");
  Serial.println(WiFi.localIP());
#else
  // but don't connect if we're using ESP NOW
  WiFi.disconnect();
#endif
  Serial.print("My MAC Address is: ");
  Serial.println(WiFi.macAddress());
  digitalWrite(M5_LED, HIGH);
  // do any setup of the transport
  m_transport->begin();
  // connected so show a solid green light
  // m_indicator_led->set_default_color(0x00ff00);
  // m_indicator_led->set_is_flashing(false, 0x00ff00);
// start off with i2S output running
#ifdef MIC_ONLY_MODE
  // start the input to get samples from the microphone
  m_input->start();
#endif
#ifndef MIC_ONLY_MODE
  m_output->start(SAMPLE_RATE);
#endif
  // start the main task for the application
  TaskHandle_t task_handle;
  xTaskCreate(application_task, "application_task", 8192, this, 1, &task_handle);
}

// application task - coordinates everything
void Application::loop()
{

  int16_t *samples = reinterpret_cast<int16_t *>(malloc(sizeof(int16_t) * 128));
  
  // continue forever
  while (true)
  {
    bool btn_now = M5.BtnA.read() == 1;
    if (btn_now && !btn_last)
    {
      toggle = !toggle;
    }
    digitalWrite(M5_LED, HIGH);

    if (toggle)
    {
#ifdef MIC_ONLY_MODE

      digitalWrite(M5_LED, LOW);
      // Serial.println("Started transmitting");
      // m_indicator_led->set_is_flashing(true, 0xff0000);

      // transmit for at least 1 second or while the button is pushed

      // read samples from the microphone
      int samples_read = m_input->read(samples, 128);

      // and send them over the transport
      for (int i = 0; i < samples_read; i++)
      {
        m_transport->add_sample(samples[i]);
      }
      m_transport->flush();
      // finished transmitting stop the input and start the output
      // Serial.println("Finished transmitting");
      // m_indicator_led->set_is_flashing(false, 0xff0000);
      // m_input->stop();

#endif

#ifndef MIC_ONLY_MODE
      // while the transmit button is not pushed and 1 second has not elapsed

      // Serial.println("Started Receiving");
      digitalWrite(M5_LED, HIGH);

      // read from the output buffer (which should be getting filled by the transport)
      m_output_buffer->remove_samples(samples, 128);
      // and send the samples to the speaker
      m_output->write(samples, 128);

      // Serial.println("Finished Receiving");
      // stop the output as we're switching into transmit mode
      // m_output->stop();
#endif
    }
    else
    {
      vTaskDelay(50);
    }
    btn_last = btn_now;
  }
}