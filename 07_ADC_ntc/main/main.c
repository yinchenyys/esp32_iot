/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ntc.h"    

#define TAG     "main"

#define test 

void app_main(void)
{
    #ifdef test
        NTC_Init();
    #else
         temp_ntc_init();
    #endif // DEBUG
   
    while(1)
    {
        #ifdef test
            ESP_LOGI(TAG,"current temp:%.2f",NTC_read());
        #else
            ESP_LOGI(TAG,"current temp:%.2f",get_temp());//初始化时已经创建一个任务不断读取并保存到参数，通过函数读取
        #endif // DEBUG
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
