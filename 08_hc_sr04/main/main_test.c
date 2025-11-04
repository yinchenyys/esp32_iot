// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/mcpwm_cap.h"
// #include "esp_private/esp_clk.h"
// #include "esp_log.h"

// #define HC_SR04_TRIG_GPIO  GPIO_NUM_32
// #define HC_SR04_ECHO_GPIO  GPIO_NUM_33

// bool IRAM_ATTR mcpwm_capture_event(mcpwm_cap_channel_handle_t cap_channel, const mcpwm_capture_event_data_t *edata, void *user_ctx)
// {
//     BaseType_t error=0;
//     TaskHandle_t sr04_handle = *((TaskHandle_t *)user_ctx);
//     static uint32_t start_time=0,end_time=0;
//     uint32_t lnterval_time=0;
//     if(edata->cap_edge==MCPWM_CAP_EDGE_POS)
//     {
//         start_time=edata->cap_value;
//         end_time =start_time;
//     }
//     else
//     {
//         end_time=edata->cap_value;
//         lnterval_time=end_time - start_time;
//         xTaskNotifyFromISR(sr04_handle,lnterval_time,eSetValueWithOverwrite,&error);
//     }

//     return  error==1;

// }


// void sr04_start(void)
// {
//     gpio_set_level(HC_SR04_TRIG_GPIO,1);
//     esp_rom_delay_us(10);
//     gpio_set_level(HC_SR04_TRIG_GPIO,0);

// }


// void app_main(void)
// {
//     mcpwm_cap_timer_handle_t sr04_cap_timer;
//     mcpwm_cap_channel_handle_t sr04_cap_channel;

//     mcpwm_capture_timer_config_t config=
//     {
//         .clk_src=MCPWM_CAPTURE_CLK_SRC_DEFAULT,
//         .group_id=0,
//     };
//     mcpwm_new_capture_timer(&config,&sr04_cap_timer);


//     mcpwm_capture_channel_config_t config2 =
//     {
//         .gpio_num=HC_SR04_ECHO_GPIO,
//         .flags.neg_edge=1,
//         .flags.pos_edge=1,
//         .flags.pull_up=1,
//         .prescale=1,
//     };
//     mcpwm_new_capture_channel(sr04_cap_timer,&config2,&sr04_cap_channel);





//     TaskHandle_t sr04_handle = xTaskGetCurrentTaskHandle();
//     mcpwm_capture_event_callbacks_t cbs=
//     {
//         .on_cap=mcpwm_capture_event
//     };
//     mcpwm_capture_channel_register_event_callbacks(sr04_cap_channel,&cbs,&sr04_handle);



//     mcpwm_capture_channel_enable(sr04_cap_channel);
//     mcpwm_capture_timer_enable(sr04_cap_timer);
//     mcpwm_capture_timer_start(sr04_cap_timer);


//     // gpio_set_direction(HC_SR04_TRIG_GPIO,GPIO_MODE_OUTPUT);

//     gpio_set_direction(HC_SR04_TRIG_GPIO,GPIO_MODE_OUTPUT);
//     //￥上面应该是用于配置多个引脚，实际
//     gpio_set_level(HC_SR04_TRIG_GPIO,0);


//     uint32_t sr04_cnt=0;
//     while(1)
//     {
//         sr04_start();
//         if(xTaskNotifyWait(0x00,ULONG_MAX,&sr04_cnt,pdMS_TO_TICKS(2000)))
//         {
//             float sr04_distance =(float) (sr04_cnt*(1000000.0/esp_clk_apb_freq()))/ 58;//如果1000000。0写1000000会不会有影响
//             ESP_LOGI("main","diatance is %.2f",sr04_distance);
//         }
//         else
//              ESP_LOGI("main","can't wait diatance");
//     }
//     vTaskDelay(pdMS_TO_TICKS(500));

// }




///////////////////////////////  第二遍  /////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_private/esp_clk.h"
#include "driver/gpio.h"
#include "driver/mcpwm_cap.h"//mcpwm->mcpwm_cap
#include "esp_log.h"

#define HC_SR04_TRIG_GPIO  GPIO_NUM_32
#define HC_SR04_ECHO_GPIO  GPIO_NUM_33
mcpwm_cap_timer_handle_t sr04_ret_cap_timer;
mcpwm_cap_channel_handle_t sr04_ret_cap_channel;

bool IRAM_ATTR sr04_mcpwm_capture_event(mcpwm_cap_channel_handle_t cap_channel, const mcpwm_capture_event_data_t *edata, void *user_ctx)
{
    BaseType_t error =0 ;
    TaskHandle_t sr04_handle =*((TaskHandle_t*)user_ctx);
    static uint32_t start_time=0,end_time=0,interal_time=0;
    if(edata->cap_edge==MCPWM_CAP_EDGE_POS)
    {
        start_time = edata->cap_value;
        end_time = start_time;
    }
    else
    {
        end_time=edata->cap_value;
        interal_time=end_time-start_time;
        xTaskNotifyFromISR(sr04_handle,interal_time,eSetValueWithOverwrite,&error);
    }

    return error==1;
}



void sr04_start(void)
{
    gpio_set_level(HC_SR04_TRIG_GPIO,1);
    esp_rom_delay_us(10);
    gpio_set_level(HC_SR04_TRIG_GPIO,0);
}

void app_main(void)
{
    mcpwm_capture_timer_config_t config1 =
    {
        .clk_src=MCPWM_CAPTURE_CLK_SRC_DEFAULT,
        .group_id=0,
    };
    mcpwm_new_capture_timer(&config1,&sr04_ret_cap_timer);

    mcpwm_capture_channel_config_t config2 =
    {
        .flags.pull_up=1,
        .flags.neg_edge=1,
        .flags.pull_down=1,
        .gpio_num=HC_SR04_ECHO_GPIO,
        .prescale=1,
    };
    mcpwm_new_capture_channel(sr04_ret_cap_timer,&config2,&sr04_ret_cap_channel);


    TaskHandle_t sr04_handle;
    sr04_handle = xTaskGetCurrentTaskHandle();
    mcpwm_capture_event_callbacks_t cbs = 
    {
        .on_cap =sr04_mcpwm_capture_event
    };
    mcpwm_capture_channel_register_event_callbacks(sr04_ret_cap_channel,&cbs,&sr04_handle);

    
    mcpwm_capture_timer_enable(sr04_ret_cap_timer);
    mcpwm_capture_channel_enable(sr04_ret_cap_channel);
    mcpwm_capture_timer_start(sr04_ret_cap_timer);

    gpio_set_direction(HC_SR04_TRIG_GPIO,GPIO_MODE_OUTPUT);
    gpio_set_level(HC_SR04_TRIG_GPIO,0);

    uint32_t sr04_cnt=0;
    while(1)
    {
        sr04_start();
        if(xTaskNotifyWait(0x00,ULONG_MAX,&sr04_cnt,pdMS_TO_TICKS(2000)))
        {
            float sr04_distance =(float) (sr04_cnt*(1000000.0/esp_clk_apb_freq()))/ 58;//如果1000000。0写1000000会不会有影响
            ESP_LOGI("main","diatance is %.2f",sr04_distance);
        }
        else
             ESP_LOGI("main","can't wait diatance");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}