#include "esp_http_client.h"
#include "weather.h"
#include <string.h>
#include "cJSON.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "wifi_manager.h"
#include "custom.h"

#include "esp_spiffs.h"//csv存储

#define TAG     "WEATHER"

#define WEATHER_BUFF_LEN    2048

extern lv_ui guider_ui;

//私钥
#define WEATHER_PRIVATE_KEY     "SH035InUM1uKg9g7A"    //这里注意，要改成自己的私钥

//http接收到的数据
static uint8_t weather_data_buff[WEATHER_BUFF_LEN];

//http接收到的数据长度
static int weather_data_size = 0;

//地理位置城市
static char city_name[48];

static esp_err_t _weather_http_event_handler(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
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
            int copy_len = 0;
            //因为这个HTTP_EVENT_ON_DATA事件可能会不只一次的触发，也就是
            //会分多次接收，因此这里需要考虑多次接收数据的完整
            //weather_data_size会记录数据的长度
            //weather_data_buff则保存了收到的所有http数据
            if(evt->data_len > WEATHER_BUFF_LEN - weather_data_size)
            {
                copy_len = WEATHER_BUFF_LEN - weather_data_size;
            }
            else
            {
                copy_len = evt->data_len;
            }
            memcpy(&weather_data_buff[weather_data_size],evt->data,copy_len);
            weather_data_size += copy_len;
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




// {
//   "location": {
//     "path": "深圳,深圳,广东,中国",
//     "timezone": "Asia/Shanghai",
//     "timezone_offset": "+08:00"
//   },
//   "daily": [
//     {
//       "date": "2026-04-26",
//       "text_day": "多云",
//       "code_day": "4",
//       "text_night": "多云",
//       "code_night": "4",
//       "high": "30",
//       "low": "21",
//       "rainfall": "0.00",
//       "precip": "0.00",
//       "wind_direction": "无持续风向",
//       "wind_direction_degree": "",
//       "wind_speed": "8.4",
//       "wind_scale": "2",
//       "humidity": "84"
//     },
//     {
//       "date": "2026-04-27",
//       "text_day": "多云",
//       "code_day": "4",
//       "text_night": "雷阵雨",
//       "code_night": "11",
//       "high": "29",
//       "low": "22",
//       "rainfall": "6.83",
//       "precip": "0.75",
//       "wind_direction": "无持续风向",
//       "wind_direction_degree": "",
//       "wind_speed": "8.4",
//       "wind_scale": "2",
//       "humidity": "81"
//     },
//     {
//       "date": "2026-04-28",
//       "text_day": "雷阵雨",
//       "code_day": "11",
//       "text_night": "大雨",
//       "code_night": "15",
//       "high": "28",
//       "low": "22",
//       "rainfall": "32.31",
//       "precip": "1.00",
//       "wind_direction": "无持续风向",
//       "wind_direction_degree": "",
//       "wind_speed": "3.0",
//       "wind_scale": "1",
//       "humidity": "89"
//     }
//   ],
//   "last_update": "2026-04-26T08:00:00+08:00"
// }

/** 解析返回的JSON格式天气信息
 * @param weather_data
 * @return 错误
*/
static esp_err_t pasre_weather(char* weather_data)
{
    weather_data_pkt_t  data[3];
    char city[48];
    cJSON *wt_js = cJSON_Parse(weather_data);
    if(!wt_js)//如果不是一个标准的json格式  
    {
        ESP_LOGI(TAG,"Invaild weather json");
        return ESP_FAIL;
    }
    cJSON *result_js = cJSON_GetObjectItem(wt_js,"results");//获取第一个{}
    if(!result_js)
    {
        ESP_LOGI(TAG,"Invaild weather results");
        return ESP_FAIL;
    }
    cJSON *result_child_js = result_js->child;//获取第一个数组
    cJSON *location_js = cJSON_GetObjectItem(result_child_js,"location");
    cJSON *name_js = cJSON_GetObjectItem(location_js,"name");
    if(name_js)
    {
        snprintf(city,sizeof(city),"%s",cJSON_GetStringValue(name_js));//获取键值对
    }

    cJSON *daily_js = cJSON_GetObjectItem(result_child_js,"daily");
    int index = 0;
    if(daily_js)
    {
        cJSON* daily_child_js = daily_js->child;
        while(daily_child_js)
        {
            cJSON *high_js = cJSON_GetObjectItem(daily_child_js,"high");
            cJSON *low_js = cJSON_GetObjectItem(daily_child_js,"low");
            cJSON *code_js = cJSON_GetObjectItem(daily_child_js,"code_day");
            if(index  < 3)
            {
                sscanf(cJSON_GetStringValue(high_js),"%d",&data[index].high_temp);//获取的是字符串，要转化为整型
                sscanf(cJSON_GetStringValue(low_js),"%d",&data[index].low_temp);
                snprintf(data[index].code,4,"%s",cJSON_GetStringValue(code_js));
                ESP_LOGI(TAG,"day[%d]->high temp:%d,low temp:%d,day_code:%s",index,data[index].high_temp,data[index].low_temp,data[index].code);
            }
            index++;
            daily_child_js = daily_child_js->next;//指向下一个数组
        }
        //todo
        //调用显示接口，把天气信息设置到LVGL里面去
        char img_path[32];
        snprintf(img_path,sizeof(img_path),"/img/%s@1x.png",data[0].code);
        set_today_img(&guider_ui,img_path,data[0].low_temp,data[0].high_temp);

        snprintf(img_path,sizeof(img_path),"/img/%s@1x.png",data[1].code);
        set_tommorrow_img(&guider_ui,img_path,data[1].low_temp,data[1].high_temp);

        snprintf(img_path,sizeof(img_path),"/img/%s@1x.png",data[2].code);
        set_after_img(&guider_ui,img_path,data[2].low_temp,data[2].high_temp);

        set_local_where(&guider_ui,city);



    }
    cJSON_Delete(wt_js);
    return ESP_OK;
}

/** 发起获取天气信息的HTTP请求
 * @param weather_data http响应的天气信息数据（JSON格式）
 * @return 错误
*/
static esp_err_t weather_http_connect(void)
{
    static char url[256];
    snprintf(url,sizeof(url),
    // "http://api.seniverse.com/v3/weather/daily.json?key=%s&location=%s&language=zh-Hans&unit=c&start=0&days=3",
    // WEATHER_PRIVATE_KEY,city_name);
    "http://api.seniverse.com/v3/weather/daily.json?key=%s&location=zhuhai&language=zh-Hans&unit=c&start=0&days=3",
    WEATHER_PRIVATE_KEY);
    esp_http_client_config_t config = 
    {
        .url = url,
        .event_handler = _weather_http_event_handler
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);//返回代表http连接的句柄
    memset(weather_data_buff,0,WEATHER_BUFF_LEN);//清空一下先
    weather_data_size = 0;//长度清零
    //此函数将完成一个完整HTTP请求才返回
    esp_err_t err = esp_http_client_perform(client);//阻塞，//不成返回：HTTP_EVENT_ON_DATA data={"status":"Wrong parameters.","status_code":"AP010001"}
    // ESP_LOGI(TAG,"GET weather data:/n%s",weather_data_buff);  
    esp_http_client_cleanup(client);
    //返回后解析天气信息
    pasre_weather((char*)weather_data_buff);
    //释放掉HTTP资源
    
    return err;
}

/** 解析位置JSON数据
 * @param location_data http响应的地理位置数据（JSON）格式
 * @return 错误
*/
static esp_err_t pasre_location(char* location_data)
{
    cJSON *location_js = cJSON_Parse(location_data);
    if(!location_js)
    {
        ESP_LOGI(TAG,"Invaild location json");
        return ESP_FAIL;
    }
    cJSON *city_js = cJSON_GetObjectItem(location_js,"city");
    if(!city_js)
    {
        ESP_LOGI(TAG,"Invaild location city");
        return ESP_FAIL;
    }
    snprintf(city_name,sizeof(city_name),"%s",cJSON_GetStringValue(city_js));
    //地理位置保存在city_name中
    ESP_LOGI(TAG,"location->city name:%s",city_name);
    cJSON_Delete(location_js);
    return ESP_OK;
}

/** 发起HTTP请求，获取当前地理位置（城市）
 * @param 无
 * @return 错误
*/
static esp_err_t location_http_connect(void)
{
    static char url[256];
    snprintf(url,sizeof(url),
    "http://ip-api.com/json/?lang=en");//获取ip地址
    esp_http_client_config_t config = 
    {
        .url = url,
        .event_handler = _weather_http_event_handler
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    memset(weather_data_buff,0,WEATHER_BUFF_LEN);
    weather_data_size = 0;
    esp_err_t err = esp_http_client_perform(client);

    // ESP_LOGI(TAG,"GET location data:/n%s",weather_data_buff);
    pasre_location((char*)weather_data_buff);
    
    esp_http_client_cleanup(client);
    return err;
}

static void weather_task(void* param)
{
    while(1)
    {
        //先判断一下网络是否已经连接了，如果没有连接循环2秒等待
        if(!wifi_manager_is_connect())
        {
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
        else    //发现已经连接上网络
        {
            //先根据ip地址请求地理位置
            location_http_connect();
            //然后获取天气信息
            weather_http_connect();
            //延时半小时
            vTaskDelay(pdMS_TO_TICKS(2000*1800));
        }
    }
}

/** 启动天气信息状况定时获取
 * @param 无
 * @return 无
*/
void weather_start(void)
{
    //这里会新建一个任务，定时获取天气状况信息
    xTaskCreatePinnedToCore(weather_task,"weather",4096,NULL,2,NULL,1);
}





//挂载图片
void init_weaher_img_buffer()
{
    esp_vfs_spiffs_conf_t conf = 
    {
        .base_path  = "/img",//该类型为SPIFF
        .format_if_mount_failed = false,//读取失败时不清除空间
        .max_files = 3,//最大挂载数为3
        .partition_label = "img",//写NULL会自动寻csv匹配的空间

    };
    esp_vfs_spiffs_register(&conf);
}