#ifndef TCP_H
#define TCP_H

#include <inttypes.h>
#include <cstring>

#include "lwip/sockets.h"
#include "esp_system.h"

#include "ControlPackets.h"

void send_wakeup(uint16_t frame_size, uint32_t sample_rate);

#endif