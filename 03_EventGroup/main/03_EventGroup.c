// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/event_groups.h"
// #include "freertos/task.h"//该文件要放在freertos/FreeRTOS.h之后
// #include "esp_log.h"

// #define NUM0_BIT BIT0
// #define NUM1_BIT BIT1

// static EventGroupHandle_t enent_ev;

// void TaskA(void *param)
// {
//     while (1)
//     {
//         xEventGroupSetBits(enent_ev,NUM0_BIT);
//         vTaskDelay(pdMS_TO_TICKS(1000));
//         xEventGroupSetBits(enent_ev,NUM1_BIT);
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

// void TaskB(void *param)
// {
//     EventBits_t ev;
//     while (1)
//     {   //句柄，位，是否退出清除，是否等待所有位，等待时间      
//         ev=xEventGroupWaitBits(enent_ev,NUM0_BIT | NUM1_BIT,pdTRUE,pdFALSE,pdMS_TO_TICKS(5000));//param3:是否退出清除，param4:是等待所有位，
//         if(ev & NUM0_BIT)
//         {
//             ESP_LOGI("event","get BIT0");
//         }
//         if(ev & NUM1_BIT)
//         {
//             ESP_LOGI("event","get BIT1");
//         }
//     }
// }

// void app_main(void)
// {
//     enent_ev= xEventGroupCreate();
//     xTaskCreatePinnedToCore(TaskA,"TaskA",2048,NULL,3,NULL,1);
//     xTaskCreatePinnedToCore(TaskB,"TaskB",2048,NULL,3,NULL,1);
// }


/*********************************************第二遍***************************************************** */
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"

EventGroupHandle_t event_handle;



void TaskA(void * param)
{
    while (1)
    {
        xEventGroupSetBits(event_handle,BIT0);
        vTaskDelay(pdMS_TO_TICKS(1500));   /* code */
    }
    

}


void TaskB(void * param)
{
    while(1)
    {
        xEventGroupSetBits(event_handle,BIT1);
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
 
}


void TaskC(void * param)
{
    EventBits_t ev;
    while(1)
    {
        ev=xEventGroupWaitBits(event_handle,BIT0|BIT1,true,false,pdMS_TO_TICKS(500));
        if(ev==BIT0)
        {
            ESP_LOGI("BIT0","TaskC is get BIT0");   
        }
        else if(ev==BIT1)
        {
            ESP_LOGI("BIT1","TaskC is get BIT0");   
        }
        else if(ev==(BIT1 | BIT0 ))
        {
            ESP_LOGI("BIT01","TaskC is get BIT0 and BIT1");   
        }
        else ESP_LOGI("BIT0","TaskC is get NULL");   
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
    
}


void app_main(void)
{
    event_handle = xEventGroupCreate();
    xTaskCreatePinnedToCore(TaskA,"TaskA",2048,NULL,3,NULL,1);
    xTaskCreatePinnedToCore(TaskB,"TaskB",2048,NULL,3,NULL,1);
    xTaskCreatePinnedToCore(TaskC,"TaskC",2048,NULL,3,NULL,1);
}

//  event_handle = xEventGroupCreate();
//  xEventGroupSetBits(event_handle,BIT0);
//  ev=xEventGroupWaitBits(event_handle,BIT0|BIT1,true,false,pdMS_TO_TICKS(500));


