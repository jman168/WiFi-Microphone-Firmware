#include <cstdint>
#include "esp_event_base.h"

#define WIFI_SSID      CONFIG_WIFI_MICROPHONE_WIFI_SSID
#define WIFI_PASS      CONFIG_WIFI_MICROPHONE_WIFI_PASSWORD
#define WIFI_MAXIMUM_RETRY  CONFIG_WIFI_MICROPHONE_MAXIMUM_RETRY

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *WIFI_TAG = "wifi station";

void wifi_init_sta();
