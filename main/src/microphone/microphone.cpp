#include "microphone/microphone.h"

Microphone::Microphone(uint16_t buffer_size) {
    output_buffer_size = buffer_size;
    input_buffer_size = buffer_size*OVERSAMPLE;
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),    //master, RX, PDM
        .sample_rate = 48000,
        .bits_per_sample = (i2s_bits_per_sample_t)16,                                    
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_STAND_MSB, //pcm data format
        .intr_alloc_flags = 1,     //interrupt level 1(lowest priority)                  
        .dma_buf_count = 2,                   
        .dma_buf_len = input_buffer_size,
        .use_apll = true,
        .fixed_mclk = 0
    };

    i2s_pin_config_t pin_config = {
        .ws_io_num = GPIO_NUM_4,
        .data_in_num = GPIO_NUM_2,
    };

    input_buff = new int16_t[input_buffer_size];
    
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
    i2s_read(I2S_NUM_0, input_buff, input_buffer_size*2, &bytes_read, portMAX_DELAY);
    
    for(int i = 0; i < output_buffer_size; i++) {
        working_sample = 0;
        for(int x = 0; x < OVERSAMPLE; x++) {
            working_sample += input_buff[i * OVERSAMPLE + x];
        }
        working_sample /= OVERSAMPLE;
        data[i] = working_sample;
    }
    return bytes_read;
}

void Microphone::dataScale(uint8_t *s_buff, int16_t *d_buff, uint32_t len) {
    for (int i = 0; i < len*2; i += 2)
    {
        d_buff[i/2] = (s_buff[i + 1] << 8) | s_buff[i]; // signed value
        d_buff[i/2] = d_buff[i/2] & 0xffff;
    }
}