// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_log.h"
// #include "nvs_flash.h"
// #include "esp_err.h"

// static const char* TAG = "main";

// #define NVS_BOB_NAMESPACE  "Bob"        //namespace最长15字节
// #define NVS_JOHN_NAMESPACE  "John"      //namespace最长15字节

// #define NVS_AGE_KEY    "age"        //年龄键名
// #define NVS_SEX_KEY    "sex"        //性别键名

// //#define NVS_write

// /** 从nvs中读取字符值
//  * @param namespace NVS命名空间
//  * @param key 要读取的键值
//  * @param value 读到的值
//  * @param maxlen 外部存储数组的最大值
//  * @return 读取到的字节数
// */
// static size_t read_nvs_str(const char* namespace,const char* key,char* value,int maxlen)
// {
//     nvs_handle_t nvs_handle;
//     esp_err_t ret_val = ESP_FAIL;
//     size_t required_size = 0;
//     ESP_ERROR_CHECK(nvs_open(namespace, NVS_READWRITE, &nvs_handle));
//     ret_val = nvs_get_str(nvs_handle, key, NULL, &required_size);//传NULL会一直到0x00的长度到required_size
//     if(ret_val == ESP_OK && required_size <= maxlen)
//     {
//         nvs_get_str(nvs_handle,key,value,&required_size);
//     }
//     else
//         required_size = 0;
//     nvs_close(nvs_handle);
//     return required_size;
// }

// /** 写入值到NVS中（字符数据）
//  * @param namespace NVS命名空间
//  * @param key NVS键值
//  * @param value 需要写入的值
//  * @return ESP_OK or ESP_FAIL
// */
// static esp_err_t write_nvs_str(const char* namespace,const char* key,const char* value)
// {
//     nvs_handle_t nvs_handle;
//     esp_err_t ret;
//     //(1).打开命名空间nvs_open
//     ESP_ERROR_CHECK(nvs_open(namespace, NVS_READWRITE, &nvs_handle));
    
//     //(2).nvs_set_str
//     ret = nvs_set_str(nvs_handle, key, value);
//     nvs_commit(nvs_handle);
//     nvs_close(nvs_handle);
//     return ret;
// }










// /** 写入值到NVS中(字节数据)
//  * @param namespace NVS命名空间
//  * @param key NVS键值
//  * @param value 需要写入的值
//  * @return ESP_OK or ESP_FAIL
// */
// static esp_err_t write_nvs_blob(const char* namespace,const char* key,uint8_t* value,size_t len)
// {
//     nvs_handle_t nvs_handle;
//     esp_err_t ret;
//     //(1).打开命名空间nvs_open
//     ESP_ERROR_CHECK(nvs_open(namespace, NVS_READWRITE, &nvs_handle));
//     //(2).把要数组的内容保存到虚拟空间nvs_set_blob
//     ret = nvs_set_blob(nvs_handle, key, value,len);
//     //(3).立即写入，不然会等待系统到一定时序再nvs_commit
//     nvs_commit(nvs_handle);//写入
//     //(4).关闭句柄nvs_close
//     nvs_close(nvs_handle);//关闭句柄
//     return ret;
// }



// /** 从nvs中读取字节数据（二进制）
//  * @param namespace NVS命名空间
//  * @param key 要读取的键值
//  * @param value 读到的值
//  * @param maxlen 外部存储数组的最大值
//  * @return 读取到的字节数
// */
// static size_t read_nvs_blob(const char* namespace,const char* key,uint8_t *value,int maxlen)
// {
//     nvs_handle_t nvs_handle;
//     esp_err_t ret_val = ESP_FAIL;
//     size_t required_size = 0;
//     //(1).打开命名空间nvs_open
//     ESP_ERROR_CHECK(nvs_open(namespace, NVS_READWRITE, &nvs_handle));
//     //(2).获取内容到要保存的数组nvs_get_blob（直接长度不确定，第3个参数填NULL,第4个参数会返回长度）
//     ret_val = nvs_get_blob(nvs_handle, key, NULL, &required_size);
//     if(ret_val == ESP_OK && required_size <= maxlen)
//     {
//         nvs_get_blob(nvs_handle,key,value,&required_size);
//     }
//     else
//         required_size = 0;
//     //(3).关闭句柄nvs_close
//     nvs_close(nvs_handle);
//     return required_size;
// }

// /** 擦除nvs区中某个键
//  * @param namespace NVS命名空间
//  * @param key 要读取的键值
//  * @return 错误值
// */
// static esp_err_t erase_nvs_key(const char* namespace,const char* key)
// {
//     nvs_handle_t nvs_handle;
//     esp_err_t ret_val = ESP_FAIL;
//     ESP_ERROR_CHECK(nvs_open(namespace, NVS_READWRITE, &nvs_handle));
//     ret_val = nvs_erase_key(nvs_handle,key);
//     ret_val = nvs_commit(nvs_handle);
//     nvs_close(nvs_handle);
//     return ret_val;
// }



