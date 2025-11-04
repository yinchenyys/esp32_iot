// #include "simple_wifi_sta.h"
// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "nvs_flash.h"
// #include "nvs.h"

// #include "freertos/event_groups.h"
// #include "esp_wifi.h"
// #include "esp_event.h"
// #include "esp_log.h"

// //需要把这两个修改成你家WIFI，测试是否连接成功
// #define DEFAULT_WIFI_SSID           "esp"
// #define DEFAULT_WIFI_PASSWORD       "15088145513"

// static const char *TAG = "wifi";



// /** 事件回调函数
//  * @param arg   用户传递的参数
//  * @param event_base    事件类别
//  * @param event_id      事件ID
//  * @param event_data    事件携带的数据
//  * @return 无
// */
// static void event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data)
// {   
//     if(event_base == WIFI_EVENT)
//     {
//         switch (event_id)
//         {
//         case WIFI_EVENT_STA_START:      //WIFI以STA模式启动后触发此事件
//             esp_wifi_connect();         //启动WIFI连接（连接内容为下面主函数wifi_config的配置）
//             break;
//         case WIFI_EVENT_STA_CONNECTED:  //WIFI连上路由器后，触发此事件
//             ESP_LOGI(TAG, "connected to AP");
//             break;
//         case WIFI_EVENT_STA_DISCONNECTED:   //WIFI从路由器断开连接后触发此事件
//             esp_wifi_connect();             //继续重连
//             ESP_LOGI(TAG,"connect to the AP fail,retry now");
//             break;
//         default:
//             break;
//         }
//     }
//     if(event_base == IP_EVENT)                  //IP相关事件
//     {
//         switch(event_id)
//         {
//             case IP_EVENT_STA_GOT_IP:           //只有获取到路由器分配的IP，才认为是连上了路由器
//                 ESP_LOGI(TAG,"get ip address");
//                 break;
//         }
//     }
// }


// //WIFI STA初始化
// esp_err_t wifi_sta_init(void)
// {   
//     //这里没有nvs_init
//     ESP_ERROR_CHECK(esp_netif_init());                      //1.用于初始化tcpip协议栈esp_netif_init
//     ESP_ERROR_CHECK(esp_event_loop_create_default());       //2.创建一个默认系统事件调度循环，之后可以注册回调函数来处理系统的一些事件esp_event_loop_create_default   (和 xTaskCreatePinnedToCore有什么区别)
//     esp_netif_create_default_wifi_sta();                    //3.使用默认配置创建STA对象esp_netif_create_default_wifi_sta

//     //4.初始化WIFI esp_wifi_init
//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
//     //5.注册事件esp_event_handler_register
//     ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&event_handler,NULL));//wifi事件，ESP_EVENT_ANY_ID所有都监听
//     ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,IP_EVENT_STA_GOT_IP,&event_handler,NULL));//网络事件，IP_EVENT_STA_GOT_IP连接到路由器获取到ip地址就会触发

//     //WIFI，sta配置
//     wifi_config_t wifi_config = 
//     { 
//         .sta = 
//         { 
//             .ssid = DEFAULT_WIFI_SSID,                 //WIFI的SSID
//             .password = DEFAULT_WIFI_PASSWORD,         //WIFI密码
// 	        .threshold.authmode = WIFI_AUTH_WPA2_PSK,   //加密方式
            
//             .pmf_cfg = 
//             {
//                 .capable = true,                         //启用保护管理帧
//                 .required = false                        //是否只是和有保护管理帧设备通信
//             },
//         },
//     };
    
//     //启动WIFI
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );                 //6.设置工作模式为STA  esp_wifi_set_mode
//     ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );   //7.设置wifi配置       esp_wifi_set_config
//     ESP_ERROR_CHECK(esp_wifi_start() );                                 //8.启动WIFI           esp_wifi_start
    
//     ESP_LOGI(TAG, "wifi_init_sta finished.");
//     return ESP_OK;
// }



/////////////////////////////////////////////////////    练习      /////////////////////////////////////////////////////////////////////
// #include <stdio.h>
// #include "simple_wifi_sta.h"
// #include "esp_wifi.h"
// #include "esp_log.h"
// #include "esp_event.h"
// #include "esp_err.h"
// #include "nvs_flash.h"
// #include "esp_smartconfig.h"
// #include "string.h"

// #define DEFAULT_WIFI_SSID           "esp"
// #define DEFAULT_WIFI_PASSWORD       "15088145513"

