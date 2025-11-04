// /* SPIFFS filesystem example.
//    This example code is in the Public Domain (or CC0 licensed, at your option.)

//    Unless required by applicable law or agreed to in writing, this
//    software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//    CONDITIONS OF ANY KIND, either express or implied.
// */

// #include <stdio.h>
// #include <string.h>
// #include <sys/unistd.h>
// #include <sys/stat.h>
// #include "esp_err.h"
// #include "esp_log.h"
// #include "esp_spiffs.h"

// /*
// # Name,   Type, SubType, Offset,  Size, Flags
// # Note: if you have increased the bootloader size, make sure to update the offsets to avoid overlap
// nvs,      data, nvs,     0x9000,  0x6000,
// phy_init, data, phy,     0xf000,  0x1000,
// factory,  app,  factory, 0x10000, 1M,
// storage,  data, spiffs,  ,        0xF0000, //加入这一行
// */



// static const char *TAG = "example";

// void app_main(void)
// {
//     ESP_LOGI(TAG, "Initializing SPIFFS");

//     esp_vfs_spiffs_conf_t conf = {
//       .base_path = "/spiffs",           //可以认为挂着点，后续使用C库函数fopen("/spiffs/...")
//       .partition_label = NULL,          //指定spiffs分区，如果为NULL，则默认为分区表中第一个spiffs类型的分区
//       .max_files = 5,                   //最大可同时打开的文件数
//       .format_if_mount_failed = true    //挂载失败是否格式化
//     };

//     //1.初始化和挂载spiffs分区esp_vfs_spiffs_register
//     esp_err_t ret = esp_vfs_spiffs_register(&conf);

//     //失败处理
//     if (ret != ESP_OK) {
//         if (ret == ESP_FAIL) {
//             ESP_LOGE(TAG, "Failed to mount or format filesystem");
//         } else if (ret == ESP_ERR_NOT_FOUND) {
//             ESP_LOGE(TAG, "Failed to find SPIFFS partition");
//         } else {
//             ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
//         }
//         return;
//     }

//     //2.执行SPIFFS文件系统检查esp_spiffs_check（查看掉电是否导致文件损坏）
//     ESP_LOGI(TAG, "Performing SPIFFS_check().");
//     ret = esp_spiffs_check(conf.partition_label);//操作spiffs文件系统器件断电，可能会导致 SPIFFS 损坏，可通过esp_spiffs_check恢复
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
//         return;
//     } else {
//         ESP_LOGI(TAG, "SPIFFS_check() successful");
//     }


//     //3.获取SPIFFS可用区域大小esp_spiffs_info
//     size_t total = 0, used = 0;
//     ret = esp_spiffs_info(conf.partition_label, &total, &used);
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s). Formatting...", esp_err_to_name(ret));
//         esp_spiffs_format(conf.partition_label);
//         return;
//     } else {
//         ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
//     }

//     //可用空间异常，执行SPIFFS检查
//     if (used > total) {
//         ESP_LOGW(TAG, "Number of used bytes cannot be larger than total. Performing SPIFFS_check().");
//         ret = esp_spiffs_check(conf.partition_label);
//         if (ret != ESP_OK) {
//             ESP_LOGE(TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
//             return;
//         } else {
//             ESP_LOGI(TAG, "SPIFFS_check() successful");
//         }
//     }




//     //4.结合VFS，可以使用标准C库函数进行文件读写fopen，再fprintf
//     ESP_LOGI(TAG, "Opening file");
//     FILE* f = fopen("/spiffs/hello.txt", "w");
//     if (f == NULL) {
//         ESP_LOGE(TAG, "Failed to open file for writing");
//         return;
//     }
//     fprintf(f, "Hello World!\n");
//     fclose(f);
//     ESP_LOGI(TAG, "File written");




//     //异常处理
//     //检查/spiffs/foo.txt这个文件是否存在，如果存在删除它
//     struct stat st;
//     if (stat("/spiffs/foo.txt", &st) == 0) {
//         // 删除/spiffs/foo.txt文件
//         unlink("/spiffs/foo.txt");
//     }
//     //重命名文件
//     ESP_LOGI(TAG, "Renaming file");
//     if (rename("/spiffs/hello.txt", "/spiffs/foo.txt") != 0) {
//         ESP_LOGE(TAG, "Rename failed");
//         return;
//     }




//     //5.打开foo文件读取一行fopen，再fgets
//     ESP_LOGI(TAG, "Reading file");
//     f = fopen("/spiffs/foo.txt", "r");
//     if (f == NULL) {
//         ESP_LOGE(TAG, "Failed to open file for reading");
//         return;
//     }
//     char line[64];
//     fgets(line, sizeof(line), f);
//     fclose(f);
//     // strip newline
//     char* pos = strchr(line, '\n');//找到'\n'字符，并返回下标
//     if (pos) {
//         *pos = '\0';
//     }
//     ESP_LOGI(TAG, "Read from file: '%s'", line);

//     //6.测试完成，卸载esp_vfs_spiffs_unregister
//     esp_vfs_spiffs_unregister(conf.partition_label);
//     ESP_LOGI(TAG, "SPIFFS unmounted");
// }


#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_spiffs.h"
#include "esp_err.h"
#include "sys/stat.h"
#include "sys/unistd.h"

/*总结*/
// 每次写入都会把上次的内容清除，且输出内容遇到/n就不会继续输出

//#define spiffs_fprintf

void app_main(void)
{
    esp_vfs_spiffs_conf_t conf =
    {
        .base_path="/spiffs",   //!err:spiffs->spifft
        .format_if_mount_failed=true,
        .max_files=5,
        .partition_label=NULL,
    };
    esp_vfs_spiffs_register(&conf);

    esp_spiffs_check(conf.partition_label);

    size_t total_buf=0,used_buf=0;
    esp_spiffs_info(conf.partition_label,&total_buf,&used_buf);
    ESP_LOGI("spiffs","spiffs total_buf is %d,spiffs used_buf is %d ",total_buf,used_buf);

    if(total_buf < used_buf)
    {
         ESP_LOGI("spiffs","buff err");
    }


#ifdef spiffs_fprintf

    // struct stat st;
    // if (stat("/spiffs/hello.txt", &st) == 0) {
    //     // 删除/spiffs/foo.txt文件
    //     unlink("/spiffs/hello.txt");
    // }

    FILE* f = fopen("/spiffs/hello.txt", "w");
    if(f==NULL)
    {
        ESP_LOGE("spiffs", "Failed to open file for writing");
        return;
        
    }
     
     fprintf(f, "sb");
     fprintf(f, "hello ");
     fclose(f);



#else

    
    FILE* f = fopen("/spiffs/hello.txt", "r");
    if(f==NULL)
    {
         ESP_LOGI("spiffs","fopen err");
         return;
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    // strip newline
    char* pos = strchr(line, '\n');//找到'\n'字符，并返回下标
    if (pos) {
        *pos = 'a';
    }
    line[64]='\0';
    ESP_LOGI("spiffs","spiffs get %s",line);

    esp_vfs_spiffs_unregister(conf.partition_label);
    fclose(f);

#endif // DEBUG


}