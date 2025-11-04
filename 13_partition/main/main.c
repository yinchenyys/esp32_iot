#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_partition.h"
#include <string.h>

//csv文件列表要放在根目录下


//    Partition Table  ---> 第一行 ： Partition Table (Custom partition table CSV)  ---> (X) Custom partition table CSV
//                          第二行 : (partitions_user.csv) Custom partition CSV file 输入分区表名(分区表要放在根目录)   partitions_user.csv 






static const char* TAG = "main";

#define USER_PARTITION_TYPE     0x40        //自定义的分区类型可设为0x40-0xFF，csv里已经写明
#define USER_PARTITION_SUBTYPE  0x01        //自定义的分区子类型（自定义类型，内存不care）

#define partition_read //仅读取

//1.创建分区指针esp_partition_t
static const esp_partition_t* partition_res=NULL; //用于操作分区

//读取缓存
static char g_esp_buf[1024];

void app_main(void)
{
    //2.找到自定义分区，返回分区指针，esp_partition_find_first
    //esp_partition_find会全部找出，esp_partition_find_first仅会找第一个
    partition_res=esp_partition_find_first(USER_PARTITION_TYPE,USER_PARTITION_SUBTYPE,"user");
    if(partition_res == NULL)
    {
        ESP_LOGI(TAG,"Can't find partition,return");
        return;
    }
    const char* test_str = "this is for test string";

#ifndef partition_read

    //3.擦除自定义分区esp_partition_erase_range
    ESP_ERROR_CHECK(esp_partition_erase_range(partition_res,0x0,0x1000));//0x0表示从该分区的0开始擦除，擦除最少是4kb,切要对齐0x1000(既4kb的倍数)
    //测试字符串
    //4.从分区偏移位置0x0写入字符串esp_partition_write
    ESP_ERROR_CHECK(esp_partition_write(partition_res,0x00, test_str, strlen(test_str)));

#endif // 
   
    //5.从分区偏移位置0x0读取字符串esp_partition_read
    ESP_ERROR_CHECK(esp_partition_read(partition_res,0x00, g_esp_buf, strlen(test_str)));
    ESP_LOGI(TAG,"Read partition str:%s",g_esp_buf);
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


//////////////////////////////////////////////  第一遍  //////////////////////////////////////////////////////////
// #include <string.h>
// #include <stdio.h>
// #include "esp_log.h"
// #include "esp_partition.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// const esp_partition_t* partition;

// #define partition_type     0x40
// #define partition_subtype  0x01
// #define partition_wirte

// char buff[1024];

// void app_main(void)
// {
//     char *test_char = "Request a dialogue with the Creator";
//     partition = esp_partition_find_first(partition_type,partition_subtype,"NULL");

//     #ifdef partition_wirte
//         esp_partition_erase_range(partition,0x0,0x1000);

//         esp_partition_write(partition,0x0,test_char,strlen(test_char));
//     #endif 


//     esp_partition_read(partition,0x0,buff,strlen(test_char));

//     ESP_LOGI("main","Read partition is %s",buff);

//     while(1)
//     {
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }

// }