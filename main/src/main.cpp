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

extern "C" {
    void app_main();
}

union UDPPacket {
  int16_t samples[256];
  char data[256*2];
};
 

uint64_t startTime;

void UDPTask(void *) {
  Microphone mic(48000, 256);
  UDPSocket sock(CONFIG_WIFI_MICROPHONE_IP_ADDRESS, CONFIG_WIFI_MICROPHONE_PORT);

  UDPPacket packet = {};

  size_t bytesRead;
  int32_t sum;

  i2s_adc_enable(I2S_NUM_0);
  startTime = esp_timer_get_time();
  for(int iteration = 0; iteration < 1875; iteration++) {
    bytesRead = mic.getSamples(packet.samples);
    if(bytesRead != 256*2)
      ESP_LOGW("ADC", "Did not get full buffer!");
    else
      sock.sendPacket(packet.data, 256*2);
  }
  printf("Average frame encode time: %" PRIu64 "us\n", (esp_timer_get_time()-startTime)/1875);

  while(1) {}
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
