#include <cstdint>
#include <cstring>

#include "driver/i2s.h"

#define DC_OFFSET 1855

class Microphone {
    public:
        Microphone(int sampleRate, uint16_t bufferSize);
        size_t getSamples(int16_t *data);

    private:
        char *raw;
        uint16_t bufferSize;
        size_t bytes_read;
};