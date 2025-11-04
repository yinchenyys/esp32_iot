// #include <stdio.h>
// #include "mqtt_client.h"
// #include "esp_log.h"
// #include "simple_wifi_sta.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_wifi.h"
// // #include "simple_wifi_sta.h"

// #include "esp_event.h"
// #include "esp_err.h"
// #include "nvs_flash.h"
// #include "esp_smartconfig.h"
// #include "string.h"
// #include "nvs_flash.h"

// #define DEFAULT_WIFI_SSID           "esp"
// #define DEFAULT_WIFI_PASSWORD       "15088145513"

// #define MQTT_ADDRESS_URI "mqtt://broker-cn.emqx.io"
// #define MQTT_ADDRESS_PORT 1883
// #define MQTT_CLIENT_ID "mqttx_f7b9e566666"
// #define MQTT_USER_NAME "yys"
// #define MQTT_USER_PASSWORD "123456"

// //两个主题名
// #define MQTT_TOPIC1 "/topic/esp32"  //esp32推送的主题
// #define MQTT_TOPIC2 "/topic/mqtt"   //esp32订阅的主题

// #define TAG "mqtt"

// static QueueHandle_t SemBIN;

// esp_mqtt_client_handle_t esp_mqtt_client_handle;


// // void wifi_event_handler(ip_event_t ev)
// // {
// //     if(ev == IP_EVENT_STA_GOT_IP)
// //     {
// //             xSemaphoreTake(SemBIN,portMAX_DELAY);
// //     }
// // }


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
//                 xSemaphoreGive(SemBIN);
//                 break;
//         }
//     }
//     if(event_base == SC_EVENT )
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
//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());//这个不能放在(esp_netif_create_default_wifi_sta之后)，不然会崩毁
//     ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&event_handler,NULL));//wifi事件，ESP_EVENT_ANY_ID所有都监听
//     ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,IP_EVENT_STA_GOT_IP,&event_handler,NULL));//网络事件，IP_EVENT_STA_GOT_IP连接到路由器获取到ip地址就会触发
//     ESP_ERROR_CHECK(esp_event_handler_register(SC_EVENT,ESP_EVENT_ANY_ID,&event_handler,NULL));
    
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

//     // esp_smartconfig_set_type(SC_TYPE_ESPTOUCH);
//     // smartconfig_start_config_t cs_cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
//     // esp_smartconfig_start(&cs_cfg);



//     return ESP_OK;
// }




// static void esp_mqtt_event(void* event_handler_arg,
//                                         esp_event_base_t event_base,
//                                         int32_t event_id,
//                                         void* event_data)
// { 
//     esp_mqtt_event_handle_t data = (esp_mqtt_event_handle_t)event_data;
//     switch (event_id)
//     {
//     case MQTT_EVENT_CONNECTED://连接到MQTT
//         ESP_LOGI(TAG,"MQTT_EVENT_CONNECTED");
//         esp_mqtt_client_subscribe_single(esp_mqtt_client_handle,MQTT_TOPIC2,1);//订阅一个主题,使用QS1
//         break;
//     case MQTT_EVENT_DISCONNECTED://断开MQTT
//         ESP_LOGI(TAG,"MQTT_EVENT_DISCONNECTED");
//         break;
//     case MQTT_EVENT_PUBLISHED://向MQTT发布消息后，收到回复
//         ESP_LOGI(TAG,"MQTT_EVENT_PUBLISHED");
//         break;
//     case MQTT_EVENT_SUBSCRIBED://向MQTT订阅消息后，收到回复
//         ESP_LOGI(TAG,"MQTT_EVENT_SUBSCRIBED");
//         break;  
//     case MQTT_EVENT_DATA://某设备向MQTT,MQTT转发给esp32,就像别人在Q群发送消息
//         ESP_LOGI(TAG,"MQTT_EVENT_DATA:topic is %s",data->topic);
//         ESP_LOGI(TAG,"MQTT_EVENT_DATA:payload is %s",data->data);
//         break;
        
        
//     default:
//         break;
//     }

// }


// void mqtt_Init(void)
// {
//     //1.esp_mqtt_client_init
//     esp_mqtt_client_config_t config = {0};
//     config.broker.address.uri= MQTT_ADDRESS_URI ;
//     config.broker.address.port=MQTT_ADDRESS_PORT;
//     config.credentials.client_id =MQTT_CLIENT_ID;
//     config.credentials.username=MQTT_USER_NAME;
//     config.credentials.authentication.password = MQTT_USER_PASSWORD;
//     esp_mqtt_client_handle = esp_mqtt_client_init(&config);

//     esp_mqtt_client_register_event(esp_mqtt_client_handle,ESP_EVENT_ANY_ID,esp_mqtt_event,NULL);

//     esp_mqtt_client_start(esp_mqtt_client_handle);

// }

// void app_main(void)
// {
//     nvs_flash_init();
    