// void app_main(void)
// {
//     //第一种写入方式(写入性别)，（不用先开辟命名空间）
//     //1.初始化NVS nvs_flash_init
//     esp_err_t ret = nvs_flash_init();
//     if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {//
//         ESP_ERROR_CHECK(nvs_flash_erase()); //NVS出现错误，执行擦除
//         ESP_ERROR_CHECK(nvs_flash_init());//重新尝试初始化
//     }

//     char read_buf[64];
//     size_t len = 0;


// #ifdef  NVS_write
//     //2.以字节方式写入到NVS中 write_nvs_str
//     write_nvs_str(NVS_BOB_NAMESPACE,NVS_SEX_KEY,"female"); 
//     write_nvs_str(NVS_JOHN_NAMESPACE,NVS_SEX_KEY,"male");
// #else
//     //3.读取NVS_BOB_NAMESPACE命名空间中的SEX键值 read_nvs_str
//     len =read_nvs_str(NVS_BOB_NAMESPACE,NVS_SEX_KEY,read_buf,64);
//     if(len)
//         ESP_LOGI(TAG,"Read BOB SEX:%s",read_buf);
//     else
//         ESP_LOGI(TAG,"Read BOB SEX fail,please perform nvs_erase_key and try again");

//      //3.读取NVS_JOHN_NAMESPACE命名空间中的SEX键值 read_nvs_str()返回长度
//     len =read_nvs_str(NVS_JOHN_NAMESPACE,NVS_SEX_KEY,read_buf,64);
//     if(len)
//         ESP_LOGI(TAG,"Read JOHN SEX:%s",read_buf);
//     else
//         ESP_LOGI(TAG,"Read JOHN SEX fail,please perform nvs_erase_key and try again");
// #endif



//    //二.以字节方式写入
//     uint8_t blob_buf[32];
 
// #ifdef  NVS_write
//     //2.以数组方式写入write_nvs_blob
//     blob_buf[0] = 19;
//     write_nvs_blob(NVS_BOB_NAMESPACE,NVS_AGE_KEY,blob_buf,1); 
//     blob_buf[0] = 23;
//     write_nvs_blob(NVS_JOHN_NAMESPACE,NVS_AGE_KEY,blob_buf,1); 
    
// #else
//     //3.以字节方式读取read_nvs_blob
//     len = read_nvs_blob(NVS_BOB_NAMESPACE,NVS_AGE_KEY,blob_buf,32);
//     if(len)
//         ESP_LOGI(TAG,"Read BOB age:%d",blob_buf[0]);
//     else
//         ESP_LOGI(TAG,"Read BOB age fail,please perform nvs_erase_key and try again");

//     //3.以字节方式读取read_nvs_blob
//     len = read_nvs_blob(NVS_JOHN_NAMESPACE,NVS_AGE_KEY,blob_buf,32);
//     if(len)
//         ESP_LOGI(TAG,"Read JOHN age:%d",blob_buf[0]);
//     else
//         ESP_LOGI(TAG,"Read JOHN age fail,please perform nvs_erase_key and try again");
// #endif
    

//     while(1)
//     {
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }



//////////////////////////////////////////////////////  第二遍  //////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_err.h"

#define WiFI_space "WiFI"
#define WiFI_name "yys"

//#define nvs_write


char* wifi_passworld ="15088145513" ;

nvs_handle_t Wifi_handle;

void app_main(void)
{
    esp_err_t err;
    err = nvs_flash_init();
    if(err)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }

#ifdef nvs_write    

    nvs_open(WiFI_space,NVS_READWRITE,&Wifi_handle);
    nvs_set_blob(Wifi_handle,WiFI_name,wifi_passworld,11);//sizeof(wifi_passworld)
    nvs_commit(Wifi_handle);
    nvs_close(Wifi_handle);
    ESP_LOGI("main","sizeof is %d",sizeof(wifi_passworld));
    //ESP_LOGI("main","sizeof is %s",strlen(wifi_passworld));
    

#else

    char buf[32]="";
    //memset(buf,32,0);
    size_t len=0;
    nvs_open(WiFI_space,NVS_READONLY,&Wifi_handle);
    nvs_get_blob(Wifi_handle,WiFI_name,NULL,&len);
    nvs_get_blob(Wifi_handle,WiFI_name,buf,&len);
 
    ESP_LOGI("main","passworld is %s",buf);
    
    nvs_close(Wifi_handle);



#endif // !nvs_write

     



}