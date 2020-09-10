#include <cstdint>
#include <cstring>
#include "imath.h"

#include "driver/i2s.h"

#define DC_OFFSET 1855

class Microphone {
    public:
        Microphone(int sampleRate, uint16_t bufferSize);
        size_t getSamples(int16_t *data);

    private:
        int16_t x = 0.0;
        int16_t xInc = 601;
        
        char *raw;
        uint16_t bufferSize;
        size_t bytes_read;
};