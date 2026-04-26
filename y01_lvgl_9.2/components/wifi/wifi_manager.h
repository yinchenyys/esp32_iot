

#ifndef _WIFI_MANAGAER_
#define _WIFI_MANAGAER_

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "string.h"
#include "lwip/ip4_addr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"







typedef enum
{
    WIFI_STATE_CONNECTED,
    WIFI_STATE_DISCONNECT
}WIFI_STATE;

typedef void(*p_wifi_state_cb)(WIFI_STATE);
typedef void(*p_wifi_scan_cb)(int num, wifi_ap_record_t *ap_records);


void wifi_manager_init(p_wifi_state_cb f);

void wifi_manager_connect(const char* wifi_ssid,const char* wifi_password);

esp_err_t wifi_manager_scan(p_wifi_scan_cb f);

esp_err_t wifi_manager_ap(void);


/** 是否已经连接了路由器
 * @param 无
 * @return 是/否
*/
bool wifi_manager_is_connect(void);

#endif // !_WIFI()