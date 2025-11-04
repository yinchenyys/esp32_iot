#ifndef _WIFI_MANAGER_H_
#define _WIFI_MANAGER_H_
#include "esp_err.h"

//WIFI STA初始化
esp_err_t wifi_sta_init(void);

// typedef uint8_t wifi_cb(void);
// typedef void (*wifi_event_cb)(ip_event_t);////￥定义了一个名为wifi_event_cb的函数指针,

#endif
