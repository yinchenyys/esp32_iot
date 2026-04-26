#include "wifi_manager.h"


#define EXAMPLE_ESP_MAXIMUM_RETRY 10
static int s_retry_num=0;

#define TAG "wifi_manager"

static p_wifi_state_cb wifi_callback = NULL;
static bool wifi_state = false;//WiFi状态
esp_netif_t *wifi_netif_ap;

#define esp_ssid "esp"          //开发板发出的名称
#define esp_password "11111111"//开发板发出的密码

static SemaphoreHandle_t Sem_frist = NULL;


static void event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT ) 
    {
        switch (event_id)
        {
        case WIFI_EVENT_STA_START://sta启动成功
            esp_wifi_connect();
            break;      
        case WIFI_EVENT_STA_DISCONNECTED:
            if(wifi_state)
            {
                if(wifi_callback)
                {
                    wifi_callback(WIFI_STATE_DISCONNECT);
                } 
                wifi_state = false ;
            }
            if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) 
            {
                esp_wifi_connect();
                s_retry_num++;
                ESP_LOGI(TAG, "retry to connect to the AP");
            } 
            break; 
        case WIFI_EVENT_STA_CONNECTED:
            break;                                    
        case WIFI_EVENT_AP_STACONNECTED:
            ESP_LOGI(TAG,"connect to the AP succes");
            break;                                    
        case WIFI_EVENT_AP_STADISCONNECTED:
            ESP_LOGI(TAG,"connect to the AP fail");
            break;                                    
        default:
            break;
        }
        
    } 
    else if (event_base == IP_EVENT ) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        wifi_state = true ;
        if(wifi_callback)//如果函数存在调用函数
        {
            wifi_callback(WIFI_STATE_CONNECTED);
        }
    }
}



void wifi_manager_init(p_wifi_state_cb f)
{
    
    ESP_ERROR_CHECK(esp_netif_init());//TCP/IP协议的初始化

    ESP_ERROR_CHECK(esp_event_loop_create_default());//创建调度循环系统
    esp_netif_create_default_wifi_sta();//创建默认sta，底层绑定到wifi接口，将相关的wifi STA事件函数注册得到事件循环中
    wifi_netif_ap = esp_netif_create_default_wifi_ap();//创建默认ap，底层绑定到wifi接口，将相关的wifi STA事件函数注册得到事件循环中
    



    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));//初始化整个wifi

    esp_event_handler_instance_t instance_any_id;//up:其实我们不用，这是用来取消相应的事件
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));//wifi事件回调函数，ESP_EVENT_ANY_ID所有事件都触发
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));//IP事件回调函数，IP_EVENT_STA_GOT_IP获取ip后触发

    wifi_callback=f;//传入main写的函数
    Sem_frist = xSemaphoreCreateBinary();
    xSemaphoreGive(Sem_frist);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );//设置工作模式
    ESP_ERROR_CHECK(esp_wifi_start() );//启动wifi

}

wifi_mode_t mode;

void wifi_manager_connect(const char* wifi_ssid,const char* wifi_password)
{
    wifi_config_t wifi_config = 
    {
        .sta = 
        {
            // .ssid = EXAMPLE_ESP_WIFI_SSID,
            // .password = EXAMPLE_ESP_WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK,//加密方式
        },
    };
    snprintf((char*)wifi_config.sta.ssid,32,"%s",wifi_ssid);
    snprintf((char*)wifi_config.sta.password,63,"%s",wifi_password);

    esp_wifi_get_mode(&mode);//获取当前工作模式
    if(mode !=  WIFI_MODE_STA)
    {
        esp_wifi_stop();
        esp_wifi_set_mode(WIFI_MODE_STA);
    }
    s_retry_num = 0 ;
    ESP_LOGI(TAG, "wifi_init_sta finished.");
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );//设置进去
    ESP_ERROR_CHECK(esp_wifi_start() );//启动wifi

}




esp_err_t wifi_manager_ap(void)
{
    esp_wifi_get_mode(&mode);
    if(mode == WIFI_MODE_APSTA)
    {
        return ESP_OK;
    }
    esp_wifi_disconnect();
    esp_wifi_stop();
    esp_wifi_set_mode(WIFI_MODE_APSTA);
    wifi_config_t wifi_ap_conf =
     {
        .ap=
        {
            .channel=5,//通道范围为5
            .max_connection=2,//最大连接数为2
            .authmode=WIFI_AUTH_WPA2_PSK,//加密方式
            .password=esp_password,
            .ssid=esp_ssid,
            .ssid_len=strlen(esp_ssid),
        }
    };
    esp_wifi_set_config(WIFI_IF_AP,&wifi_ap_conf);

    esp_netif_ip_info_t ipInfo;
    IP4_ADDR(&ipInfo.ip,192,168,100,1);         //IP地址
    IP4_ADDR(&ipInfo.gw,192,168,100,1);         //网关
    IP4_ADDR(&ipInfo.netmask,255,255,255,0);    //子网掩码

    esp_netif_dhcps_stop(wifi_netif_ap);//修改ip地址前要先关闭
    esp_netif_set_ip_info(wifi_netif_ap,&ipInfo);
    esp_netif_dhcps_start(wifi_netif_ap);

    return esp_wifi_start();
}


static void scan_task(void *param)
{
    p_wifi_scan_cb callback = (p_wifi_scan_cb)param;
    uint16_t ap_count = 0;
    uint16_t ap_num = 0;
    wifi_ap_record_t *ap_list = (wifi_ap_record_t *)malloc(sizeof(wifi_ap_record_t)*ap_num);
    esp_wifi_scan_start(NULL,true);//NULL表示为默认配置,ture表示打开阻塞
    esp_wifi_scan_get_ap_num(&ap_count);//记录扫描出来的个数
    esp_wifi_scan_get_ap_records(&ap_num,ap_list);//记录扫描到的热点数到ap_list,最大数不会超过ap_num,如果少于ap_num,则把数量赋值给ap_num;
    ESP_LOGI(TAG,"get ap num is %d ", ap_num);
    if(callback)
    {
        callback(ap_num,ap_list);
    }
    free(ap_list);
    xSemaphoreGive(Sem_frist);
    vTaskDelete(NULL);

}


esp_err_t wifi_manager_scan(p_wifi_scan_cb f)//这里不用定义为指针函数吗
{
    if(pdTRUE == xSemaphoreTake(Sem_frist,0))
    {
        esp_wifi_clear_ap_list();
        return xTaskCreatePinnedToCore(scan_task,"wifi_scan",8192,f,3,NULL,1);
    }
    return 0;
}


bool wifi_manager_is_connect(void)
{
    return wifi_state;
}