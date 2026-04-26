#ifndef _LV_PORT_H_
#define _LV_PORT_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "esp_log.h"
#include "st7789_driver.h"
#include "cst816t_driver.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_lvgl_port.h"



void lv_port_init(void);


#endif