//     SemBIN = xSemaphoreCreateBinary();
//     // xSemaphoreTake(SemBIN,portMAX_DELAY);
//     // wifi_sta_init();//调用就会崩溃，应该是什么被释放掉了
//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());//这个不能放在(esp_netif_create_default_wifi_sta之后)，不然会崩毁
//     ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&event_handler,NULL));//wifi事件，ESP_EVENT_ANY_ID所有都监听
//     ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,IP_EVENT_STA_GOT_IP,&event_handler,NULL));//网络事件，IP_EVENT_STA_GOT_IP连接到路由器获取到ip地址就会触发
//     // ESP_ERROR_CHECK(esp_event_handler_register(SC_EVENT,ESP_EVENT_ANY_ID,&event_handler,NULL));
    
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

//     xSemaphoreTake(SemBIN,portMAX_DELAY);
//     mqtt_Init();

//     int count = 0;
//     char count_str[32];
//     while(1)
//     {
//         snprintf(count_str,sizeof(count_str),"{\"count\":%d}",count);
//         esp_mqtt_client_publish(esp_mqtt_client_handle,MQTT_TOPIC1,count_str,strlen(count_str),1,0);
//         count++;
//         vTaskDelay(pdMS_TO_TICKS(2000));
//     }


// }











/////////////////////////////  尝试2  ////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "mqtt_client.h"
#include "esp_log.h"
#include "simple_wifi_sta.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "simple_wifi_sta.h"

// #include "esp_event.h"
// #include "esp_err.h"
// #include "nvs_flash.h"
// #include "esp_smartconfig.h"
// #include "string.h"
#include "nvs_flash.h"

#define DEFAULT_WIFI_SSID           "esp"
#define DEFAULT_WIFI_PASSWORD       "15088145513"

#define MQTT_ADDRESS_URI "mqtt://broker-cn.emqx.io"
#define MQTT_ADDRESS_PORT 1883
#define MQTT_CLIENT_ID "mqttx_f7b9e566666"
#define MQTT_USER_NAME "yys"
#define MQTT_USER_PASSWORD "123456"

//两个主题名
#define MQTT_TOPIC1 "/topic/esp32"  //esp32推送的主题
#define MQTT_TOPIC2 "/topic/mqtt"   //esp32订阅的主题

#define TAG "mqtt"


esp_mqtt_client_handle_t esp_mqtt_client_handle;


// void wifi_event_handler(ip_event_t ev)
// {
//     if(ev == IP_EVENT_STA_GOT_IP)
//     {
//             xSemaphoreTake(SemBIN,portMAX_DELAY);
//     }
// }


static void esp_mqtt_event(void* event_handler_arg,
                                        esp_event_base_t event_base,
                                        int32_t event_id,
                                        void* event_data)
{ 
    esp_mqtt_event_handle_t data = (esp_mqtt_event_handle_t)event_data;
    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED://连接到MQTT
        ESP_LOGI(TAG,"MQTT_EVENT_CONNECTED");
        esp_mqtt_client_subscribe_single(esp_mqtt_client_handle,MQTT_TOPIC2,1);//订阅一个主题,使用QS1
        break;
    case MQTT_EVENT_DISCONNECTED://断开MQTT
        ESP_LOGI(TAG,"MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_PUBLISHED://向MQTT发布消息后，收到回复
        ESP_LOGI(TAG,"MQTT_EVENT_PUBLISHED");
        break;
    case MQTT_EVENT_SUBSCRIBED://向MQTT订阅消息后，收到回复
        ESP_LOGI(TAG,"MQTT_EVENT_SUBSCRIBED");
        break;  
    case MQTT_EVENT_DATA://某设备向MQTT,MQTT转发给esp32,就像别人在Q群发送消息
        ESP_LOGI(TAG,"MQTT_EVENT_DATA:topic is %s",data->topic);
        ESP_LOGI(TAG,"MQTT_EVENT_DATA:payload is %s",data->data);
        break;
        
        
    default:
        break;
    }

}


void mqtt_Init(void)
{
    //1.mqtt初始化 esp_mqtt_client_init
    esp_mqtt_client_config_t config = {0};
    config.broker.address.uri= MQTT_ADDRESS_URI ;
    config.broker.address.port=MQTT_ADDRESS_PORT;
    config.credentials.client_id =MQTT_CLIENT_ID;
    config.credentials.username=MQTT_USER_NAME;
    config.credentials.authentication.password = MQTT_USER_PASSWORD;
    esp_mqtt_client_handle = esp_mqtt_client_init(&config);

    //2.mqtt注册事件 esp_mqtt_client_register_event
    esp_mqtt_client_register_event(esp_mqtt_client_handle,ESP_EVENT_ANY_ID,esp_mqtt_event,NULL);

    //3.mqtt启动
    esp_mqtt_client_start(esp_mqtt_client_handle);

}

void app_main(void)
{
    nvs_flash_init();
    
    // SemBIN = xSemaphoreCreateBinary();
    // xSemaphoreTake(SemBIN,portMAX_DELAY);
    wifi_sta_init();

    // xSemaphoreTake(SemBIN,portMAX_DELAY);
    mqtt_Init();

    int count = 0;
    char count_str[32];
    while(1)
    {
        snprintf(count_str,sizeof(count_str),"{\"count\":%d}",count);
        esp_mqtt_client_publish(esp_mqtt_client_handle,MQTT_TOPIC1,count_str,strlen(count_str),1,0);
        count++;
        vTaskDelay(pdMS_TO_TICKS(2000));
    }


}

