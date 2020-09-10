/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>

#include "sdkconfig.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "wifi/wifi.h"
#include "udp/udp.h"
#include "opus/opus.h"
#include "microphone/microphone.h"
#include "imath.h"

extern "C" {
    void app_main();
}

struct UDPPacket_t {
  uint32_t packet_number;
  int16_t samples[256];
};

union UDPPacket {
  UDPPacket_t packet;
  char data[256*2 + 4];
};
 

uint64_t startTime;

void UDPTask(void *) {
  Microphone mic(48000, 256);
  UDPSocket sock(CONFIG_WIFI_MICROPHONE_IP_ADDRESS, CONFIG_WIFI_MICROPHONE_PORT);

  UDPPacket packet = {};

  size_t bytesRead;
  uint32_t packet_number = 0;

  isin(0);

  while(1) {
    bytesRead = mic.getSamples(packet.packet.samples);
    packet.packet.packet_number = packet_number;
    sock.sendPacket(packet.data, sizeof(UDPPacket_t));
    packet_number++;
  }
}


void app_main()
{
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // ESP_LOGI(WIFI_TAG, "ESP_WIFI_MODE_STA");
  wifi_init_sta();

  // xTaskCreatePinnedToCore(UDPTask, "UDP", 32768, NULL, 1, NULL, 0);
  UDPTask(NULL);
}
