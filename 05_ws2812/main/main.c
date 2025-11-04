/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "led_ws2812.h"

#define WS2812_GPIO_NUM     GPIO_NUM_26
#define WS2812_LED_NUM      12

/** HSV转RGB ，暂无用到
 * @param h:色调(0-360) s饱和度(0-100) v亮度(0-100)
 * @param rgb
 * @return 无
*/
void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b)
{
    h %= 360; // h -> [0,360]
    uint32_t rgb_max = v * 2.55f;
    uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

    uint32_t i = h / 60;
    uint32_t diff = h % 60;

    // RGB adjustment amount by hue
    uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

    switch (i) {
    case 0:
        *r = rgb_max;
        *g = rgb_min + rgb_adj;
        *b = rgb_min;
        break;
    case 1:
        *r = rgb_max - rgb_adj;
        *g = rgb_max;
        *b = rgb_min;
        break;
    case 2:
        *r = rgb_min;
        *g = rgb_max;
        *b = rgb_min + rgb_adj;
        break;
    case 3:
        *r = rgb_min;
        *g = rgb_max - rgb_adj;
        *b = rgb_max;
        break;
    case 4:
        *r = rgb_min + rgb_adj;
        *g = rgb_min;
        *b = rgb_max;
        break;
    default:
        *r = rgb_max;
        *g = rgb_min;
        *b = rgb_max - rgb_adj;
        break;
    }
}


// void resursion_led(uint8_t *r,uint8_t *g,uint8_t *b)
// {

    
// }

void app_main(void)
{
    uint8_t flag=0;
    int index = 0;

    //ws2812_strip_handle_t ws2812_handle = NULL;
    //ws2812_init(WS2812_GPIO_NUM,WS2812_LED_NUM,&ws2812_handle);//gpio口，灯珠个数，操作句柄

    ws2812_init(WS2812_GPIO_NUM,WS2812_LED_NUM);//gpio口，灯珠个数，操作句柄

    uint8_t r=0,g=0,b=0;

    while(1)
    {

    /*   
        //红色跑马灯
        for(index = 0;index < WS2812_LED_NUM;index++)
        {
            uint32_t r = 230,g = 20,b = 20;
            ws2812_write(ws2812_handle,index,r,g,b);
            vTaskDelay(pdMS_TO_TICKS(80));
        }
        //绿色跑马灯
        for(index = 0;index < WS2812_LED_NUM;index++)
        {
            uint32_t r = 20,g = 230,b = 20;
            ws2812_write(ws2812_handle,index,r,g,b);
            vTaskDelay(pdMS_TO_TICKS(80));
        }
        //蓝色跑马灯
        for(index = 0;index < WS2812_LED_NUM;index++)
        {
            uint32_t r = 20,g = 20,b = 230;
            ws2812_write(ws2812_handle,index,r,g,b);
            vTaskDelay(pdMS_TO_TICKS(80));
        }
    */ 

    for(index = 0;index < WS2812_LED_NUM;index++)
    {
        if(flag==0)
        {
            if(b>10)
            {
                if(g>10)
                {
                    if(r>10)
                    {
                        flag=1;
    
                    }
                    else
                        r++;  
                }
                else
                    g++;
            }
            else
                b++;

        }
        else
        {
            if(b<=0)
            {
                if(g<=0)
                {
                    if(r<=0)
                    {
                        flag=0;
                    }
                    else
                        r--;  
                }
                else
                    g--;
            }
            else
                b--;
        }

        //ws2812_write(ws2812_handle,index,r,g,b);
        ws2812_write(index,r,g,b);
        vTaskDelay(pdMS_TO_TICKS(80));
    }

    }

}