// static const char *TAG = "wifi";

// static QueueHandle_t SemBIN;


// static void event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data)
// {   
//     if(event_base == WIFI_EVENT)
//     {
//         switch (event_id)
//         {
//         case WIFI_EVENT_STA_START:      //WIFI以STA模式启动后触发此事件
//             esp_wifi_connect();         //启动WIFI连接（连接内容为下面主函数wifi_config的配置）
//             break;
//         case WIFI_EVENT_STA_CONNECTED:  //WIFI连上路由器后，触发此事件
//             ESP_LOGI(TAG, "connected to AP");
//             break;
//         case WIFI_EVENT_STA_DISCONNECTED:   //WIFI从路由器断开连接后触发此事件
//             esp_wifi_connect();             //继续重连
//             ESP_LOGI(TAG,"connect to the AP fail,retry now");
//             break;
//         default:
//             break;
//         }
//     }
//     if(event_base == IP_EVENT)                  //IP相关事件
//     {
//         switch(event_id)
//         {
//             case IP_EVENT_STA_GOT_IP:           //只有获取到路由器分配的IP，才认为是连上了路由器
//                 // xSemaphoreGive(SemBIN);
//                 ESP_LOGI(TAG,"get ip address");
//                 break;
//         }
//     }
//     if(event_base == SC_EVENT )//智能云
//     {
//         switch (event_id)
//         {
//             case SC_EVENT_SCAN_DONE:
//                 ESP_LOGI(TAG,"sc scan done");
//                 break;
//             case SC_EVENT_GOT_SSID_PSWD:
//             {
//                 smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;//直接赋值，而非取出
//                 wifi_config_t wifi_config = {0};
//                 memset(&wifi_config,0,sizeof(wifi_config));
//                 snprintf((char*)wifi_config.sta.ssid,sizeof(wifi_config.sta.ssid),"%s",(char*)evt->ssid);//这个应该是一个写入函数，为什么要用这个
//                 snprintf((char*)wifi_config.sta.password,sizeof(wifi_config.sta.password),"%s",(char*)evt->password);//这个应该是一个写入函数，为什么要用这个
//                 wifi_config.sta.bssid_set=evt->bssid_set;
//                 if( wifi_config.sta.bssid_set)
//                     memcpy(wifi_config.sta.bssid,evt->bssid,6);
//                 esp_wifi_disconnect();//断开连接
//                 esp_wifi_set_config(WIFI_IF_STA,&wifi_config);
//                 esp_wifi_connect();//重新连接
//                 break;
//             }
//             case SC_EVENT_SEND_ACK_DONE://连接完成
//             {
//                 esp_smartconfig_stop();
//                 break;
//             }
//             default:
//                 break;
//         }

//     }

// }


// esp_err_t wifi_sta_init(void)
// {
//     // SemBIN = xSemaphoreCreateBinary();
//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());//这个不能放在(esp_netif_create_default_wifi_sta之后)，不然会崩毁
//     ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&event_handler,NULL));//wifi事件，ESP_EVENT_ANY_ID所有都监听
//     ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,IP_EVENT_STA_GOT_IP,&event_handler,NULL));//网络事件，IP_EVENT_STA_GOT_IP连接到路由器获取到ip地址就会触发
//     ESP_ERROR_CHECK(esp_event_handler_register(SC_EVENT,ESP_EVENT_ANY_ID,&event_handler,NULL));//智能云
    
//     esp_netif_create_default_wifi_sta();



//     wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&config));
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
   

//     wifi_config_t wifi_conf = 
//         {
//         .sta=
//         {
//             .password=DEFAULT_WIFI_PASSWORD,
//             .ssid=DEFAULT_WIFI_SSID,
//             .threshold=
//             {
//                 .authmode=WIFI_AUTH_WPA2_PSK,
//             },
//             .pmf_cfg=
//             {
//                 .capable=true,//是否启用管理帧
//                 .required=false,//是否仅和有管理帧的设备通信
//             }
//         }
//     };
//     ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA,&wifi_conf));
//     ESP_ERROR_CHECK( esp_wifi_start());

    

//     //开启智能云
//     // esp_smartconfig_set_type(SC_TYPE_ESPTOUCH);
//     // smartconfig_start_config_t cs_cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
//     // esp_smartconfig_start(&cs_cfg);


//     // xSemaphoreTake(SemBIN,portMAX_DELAY);
//     return ESP_OK;
// }



