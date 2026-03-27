// #include <stdio.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/semphr.h"
// #include "esp_log.h"
// #include "driver/gpio.h"
// //#include "dht11.h"

// SemaphoreHandle_t bin_sem;

/*二值信号量
void TaskA(void *param)
{
    while(1)
    {
        xSemaphoreGive(bin_sem);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void TaskB(void *param)
{
    //等待信号量后，打印信息
    while(1)
    {
        if(pdTRUE == xSemaphoreTake(bin_sem,portMAX_DELAY))
        {
            ESP_LOGI("bin","taskB successful");
        }
    }
}  


void app_main(void)
{
    bin_sem = xSemaphoreCreateBinary();
    xTaskCreatePinnedToCore(TaskA,"TaskA",2048,NULL,3,NULL,1);
    xTaskCreatePinnedToCore(TaskB,"TaskB",2048,NULL,4,NULL,1);
}
*/






// #include <stdio.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/semphr.h"
// #include "esp_log.h"
// #include "driver/gpio.h"
// //#include "dht11.h"

// SemaphoreHandle_t bin_sem;
// /*数字信号量*/
// void TaskA(void *param)
// {
//     while(1)
//     {
//         xSemaphoreGive(bin_sem);
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

// void TaskB(void *param)
// {
//     //等待信号量后，打印信息
//     while(1)
//     {
//         if(pdTRUE == xSemaphoreTake(bin_sem,portMAX_DELAY))
//         {
//             ESP_LOGI("bin","taskB successful");
//         }
//     }
// }

// void TaskC(void *param)
// {
//     //等待信号量后，打印信息
//     while(1)
//     {
//         if(pdTRUE == xSemaphoreTake(bin_sem,portMAX_DELAY))
//         {
//             ESP_LOGI("bin","taskC successful");
//         }
//     }
// }


// void app_main(void)
// {
//     bin_sem = xSemaphoreCreateCounting(3,1);
//     //bin_sem = xSemaphoreCreateBinary();
//     xTaskCreatePinnedToCore(TaskA,"TaskA",2048,NULL,3,NULL,1);
//     xTaskCreatePinnedToCore(TaskB,"TaskB",2048,NULL,4,NULL,1);
//     xTaskCreatePinnedToCore(TaskC,"TaskC",2048,NULL,4,NULL,1);
// }







/***************************************第二遍*************************************************************** */
// #include <stdio.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/semphr.h"
// #include "esp_log.h"

// QueueHandle_t sem_handle;

// void TaskA(void *param)
// {
//     while (1)
//     {
//         if(pdTRUE==xSemaphoreGive(sem_handle))
//         {
//             ESP_LOGI("TaskA","TaskA is give Sem");
//         }
//         vTaskDelay(pdMS_TO_TICKS(1000));
//         /* code */
//     }
    

// }

// void TaskB(void *param)
// {
//     while (1)
//     {
//         if(pdTRUE==xSemaphoreTake(sem_handle,100))
//         {
//             ESP_LOGI("TaskB","TaskB is take Sem");
//         }
//         /* code */
//     }
    
// }


// void TaskC(void *param)
// {
//     while (1)
//     {
//         if(pdTRUE==xSemaphoreTake(sem_handle,100))
//         {
//             ESP_LOGI("TaskC","TaskC is take Sem");
//         }
//         /* code */
//     }
// }



// void app_main(void)
// {
//     sem_handle=xSemaphoreCreateCounting(3,0);
//     xTaskCreatePinnedToCore(TaskA,"TaskA",2048,NULL,3,NULL,1);
//     xTaskCreatePinnedToCore(TaskB,"TaskB",2048,NULL,3,NULL,1);
//     xTaskCreatePinnedToCore(TaskC,"TaskC",2048,NULL,4,NULL,1);
// }



/*****************************************第三遍******************************************************** */
#include<stdio.h>
#include<string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"


QueueHandle_t sem_handle;


void TaskA(void* param)
{
    while (1)
    {
        if(pdTRUE==xSemaphoreGive(sem_handle))
        {
            ESP_LOGI("taskA","taskA is give");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
       
        /* code */
    }
}


void TaskB(void* param)
{
    while (1)
    {
        if(pdTRUE==xSemaphoreTake(sem_handle,1000))
        {
            ESP_LOGI("taskB","taskB is take");
        }
        else ESP_LOGI("taskB","taskB is faill");
     
        /* code */
    }
}


void TaskC(void* param)
{
    while (1)
    {
        if(pdTRUE==xSemaphoreTake(sem_handle,1000))
        {
            ESP_LOGI("taskC","taskC is take");
        }
        else ESP_LOGI("taskC","taskC is faill");
     
        /* code */
    }
}





void app_main(void)
{
    sem_handle=xSemaphoreCreateCounting(10,1);//可以拿10个，已经拿走1个
    xTaskCreatePinnedToCore(TaskA,"TaskA",2048,NULL,3,NULL,1);
    xTaskCreatePinnedToCore(TaskB,"TaskB",2048,NULL,3,NULL,1);
    xTaskCreatePinnedToCore(TaskC,"TaskC",2048,NULL,3,NULL,1);
}



//sem_handle = xSemaphoreCreateBinary();
//sem_handle = xSemaphoreCreateCounting(10,1);
//pdTRUW == xSemaphoreGive(bin_sem)
//pdTRUE == xSemaphoreTake(bin_sem,portMAX_DELAY)



