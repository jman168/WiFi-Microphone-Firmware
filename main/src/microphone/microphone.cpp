#include "microphone/microphone.h"

Microphone::Microphone(uint16_t buffer_size) : buffer_size(buffer_size) {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),    //master, RX, PDM
        .sample_rate = 48000,
        .bits_per_sample = (i2s_bits_per_sample_t)16,                                    
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_STAND_MSB, //pcm data format
        .intr_alloc_flags = 1,     //interrupt level 1(lowest priority)                  
        .dma_buf_count = 2,                   
        .dma_buf_len = buffer_size,
        .use_apll = true,
        .fixed_mclk = 0
    };

    i2s_pin_config_t pin_config = {
        .ws_io_num = GPIO_NUM_4,
        .data_in_num = GPIO_NUM_2,
    };
    
    //install and start i2s driver
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    
    //set the i2s pin config
    i2s_set_pin(I2S_NUM_0, &pin_config);

    i2s_set_pdm_rx_down_sample(I2S_NUM_0, I2S_PDM_DSR_8S);

    i2s_hal_context_t hal;
    i2s_hal_init(&hal, I2S_NUM_0);

    rtc_clk_apll_enable(1, 149, 212, 5, 2);
    i2s_hal_set_clk_div(&hal, 1, 1, 0, 8, 8);
    i2s_hal_set_clock_sel(&hal, I2S_CLK_APLL);
}

size_t Microphone::getSamples(int16_t *data) {
    i2s_read(I2S_NUM_0, data, buffer_size*2, &bytes_read, portMAX_DELAY);
    return bytes_read;
}