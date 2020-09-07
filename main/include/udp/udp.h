#include "stdint.h"

#include "lwip/sockets.h"

class UDPSocket {
    public:
        UDPSocket(const char *addr, uint16_t port);

        void sendPacket(const void *data, size_t length);

    private:
        sockaddr_in dest_addr;
        int sock;
};