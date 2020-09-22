#include <inttypes.h>

#define PACKET_HEADER_SIZE 5

struct UDPPacket_t {
    uint32_t packet_number;
    uint8_t data_length;
    unsigned char data[40];
};

union UDPPacket
{
    UDPPacket_t packet;
    unsigned char data[sizeof(UDPPacket_t)];
};
