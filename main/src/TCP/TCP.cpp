#include "TCP/TCP.h"

void send_wakeup(uint16_t frame_size, uint32_t sample_rate) {
    sockaddr_in addr; 
    init_packet_t packet;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(CONFIG_WIFI_MICROPHONE_IP_ADDRESS);
    addr.sin_port = htons(3090); 

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    
    esp_efuse_mac_get_default(packet.mac);
    packet.frame_size = frame_size;
    packet.sample_rate = sample_rate;

    char buff[sizeof(init_packet_t)];
    memcpy(buff, &packet, sizeof(init_packet_t));

    send(sock, buff, sizeof(init_packet_t), 0);
}