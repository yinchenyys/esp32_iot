// #include <stdio.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"
// #include "esp_log.h"

// QueueHandle_t queue_handle=NULL;//队列句柄

// typedef struct 
// {
//     int value;
// }queue_data_t;//队列里面的数据单元

// void taskA(void *param)
// {
//     queue_data_t data;
//     while (1)
//     {
//         //3.创建接收xQueueReceive
//         if (pdTRUE == xQueueReceive(queue_handle,&data,100))//任务句柄，缓存，超时时间
//         {
//             ESP_LOGI("queue","receive data:%d",data.value);
//         }
//     }
    
// }

// void taskB(void *param)
// {
//     queue_data_t data;
//     memset(&data,0,sizeof(queue_data_t));
//     while (1)
//     {
//         //3.创建发送xQueueSend
//         xQueueSend(queue_handle,&data,100);
//         vTaskDelay(pdMS_TO_TICKS(1000));
//         data.value++;
    
//     }
    
// }

// void app_main(void)
// {
//     //1.创建消息队列xQueueCreate
//     queue_handle = xQueueCreate(10,sizeof(queue_data_t));//创建队列

//     //2.创建函数xTaskCreatePinnedToCore
//     xTaskCreatePinnedToCore(taskA,"taskA",2048,NULL,3,NULL,1);
//     xTaskCreatePinnedToCore(taskB,"taskB",2048,NULL,3,NULL,1);

    

// }




/**********************************   第二遍    *********************************************************** */
// #include <stdio.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"
// #include "esp_log.h"

// typedef struct 
// {
//     int vule;
//     /* data */
// } queue_num;

// QueueHandle_t queue_handle;

// void taskA(void * parem)
// {
//     queue_num data;
//     while(1)
//     {
//         if(pdTRUE== xQueueReceive(queue_handle,&data,100))
//         {
//             ESP_LOGI("TaskA","receive data is %d",data.vule);
//         }
//     }
// }




// void taskB(void * parem)
// {
//     queue_num data;
//     memset(&data,0,sizeof(queue_num));//分配全是0
//     while(1)
//     {
//         if(pdTRUE==xQueueSend(queue_handle,&data,100))
//         {
//             ESP_LOGI("TaskB","send data is %d",data.vule);
//         }
       
//        vTaskDelay(pdMS_TO_TICKS(1000));
//        data.vule++;
//     }
   
    
// }





// void app_main(void)
// {
//     queue_handle = xQueueCreate(10,sizeof(queue_num));
//     xTaskCreatePinnedToCore(taskA,"taskA",2048,NULL,3,NULL,1);
//     xTaskCreatePinnedToCore(taskB,"taskB",2048,NULL,3,NULL,1);
    

// }


/****************************************第三遍********************************************************** */
// #include <stdio.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"
// #include "esp_log.h"


// typedef struct 
// {
//     int vaiue;

// }queue_data;



// QueueHandle_t queue_handle;

// void TaskA(void * param)
// {
//     queue_data data;
//     memset(&data,0,sizeof(queue_data));

//     while (1)
//     {
//         if(pdTRUE == xQueueSend(queue_handle,&data,100) )
//         {
//             ESP_LOGI("TaskA","send data is %d",data.vaiue);
//         }
//         vTaskDelay(pdMS_TO_TICKS(1000));
//         data.vaiue++;
//         /* code */
//     }
    


// }

// void TaskB(void * param)
// {
//     queue_data data;
//     memset(&data,0,sizeof(queue_data));

//     while (1)
//     {
//         if(pdTRUE == xQueueReceive(queue_handle,&data,100) )
//         {
//             ESP_LOGI("TaskB","receive data is %d",data.vaiue);
//         }
//         /* code */
//     }

    
// }

// void app_main(void)
// {
//     queue_handle=xQueueCreate(10,sizeof(queue_data));

//     xTaskCreatePinnedToCore(TaskA,"TaskA",2048,NULL,3,NULL,1);
//     xTaskCreatePinnedToCore(TaskB,"TaskB",2048,NULL,3,NULL,1);
// }



/***************************************** 第四遍 ****************************************************** */
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

QueueHandle_t queue_handle;

typedef struct 
{
    int value;
    /* data */
}queue_data;



void TaskA(void * param)
{
    queue_data data;
    memset(&data,0,sizeof(queue_data));
    while (1)
    {
        if(pdTRUE==xQueueSend(queue_handle,&data,100))
        {
            ESP_LOGI("taskA","TaskA send is %d",data.value);
        }
        data.value++;
        vTaskDelay(pdMS_TO_TICKS(1000));
          /* code */
    }
    
    
}

void TaskB(void * param)
{
    queue_data data;
    memset(&data,0,sizeof(queue_data));
    while (1)
    {
        if(pdTRUE==xQueueReceive(queue_handle,&data,100))
        {
            ESP_LOGI("taskB","TaskB send is %d",data.value);
        }         /* code */
    }
    
}


void app_main(void)
{
    queue_handle=xQueueCreate(10,sizeof(queue_data));
    xTaskCreatePinnedToCore(TaskA,"TaskA",2048,NULL,3,NULL,1);
    xTaskCreatePinnedToCore(TaskB,"TaskB",2048,NULL,3,NULL,1);
}