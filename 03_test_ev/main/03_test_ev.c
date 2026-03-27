#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "esp_log.h"

//static EventGroupHandle_t enent_ev;
static TaskHandle_t TaskA_handle;
static TaskHandle_t TaskB_handle;

void TaskA(void *param)
{
    uint32_t num=0;
    while (1)
    {
        xTaskNotify(TaskB_handle,num,eSetValueWithOverwrite);
        vTaskDelay(pdMS_TO_TICKS(1000));
        num++;
    }
}

void TaskB(void *param)
{
    uint32_t value;
    while (1)
    {
        //param:进入函数清除的通知值，退出函数清除的通知值，通知值，函数
        xTaskNotifyWait(0x00,ULLONG_MAX,&value,portMAX_DELAY);
        ESP_LOGI("Notify","notify num : %lu",value);     

    }
}

void app_main(void)
{ 
    xTaskCreatePinnedToCore(TaskB,"TaskB",2048,NULL,3,&TaskB_handle,1);//任务A先创建，可能会没创建B,就先请求
    xTaskCreatePinnedToCore(TaskA,"TaskA",2048,NULL,3,&TaskA_handle,1);
}

/*************************************** 第2遍  ********************************************************** */
// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/event_groups.h"
// #include "esp_log.h"


// TaskHandle_t TaskA_handle;
// TaskHandle_t TaskB_handle;

// void TaskA(void *param)
// {
//     uint32_t value = 0;
//     while(1)
//     {
//          xTaskNotify(TaskB_handle,value,eSetValueWithOverwrite);
//          vTaskDelay(pdMS_TO_TICKS(1000));
//          value++;
//     }
   


// }


// void TaskB(void *param)
// {
//     uint32_t value = 0;
//     while(1)
//     {
//         xTaskNotifyWait(0x00,ULLONG_MAX,&value,pdMS_TO_TICKS(2000));//代码会阻塞在这里无法执行
//         ESP_LOGI("TaskB","wait notify is %lu",value);
//         //ESP_LOGI("TaskB","??????");
//     }
    
// }


// void app_main(void)
// {
//     //先建A再B会崩溃
//     xTaskCreatePinnedToCore(TaskB,"TaskB",2048,NULL,3,&TaskB_handle,1);
//     xTaskCreatePinnedToCore(TaskA,"TaskA",2048,NULL,3,&TaskA_handle,1);
// }



//xTaskNotify(TaskB_handle,num,eSetValueWithOverwrite);
//xTaskNotifyWait(0x00,ULLONG_MAX,&value,portMAX_DELAY);


