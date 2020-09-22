#include <cstdint>
#include <cstring>

#include "driver/i2s.h"

#define DC_OFFSET 1855
#define OVERSAMPLE 2

class Microphone {
    public:
        Microphone(uint16_t buffer_size);
        size_t getSamples(int16_t *data);
        void setSampleRate(uint32_t rate);

    private:
        int32_t working_sample;
        int16_t *input_buff;

        uint16_t buffer_size;
        size_t bytes_read;
};