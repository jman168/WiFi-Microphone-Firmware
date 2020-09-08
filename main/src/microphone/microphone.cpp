#include "microphone/microphone.h"

Microphone::Microphone(int sampleRate, uint16_t bufferSize) : bufferSize(bufferSize) {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
        .sample_rate =  sampleRate,
        .bits_per_sample = (i2s_bits_per_sample_t)(16),
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_MSB,
        .intr_alloc_flags = 0,
        .dma_buf_count = 2,
        .dma_buf_len = bufferSize,
        .use_apll = 1,
        .fixed_mclk = sampleRate,
    };
    
    //install and start i2s driver
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    //init ADC pad
    i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_0);

    raw = new char[bufferSize*2];
}

size_t Microphone::getSamples(int16_t *data) {
    i2s_read(I2S_NUM_0, raw, bufferSize*2, &bytes_read, portMAX_DELAY);
    for(int i = 0; i < bufferSize; i++) {
        data[i] = (int16_t)(((uint16_t)(raw[i*2 + 1] & 0xF) << 8) | raw[i*2 + 0]) - (int16_t)DC_OFFSET;
    }
    return bytes_read;
}