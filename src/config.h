#include <freertos/FreeRTOS.h>
#include <driver/i2s.h>
#include <driver/gpio.h>

// #define MIC_ONLY_MODE
#define SPEAKER_ONLY_MODE

// WiFi credentials
// #define WIFI_SSID << YOUR_SSID >>
// #define WIFI_PSWD << YOUR_PASSWORD >>

// sample rate for the system
#define SAMPLE_RATE 16000

// are you using an I2S microphone - comment this if you want to use an analog mic and ADC input
#define USE_I2S_MIC_INPUT

// I2S Microphone Settings
// When using m5stickC's built in i2s microphone, PDM mode can be used only for I2S_NUM_0
#define I2S_NUM_MIC I2S_NUM_0
#define I2S_NUM_SPEAKER I2S_NUM_0

// Which channel is the I2S microphone on? I2S_CHANNEL_FMT_ONLY_LEFT or I2S_CHANNEL_FMT_ONLY_RIGHT
// Generally they will default to LEFT - but you may need to attach the L/R pin to GND
// #define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_RIGHT
#define I2S_MIC_SERIAL_CLOCK I2S_PIN_NO_CHANGE //SCK - BCLK
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_0 //WS - LRC
#define I2S_MIC_SERIAL_DATA GPIO_NUM_34 //SD

// Analog Microphone Settings - ADC1_CHANNEL_7 is GPIO35
#define ADC_MIC_CHANNEL ADC1_CHANNEL_7


// speaker settings for SPEAKER 2 HAT
#define USE_I2S_SPEAKER_OUTPUT
#define I2S_SPEAKER_SERIAL_CLOCK GPIO_NUM_26 //BCLK
#define I2S_SPEAKER_LEFT_RIGHT_CLOCK GPIO_NUM_0 //LRC
// you need to set gpio 36 to floating, which uses a common pin to gpio25
//    gpio_pulldown_dis(GPIO_NUM_36);
//    gpio_pullup_dis(GPIO_NUM_36);
#define I2S_SPEAKER_SERIAL_DATA GPIO_NUM_25 //DIN
// Shutdown line if you have this wired up or -1 if you don't
#define I2S_SPEAKER_SD_PIN -1 //SD


#define DAC_SPEAKER_PIN GPIO_NUM_26;

// transmit button
#define GPIO_TRANSMIT_BUTTON 39

// Which transport do you want to use? ESP_NOW or UDP?
// comment out this line to use UDP
#define USE_ESP_NOW

// On which wifi channel (1-11) should ESP-Now transmit? The default ESP-Now channel on ESP32 is channel 1
#define ESP_NOW_WIFI_CHANNEL 1

// In case all transport packets need a header (to avoid interference with other applications or walkie talkie sets), 
// specify TRANSPORT_HEADER_SIZE (the length in bytes of the header) in the next line, and define the transport header in config.cpp
#define TRANSPORT_HEADER_SIZE 0
extern uint8_t transport_header[TRANSPORT_HEADER_SIZE];


// i2s config for using the internal ADC
extern i2s_config_t i2s_adc_config;
// i2s config for reading from of I2S
extern i2s_config_t i2s_mic_Config;
// i2s microphone pins
extern i2s_pin_config_t i2s_mic_pins;
// i2s speaker pins
extern i2s_pin_config_t i2s_speaker_pins;
