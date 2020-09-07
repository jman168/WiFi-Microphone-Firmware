/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "sdkconfig.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "wifi/wifi.h"
#include "udp/udp.h"

#ifdef CONFIG_IDF_TARGET_ESP32
#define CHIP_NAME "ESP32"
#endif

#ifdef CONFIG_IDF_TARGET_ESP32S2BETA
#define CHIP_NAME "ESP32-S2 Beta"
#endif

extern "C" {
    void app_main();
}

double x = 0;
double xInc = (M_PI*2.0) / 22050.0 * 440.0;
uint32_t packet_count = 0;

union packet {
  int16_t samples[256];
  char data[512];
};

void UDPTask(void *) {
  UDPSocket s(CONFIG_WIFI_MICROPHONE_IP_ADDRESS, CONFIG_WIFI_MICROPHONE_PORT);

  packet p;

  while(1) {
    vTaskDelay(5 / portTICK_PERIOD_MS);
    for(int i = 0; i < 256; i++) {
      p.samples[i] = sin(x) * 32767.0;
      x+=xInc;
    }
    s.sendPacket(p.data, strlen(p.data));
    packet_count++;
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

    ESP_LOGI(WIFI_TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();

    xTaskCreatePinnedToCore(UDPTask, "UDP", 4096, NULL, 1, NULL, 0);
}
