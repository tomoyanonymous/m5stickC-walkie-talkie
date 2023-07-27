#include "I2SMEMSSampler.h"
#include "soc/i2s_reg.h"
#include <M5StickCPlus.h>
I2SMEMSSampler::I2SMEMSSampler(
    i2s_port_t i2s_port,
    i2s_pin_config_t &i2s_pins,
    i2s_config_t i2s_config,
    int raw_samples_size,
    bool fixSPH0645) : I2SSampler(i2s_port, i2s_config)
{
    m_i2sPins = i2s_pins;
    m_fixSPH0645 = fixSPH0645;
    m_raw_samples_size = raw_samples_size;
    m_raw_samples = (int32_t *)malloc(sizeof(int32_t) * raw_samples_size);
}

I2SMEMSSampler::~I2SMEMSSampler()
{
    free(m_raw_samples);
}

void I2SMEMSSampler::configureI2S()
{
    if (m_fixSPH0645)
    {
        // FIXES for SPH0645
#if CONFIG_IDF_TARGET_ESP32
        REG_SET_BIT(I2S_TIMING_REG(m_i2sPort), BIT(9));
        REG_SET_BIT(I2S_CONF_REG(m_i2sPort), I2S_RX_MSB_SHIFT);
#endif
    }

    auto res = i2s_set_pin(m_i2sPort, &m_i2sPins);
    if (res != ESP_OK)
    {
        Serial.println(esp_err_to_name(res));
    }
    auto res2 = i2s_set_clk(m_i2sPort, sample_rate(), I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
    if (res2 != ESP_OK)
    {
        Serial.println(esp_err_to_name(res));
    }
}

int I2SMEMSSampler::read(int16_t *samples, int count)
{
    // read from i2s
    size_t bytes_read = 0;
    if (count > m_raw_samples_size)
    {
        count = m_raw_samples_size; // Buffer is too small
    }
    // auto ticks_to_wait = (100 / portTICK_RATE_MS); // important!!!
    auto ticks_to_wait = portMAX_DELAY;

    i2s_read(m_i2sPort, m_raw_samples, sizeof(int32_t) * count, &bytes_read, ticks_to_wait);
    int samples_read = bytes_read / sizeof(int32_t);
    for (int i = 0; i < samples_read; i++)
    {
        int32_t temp = m_raw_samples[i] >> 11;
        samples[i] = (temp > INT16_MAX) ? INT16_MAX : (temp < -INT16_MAX) ? -INT16_MAX
                                                                          : (int16_t)temp;
    }
    Serial.println(samples[0]);
    return samples_read;
}
