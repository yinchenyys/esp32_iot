
#include "ap_wifi.h"

#define SPIFF_MOUNT "/spiffs"               //挂载点
#define HTML_PATH   "/spiffs/apcfg.html"    //存储路径（取与目录相同名字）

static char * html_code  = NULL;

static EventGroupHandle_t  apcfg_ev = NULL;

static char current_ssid[32];
static char current_password[64];

#define TAG "ap_wifi"


void ap_wifi_task( void *param )//任务函数，但收到wifi信息就进行连接
{
    EventBits_t ev;
    while(1)
    {
        ev = xEventGroupWaitBits(apcfg_ev,BIT1,pdTRUE,pdFALSE,pdMS_TO_TICKS(10000));
        if(ev & BIT1)
        {
            web_ws_stop();
            wifi_manager_connect(current_ssid,current_password);
            
        }
    }

}

static char *init_web_page_buffer()
{
    esp_vfs_spiffs_conf_t conf = 
    {
        .base_path  = SPIFF_MOUNT,//该类型为SPIFF
        .format_if_mount_failed = false,//读取失败时不清除空间
        .max_files = 3,//最大挂载数为3
        .partition_label = "html",//写NULL会自动寻csv匹配的空间

    };
    esp_vfs_spiffs_register(&conf);

    struct stat st;//创建一个读取文件的结构体
    if(stat(HTML_PATH,&st))//当返回值不为0则表示失败
    {
        ESP_LOGE(TAG, "apcfg.html not found");
        return NULL;
    }
    char * buf = (char *)malloc(st.st_size + 1);
    memset(buf, 0 , st.st_size + 1);//开辟缓存内存区域


    FILE *fp = fopen(HTML_PATH,"r");
    if(fp)
    {
        if(0 == fread(buf, st.st_size,1,fp))//内存区域，大小，多少块，文件指针
        {
            //读取失败
            free(buf);
            buf = NULL;
        }
        fclose(fp);
    }
    else
    {
        free(buf);
        buf = NULL;
    }
    return buf;


}

void ap_wifi_init(p_wifi_state_cb f)
{
    wifi_manager_init(f);
    // wifi_manager_connect("yys","15088145513qq");
    html_code = init_web_page_buffer();
    apcfg_ev = xEventGroupCreate();//创建一个任务，当收到wifi信息进行连接
    xTaskCreatePinnedToCore(ap_wifi_task,"ap_wifi_task",4096,NULL,3,NULL,1);

}

void ap_wifi_connect()
{

}

void wifi_scan_handle(int num, wifi_ap_record_t *ap_records)//填充JSON信息，把收到的wifi填入json
{
    cJSON * root = cJSON_CreateObject();//获得根
    cJSON * wifilist_js = cJSON_AddArrayToObject(root,"wifi_list");
    for(int i = 0 ; i < num ; i++)
    {
        cJSON * wifi_js = cJSON_CreateObject();
        cJSON_AddStringToObject(wifi_js , "ssid" , (char*)ap_records[i].ssid);//填入字符串
        cJSON_AddNumberToObject(wifi_js, "rssi" , ap_records[i].rssi);//填入数值
        if(ap_records[i].authmode == WIFI_AUTH_OPEN)
            cJSON_AddBoolToObject(wifi_js,"encrypted",0);//填入bool
        else
            cJSON_AddBoolToObject(wifi_js,"encrypted",1);//填入bool
        cJSON_AddItemToArray(wifilist_js,wifi_js);
    }
    char * data = cJSON_Print(root);
    ESP_LOGI(TAG,"ws send : %s", data);
    web_ws_send((uint8_t *)data,strlen(data));
    cJSON_free(data);//释放的是根节点就会递归地释放
    cJSON_Delete(root);
}



static void ws_receive_handle(uint8_t * payload , int len)
{
    cJSON* root = cJSON_Parse((char*)payload);//payload什么时候传输
    if(root)
    {
        cJSON * scan_js = cJSON_GetObjectItem(root , "scan");//获取，root这大类中，scan的值
        cJSON * ssid_js = cJSON_GetObjectItem(root , "ssid");
        cJSON * password_js = cJSON_GetObjectItem(root , "password");
        if(scan_js)
        {
            char * scan_value = cJSON_GetStringValue(scan_js);
            if(strcmp(scan_value, "start") == 0)
            {
                wifi_manager_scan(wifi_scan_handle);
            }
        }
        if(ssid_js && password_js)
        {
            char * ssid_value = cJSON_GetStringValue(ssid_js);
            char * password_value = cJSON_GetStringValue(password_js);//记录收到的wifi密码
            snprintf(current_ssid , sizeof(current_ssid), "%s" , ssid_value);//转换为字符串保存
            snprintf(current_password , sizeof(current_password), "%s" , password_value );
            xEventGroupSetBits(apcfg_ev,BIT1); 
        }
    }
    else
    {
        ESP_LOGE(TAG,"Receive invaild json");
    }
}

void ap_wifi_apcfg()
{
    wifi_manager_ap();
    ws_cfg_t ws_cfg = 
    {
        .html_code = html_code,
        .receive_fn = ws_receive_handle,
    };
    web_ws_start(&ws_cfg);
}