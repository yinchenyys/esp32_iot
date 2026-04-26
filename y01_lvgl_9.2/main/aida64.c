#include "aida64.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "custom.h"

#define TAG "AIDA64"

static char host_aida64_url[128];

static TaskHandle_t  aida64_task_handle = NULL;
static esp_http_client_handle_t http_event_client_handle = NULL;
// static http_event_handle_cb http_event_handler = NULL;
static EventGroupHandle_t aida64_event_handle = NULL;

#define AIDA64_CONNECT_BIT BIT0

static bool is_http_connect = false;

extern lv_ui guider_ui;

//解析函数
// HTTP/1.1 200 OK
// Content-Type: text/event-stream
// Cache-Control: no-cache
// Access-Control-Allow-Origin: *
// Access-Control-Expose-Headers: *
// Access-Control-Allow-Credentials: true

// data: Page0|{|}Simple1|CPU rate 3093 MHz{|}Simple2|CPU temp 0鈩儃|}Simple3|MEM rate 69%{|}Simple4|MEM use 5019 MB{|}

static bool aida64_monitor_parse(char* data, aida64_data_t * aida64_data)//传入自定义结构体
{
    const char * search_str = NULL;
    if(!data)
        return false;


    search_str = strstr(data,"CPU rate ");//定位到出现CPU rate，的"C"地址的指针（即修改该指针，会修改data）
    if(search_str)
    {
        sscanf(search_str,"%*[^0-9]%d",&aida64_data->cpu_rate);//在 %*[^0-9] 跳过前缀的非数字字符后，%d 会紧接着读取找到的第一个整数。
    }
    else 
        return false;


    search_str = strstr(data,"CPU temp ");//CPU rate 及前面的内容都不要
    if(search_str)
    {
        sscanf(search_str,"%*[^0-9]%d",&aida64_data->cpu_temp);//在 %*[^0-9] 跳过前缀的非数字字符后，%d 会紧接着读取找到的第一个整数。
    }
    else 
        return false;


    search_str = strstr(data,"MEM rate ");//CPU rate 及前面的内容都不要
    if(search_str)
    {
        sscanf(search_str,"%*[^0-9]%d",&aida64_data->mem_rate);//在 %*[^0-9] 跳过前缀的非数字字符后，%d 会紧接着读取找到的第一个整数。
    }
    else 
        return false;


    search_str = strstr(data,"MEM use ");//CPU rate 及前面的内容都不要
    if(search_str)
    {
        sscanf(search_str,"%*[^0-9]%d",&aida64_data->mem_use);//在 %*[^0-9] 跳过前缀的非数字字符后，%d 会紧接着读取找到的第一个整数。
    }
    else 
        return false;

    return true;
    
}




static esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            is_http_connect = true;
            set_link_state(&guider_ui,true);
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
        {
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            printf("HTTP_EVENT_ON_DATA data=%.*s\r\n",evt->data_len,(char*)evt->data);
            aida64_data_t aida64_data;
            //与天气数据流不同
            if(aida64_monitor_parse((char*)evt->data, &aida64_data))
            {
                set_cpu_num(&guider_ui,aida64_data.cpu_rate,aida64_data.cpu_temp,aida64_data.mem_rate,aida64_data.mem_use);
            }



            break;
        }
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
            break;
        default:break;
    }
    return ESP_OK;
}

static void aiad64_monitor_task(void *param)
{
    esp_http_client_config_t config = 
    {
        .url ="http://192.168.1.2:80/sse",//先随便写,拿个句柄先
        .event_handler = http_event_handler 
    };
     http_event_client_handle = esp_http_client_init(&config);//返回代表http连接的句柄
    
    while(1)
    {
        EventBits_t ev = xEventGroupWaitBits(aida64_event_handle,AIDA64_CONNECT_BIT,true,false,pdMS_TO_TICKS(1000));
        if(ev & AIDA64_CONNECT_BIT )//连接按下
        {
            esp_http_client_set_url(http_event_client_handle,host_aida64_url);
            esp_http_client_set_method(http_event_client_handle, HTTP_METHOD_GET);
            esp_http_client_set_header(http_event_client_handle,"Accept","text/event-stream");
            ESP_LOGI(TAG,"http_perform");
            esp_http_client_perform(http_event_client_handle);//发起http连接（以时间片）
            //This function include `esp_http_client_open` -> `esp_http_client_write` -> `esp_http_client_fetch_headers` -> `esp_http_client_read` (and option) `esp_http_client_close`.
            //但我们不会我结束，会一直调用
            is_http_connect = false;
            set_link_state(&guider_ui,false);
        }
    }
}


void aida64_monitor_start(char *ip)
{
     if(aida64_event_handle == NULL)
     {
        aida64_event_handle = xEventGroupCreate();
     } 
     if(aida64_task_handle == NULL)
     {
        xTaskCreatePinnedToCore(aiad64_monitor_task,"AIDA64",4096,NULL,3,&aida64_task_handle,1);
     }
     snprintf(host_aida64_url, sizeof(host_aida64_url),"http://%s:80/sse",ip);
     xEventGroupSetBits(aida64_event_handle,AIDA64_CONNECT_BIT);
}


void aia64_monitor_stop(void)
{
    esp_http_client_close(http_event_client_handle);//
}

int aida64_monitor_isconnect(void)
{
    return is_http_connect;
}