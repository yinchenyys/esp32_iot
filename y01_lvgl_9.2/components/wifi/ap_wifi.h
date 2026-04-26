#ifndef _AP_WIfI_H_
#define _AP_WIfI_H_


#include <sys/stat.h>//读取文件大小
#include <string.h>
#include "wifi_manager.h"
#include "esp_spiffs.h"//csv存储
#include "ws_server.h"
#include "cJSON.h"


void ap_wifi_apcfg();
void ap_wifi_init(p_wifi_state_cb f);

#endif // !_AP_WIfI_H_