// #include <stdio.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/semphr.h"
// #include "esp_log.h"
// #include "driver/gpio.h"
// #include "dht11.h"//记得修改make文件

// SemaphoreHandle_t dht11_mutex;

// void TaskA(void *param)
// {
//     int temp, humidity;
//     while(1)
//     {
//         xSemaphoreTake(dht11_mutex,portMAX_DELAY);//获取互斥信号量
//         vTaskDelay(pdMS_TO_TICKS(500));
//         if(DHT11_StartGet(&temp, &humidity))
//         {
//             ESP_LOGI("A1-->DHT11","temp:%d,humidity:%d", temp/10, humidity);
//         }
//         xSemaphoreGive(dht11_mutex);//释放互斥信号量
//         vTaskDelay(pdMS_TO_TICKS(1000));
        
//     }
// }

// void TaskB(void *param)
// {
//     int temp, humidity;
//     while(1)
//     {
//         xSemaphoreTake(dht11_mutex,portMAX_DELAY);//获取互斥信号量
//         vTaskDelay(pdMS_TO_TICKS(500));
//         if(DHT11_StartGet(&temp, &humidity))//如果两个函数同时访问，就会程序崩坏
//         {
//             ESP_LOGI("B1-->DHT11","temp:%d,humidity:%d", temp/10, humidity);
//         }
      
//         xSemaphoreGive(dht11_mutex);//释放互斥信号量 
//         vTaskDelay(pdMS_TO_TICKS(1500));
        

//     }
// }  


// void app_main(void)
// {
//     dht11_mutex = xSemaphoreCreateMutex();
//     DHT11_Init(GPIO_NUM_25);
//     xTaskCreatePinnedToCore(TaskA,"TaskA",2048,NULL,3,NULL,1);
//     xTaskCreatePinnedToCore(TaskB,"TaskB",2048,NULL,4,NULL,1);
// }



/***********************************  第二遍    ******************************************************* */
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "dht11.h"
#include "driver/gpio.h"

QueueHandle_t sem_handle;

void TaskA(void *param)
{
    int temp,hump;
    while (1)
    {
        xSemaphoreTake(sem_handle,100);
        vTaskDelay(pdMS_TO_TICKS(1000));
        
        if(DHT11_StartGet(&temp,&hump))
        {
            ESP_LOGI("TaskA","task get temp is %.1f,task get hump is %d",(float)temp/10,hump);
        }
        xSemaphoreGive(sem_handle);
        vTaskDelay(pdMS_TO_TICKS(1000));
        /* code */
    }
    
}


void TaskB(void *param)
{
    int temp,hump;
    while (1)
    {
        xSemaphoreTake(sem_handle,100);
        vTaskDelay(pdMS_TO_TICKS(1000));
        
        if(DHT11_StartGet(&temp,&hump))
        {
            ESP_LOGI("TaskA","task get temp is %.1f,task get hump is %d",(float)temp/10,hump);
        }
        xSemaphoreGive(sem_handle);
        vTaskDelay(pdMS_TO_TICKS(1000));
        /* code */
    }
}



void app_main(void)
{
    sem_handle=xSemaphoreCreateCounting(3,1);
    DHT11_Init(GPIO_NUM_25);
    xTaskCreatePinnedToCore(TaskA,"TaskA",2048,NULL,3,NULL,1);
    xTaskCreatePinnedToCore(TaskB,"TaskB",2048,NULL,3,NULL,1);
}
