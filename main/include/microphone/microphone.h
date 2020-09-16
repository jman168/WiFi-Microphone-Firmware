#include <cstdint>
#include <cstring>
#include "imath.h"

#include "driver/i2s.h"

#define DC_OFFSET 1855
#define OVERSAMPLE 2

class Microphone {
    public:
        Microphone(uint16_t buffer_size);
        size_t getSamples(int16_t *data);
        void setSampleRate(uint32_t rate);

    private:
        void dataScale(uint8_t *s_buff, int16_t *d_buff, uint32_t len);

        int32_t working_sample;
        int16_t *input_buff;

        uint16_t input_buffer_size;
        uint16_t output_buffer_size;
        size_t bytes_read;
};