/////////////////////////////////////////////////////    练习      /////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "esp_smartconfig.h"
#include "string.h"
#include "simple_wifi_sta.h"

#define DEFAULT_WIFI_SSID           "esp"
#define DEFAULT_WIFI_PASSWORD       "15088145513"

static const char *TAG = "wifi";

static QueueHandle_t SemBIN;
// static wifi_event_cb  wifi_cb = NULL;

static void event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data)
{   
    if(event_base == WIFI_EVENT)
    {
        switch (event_id)
        {
        case WIFI_EVENT_STA_START:      //WIFI以STA模式启动后触发此事件
            esp_wifi_connect();         //启动WIFI连接（连接内容为下面主函数wifi_config的配置）
            break;
        case WIFI_EVENT_STA_CONNECTED:  //WIFI连上路由器后，触发此事件
            ESP_LOGI(TAG, "connected to AP");
            break;
        case WIFI_EVENT_STA_DISCONNECTED:   //WIFI从路由器断开连接后触发此事件
            esp_wifi_connect();             //继续重连
            ESP_LOGI(TAG,"connect to the AP fail,retry now");
            break;
        default:
            break;
        }
    }
    if(event_base == IP_EVENT)                  //IP相关事件
    {
        switch(event_id)
        {
            case IP_EVENT_STA_GOT_IP:           //只有获取到路由器分配的IP，才认为是连上了路由器
                ESP_LOGI(TAG,"get ip address");
                xSemaphoreGive(SemBIN);
                // if(wifi_cb)
                //     wifi_cb(IP_EVENT_STA_GOT_IP);
                break;
        }
    }
    if(event_base == SC_EVENT )
    {
        switch (event_id)
        {
            case SC_EVENT_SCAN_DONE:
                ESP_LOGI(TAG,"sc scan done");
                break;
            case SC_EVENT_GOT_SSID_PSWD:
            {
                smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;//直接赋值，而非取出
                wifi_config_t wifi_config = {0};
                memset(&wifi_config,0,sizeof(wifi_config));
                snprintf((char*)wifi_config.sta.ssid,sizeof(wifi_config.sta.ssid),"%s",(char*)evt->ssid);//这个应该是一个写入函数，为什么要用这个
                snprintf((char*)wifi_config.sta.password,sizeof(wifi_config.sta.password),"%s",(char*)evt->password);//这个应该是一个写入函数，为什么要用这个
                wifi_config.sta.bssid_set=evt->bssid_set;
                if( wifi_config.sta.bssid_set)
                    memcpy(wifi_config.sta.bssid,evt->bssid,6);
                esp_wifi_disconnect();//断开连接
                esp_wifi_set_config(WIFI_IF_STA,&wifi_config);
                esp_wifi_connect();//重新连接
                break;
            }
            case SC_EVENT_SEND_ACK_DONE://连接完成
            {
                esp_smartconfig_stop();
                break;
            }
            default:
                break;
        }

    }

}


esp_err_t wifi_sta_init(void)
{
    SemBIN = xSemaphoreCreateBinary();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());//这个不能放在(esp_netif_create_default_wifi_sta之后)，不然会崩毁
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&event_handler,NULL));//wifi事件，ESP_EVENT_ANY_ID所有都监听
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,IP_EVENT_STA_GOT_IP,&event_handler,NULL));//网络事件，IP_EVENT_STA_GOT_IP连接到路由器获取到ip地址就会触发
    ESP_ERROR_CHECK(esp_event_handler_register(SC_EVENT,ESP_EVENT_ANY_ID,&event_handler,NULL));
    
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&config));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    wifi_config_t wifi_conf = 
        {
        .sta=
        {
            .password=DEFAULT_WIFI_PASSWORD,
            .ssid=DEFAULT_WIFI_SSID,
            .threshold=
            {
                .authmode=WIFI_AUTH_WPA2_PSK,
            },
            .pmf_cfg=
            {
                .capable=true,//是否启用管理帧
                .required=false,//是否仅和有管理帧的设备通信
            }
        }
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA,&wifi_conf));

    ESP_ERROR_CHECK( esp_wifi_start());

    // esp_smartconfig_set_type(SC_TYPE_ESPTOUCH);
    // smartconfig_start_config_t cs_cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
    // esp_smartconfig_start(&cs_cfg);


    xSemaphoreTake(SemBIN,portMAX_DELAY);

    return ESP_OK;
}