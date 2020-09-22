#include "udp/udp.h"

UDPSocket::UDPSocket(const char *addr, uint16_t port) {
    dest_addr.sin_addr.s_addr = inet_addr(addr);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    connect(sock, (sockaddr *)&dest_addr, sizeof(dest_addr));

}

void UDPSocket::sendPacket(const void *data, size_t length) {
    send(sock, data, length, 0);
}