

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "button.h"
#include "esp_log.h"



#define SHORT_EV    BIT0    //短按
#define LONG_EV     BIT1    //长按
#define BTN_GPIO    GPIO_NUM_39
#define LED_GPIO    GPIO_NUM_27



// 单独设置GPIO控制灯亮
void short_press_handle(void)
{
    // gpio_set_level(LED_GPIO,gpio_get_level(LED_GPIO)?0:1);
    if(gpio_get_level(LED_GPIO)==0)
        gpio_set_level(LED_GPIO,1);
    else
        gpio_set_level(LED_GPIO,0);
    ESP_LOGI("LED","led level is %d",gpio_get_level(LED_GPIO));
}


void long_press_handle(void)
{
    ESP_LOGI("LED","get long press");
}

void app_main(void)
{
    //加入按键配置
    button_config_t btn_cfg = 
    {
        .gpio_num = BTN_GPIO,       //gpio号
        .active_level = 0,          //按下的电平
        .long_press_time = 3000,    //长按时间3000ms
        .short_cb = short_press_handle,           //短按回调函数
        .long_cb = long_press_handle ,            //长按回调函数
        
    };
    button_event_set(&btn_cfg);     //添加按键响应事件处理


    gpio_config_t led_gpio_t = 
    {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ull<<LED_GPIO),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&led_gpio_t));

     gpio_set_level(LED_GPIO,0);
}
