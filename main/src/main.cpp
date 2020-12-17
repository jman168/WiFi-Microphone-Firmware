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
#include "udp/packet.h"
#include "udp/udp.h"
#include "opus/opus.h"
#include "microphone/microphone.h"
#include "TCP/TCP.h"

#define SEARCH_INTERATIONS 375
#define TARGET_MICROSECONDS 6000000

#define FRAME_SIZE 240
#define FRAME_TIME 5000 // frame time in us
#define FRAME_TIME_ALLOWED_VARIATION 1000 // the maximum time in us over the frame time that is allowed

// #define BENCHMARK_LOOP_TIME

extern "C" {
    void app_main();
}

int16_t samples[FRAME_SIZE];

uint64_t startTime;

OpusEncoder *encoder;
int error;

void UDPTask(void *) {
  Microphone mic(FRAME_SIZE);
  UDPSocket sock(CONFIG_WIFI_MICROPHONE_IP_ADDRESS, CONFIG_WIFI_MICROPHONE_PORT);

  UDPPacket packet = {};

  size_t bytesRead;
  uint32_t packet_number = 0;
  int64_t last = 9999999999;
  int64_t current = 0;

  int64_t loop_start, loop_end;
  
  TaskHandle_t warning_task;

#ifdef BENCHMARK_LOOP_TIME
  loop_start = esp_timer_get_time();
  while(packet_number<1000) {
#else
  while(1) {
#endif

    bytesRead = mic.getSamples(samples); // Read the samples
    
    packet.packet.data_length = opus_encode(encoder, samples, FRAME_SIZE, packet.packet.data, 40); // Encode the data and set the data size of the packet
    packet.packet.packet_number = packet_number; // Set the packet number for packet loss detection on the other side

    sock.sendPacket(packet.data, packet.packet.data_length+PACKET_HEADER_SIZE); // Send over the packet, but only the minimum required data

    packet_number++; // Increment the packet number
  }

  loop_end = esp_timer_get_time();
  ESP_LOGI("AUDIO STREAM", "Main loop takes and average of %" PRId64 "us.", (loop_end - loop_start)/1000);
  
  while(1) {
    vTaskDelay(1000);
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

  wifi_init_sta();
  
  encoder = opus_encoder_create(48000, 1, OPUS_APPLICATION_AUDIO, &error);
  if(error < 0)
    ESP_LOGE("OPUS", "Error creating opus encoder.");

  error = opus_encoder_ctl(encoder, OPUS_SET_COMPLEXITY(5));
  if(error < 0)
    ESP_LOGE("OPUS", "Error setting opus encoder complexity.");

  error = opus_encoder_ctl(encoder, OPUS_SET_BITRATE(32000));
  if(error < 0)
    ESP_LOGE("OPUS", "Error setting opus encoder bitrate.");

  send_wakeup(FRAME_SIZE, 48000);

  xTaskCreatePinnedToCore(UDPTask, "UDP", 65536, NULL, 1, NULL, 0);
  // UDPTask(NULL);
}
