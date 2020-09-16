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
#include "esp_timer.h"

#include "wifi/wifi.h"
#include "udp/udp.h"
#include "opus/opus.h"
#include "microphone/microphone.h"
#include "imath.h"

#define SEARCH_INTERATIONS 375
#define TARGET_MICROSECONDS 6000000

extern "C" {
    void app_main();
}

struct UDPPacket_t {
  uint32_t packet_number;
  int16_t samples[128];
};

union UDPPacket {
  UDPPacket_t packet;
  char data[128*2 + 4];
};
 

uint64_t startTime;

void UDPTask(void *) {
  Microphone mic(128);
  UDPSocket sock(CONFIG_WIFI_MICROPHONE_IP_ADDRESS, CONFIG_WIFI_MICROPHONE_PORT);

  UDPPacket packet = {};

  size_t bytesRead;
  uint32_t packet_number = 0;

  while(1) {
    bytesRead = mic.getSamples(packet.packet.samples);
    packet.packet.packet_number = packet_number;
    sock.sendPacket(packet.data, sizeof(UDPPacket_t));
    packet_number++;
  }
}

// double high_rate = 49500;
// double low_rate = 46500;

// void UDPTask(void *) {
//   Microphone mic(48000, 256);
//   size_t bytesRead;
//   UDPPacket packet = {};

//   int64_t start = esp_timer_get_time();
//   for(int i = 0; i < SEARCH_INTERATIONS; i++) {
//     bytesRead = mic.getSamples(packet.packet.samples);
//     printf("Read sample %i!\n", i);
//   }
//   int64_t end = esp_timer_get_time();
//   printf("Achieved frequency: %f\n", (double)(SEARCH_INTERATIONS*256) / ((double)(end-start) / 1000000.0));

//   // while(true) {
//   //   double frequency = (high_rate + low_rate) / 2.0;
//   //   mic.setSampleRate(frequency);
//   //   int64_t start = esp_timer_get_time();
//   //   for(int i = 0; i < SEARCH_INTERATIONS; i++) {
//   //     bytesRead = mic.getSamples(packet.packet.samples);
//   //   }
//   //   int64_t end = esp_timer_get_time();
//   //   if(end - start > TARGET_MICROSECONDS) { // Too slow
//   //     low_rate = frequency;
//   //   } else { // Too fast
//   //     high_rate = frequency;
//   //   }
//   //   printf("Achieved frequency: %f, set frequency: %f\n", (double)(SEARCH_INTERATIONS*256) / ((double)(end-start) / 1000000.0), frequency);
//   // }
// }


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
