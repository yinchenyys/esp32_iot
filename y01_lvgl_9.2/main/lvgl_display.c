#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "lv_port.h"
#include "lv_demos.h"
#include "st7789_driver.h"
#include "ui_led.h"
#include "driver/gpio.h"
#include "ui_home.h"


#include "gui_guider.h"
#include "custom.h"


#include "esp_log.h"
#include "nvs_flash.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
#include "ap_wifi.h"
#include "button.h"
#include "esp_sntp.h"
#include "weather.h"



#define TAG "main"

lv_ui guider_ui ;


//初始化时间校验
 void sntp_finish_callback (struct timeval *tv)
 {
     struct tm t;
    localtime_r(&tv->tv_sec,&t);
    set_home_time(&guider_ui,t.tm_year +1900,t.tm_mon + 1,t.tm_mday,t.tm_wday,t.tm_hour,t.tm_min,t.tm_sec);
}
 


void my_sntp_init(void)
{
    if(!esp_sntp_enabled())
    {
        ESP_LOGI(TAG,"sntp_init");
        esp_sntp_setoperatingmode(ESP_SNTP_OPMODE_POLL);
        esp_sntp_setservername(0,"ntp.aliyun.com");
        esp_sntp_setservername(1,"time.asia.apple.com");
        esp_sntp_setservername(2,"pool.ntp.org");
        esp_sntp_set_time_sync_notification_cb(sntp_finish_callback);
        esp_sntp_init();
        
    }
}



//wifi回调函数
void wifi_state_callack(WIFI_STATE state)
{
    if(state == WIFI_STATE_CONNECTED)
    {
        ESP_LOGI(TAG, "Wifi connected");
        my_sntp_init();
    }
    else if(state == WIFI_STATE_DISCONNECT)
    {
        ESP_LOGI(TAG, "Wifi disconnect");
    }
}



//按键回调函数
void short_press_handle(void)
{

}


void long_press_handle(void)
{
    ap_wifi_apcfg();
    ESP_LOGI("LED","get long press");
}





void app_main(void)
{
    //屏幕及lvgl初始化
    init_weaher_img_buffer();
    lv_port_init();

    //网络初始化
    nvs_flash_init();
    ap_wifi_init(wifi_state_callack);//写一个函数会传给函数调用
    setenv("TZ","CST-8",1);//改变虚拟环境,表明我们在东8区,1表示覆盖
    tzset();//启动修改
    // //加入按键配置
    button_config_t btn_cfg = 
    {
        .gpio_num = BTN_GPIO,       //gpio号
        .active_level = 0,          //按下的电平
        .long_press_time = 3000,    //长按时间
        .short_cb = short_press_handle,           //短按回调函数
        .long_cb = long_press_handle             //长按回调函数
    };
    button_event_set(&btn_cfg);     //添加按键响应事件处理

    lvgl_port_lock(0);//上锁，如果没0这个锁，防止没绘制完就跑起来
    // ui_home_create();
    setup_ui(&guider_ui);
    custom_init(&guider_ui);//自己修改内容
    lvgl_port_unlock();
    weather_start();
    
//     while(1)
//     {
//         // lv_task_handler();
//         time_t now = time(NULL);
//         ESP_LOGI(TAG,"now time : %lld", now);
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
}
