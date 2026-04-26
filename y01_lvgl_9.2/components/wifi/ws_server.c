#include "ws_server.h"

static const char * http_html = NULL;//接收传过来函数的参数

static ws_receive_cb ws_receive_fn = NULL;//接收传过来函数的参数

static httpd_handle_t server_handle =  NULL; 

#define TAG "WS_SERVER"

static int client_fds =-1;


esp_err_t get_http_req(httpd_req_t *r)
{
    return httpd_resp_send(r,http_html,HTTPD_RESP_USE_STRLEN);
}

esp_err_t handle_ws_req(httpd_req_t *r)
{
    if(r-> method == HTTP_GET)
    {
        client_fds = httpd_req_to_sockfd(r);//获取sockfd，发送时会用到
        return ESP_OK;//第一次会有一个get请求，不需要
    }
    httpd_ws_frame_t pkt;
    esp_err_t ret;
    memset(&pkt,0,sizeof(pkt));
    ret = httpd_ws_recv_frame(r,&pkt,0);//由于不知道接收数据的大小，第一次接收空间写0，pkt.len也会返回接收到数据的大小
    if(ret != ESP_OK)
    {
        return ret;
    }
    uint8_t *buf = (uint8_t *)malloc(pkt.len + 1);
    if(buf == NULL)
    {
        return ESP_FAIL;//随便返回一个错误
    }
    pkt.payload = buf;//把空间指向buf
    ret= httpd_ws_recv_frame(r,&pkt,pkt.len);//第二次才真正写入
    if(ret == ESP_OK)//当空间不够会返回错误
    {
        if(pkt.type == HTTPD_WS_TYPE_TEXT)//一般接收的都是txt类型
        {
            ESP_LOGI(TAG, "GET websocket message : %s", pkt.payload);
            if(ws_receive_fn)
            {
                //如果有传入的函数则执行
                ws_receive_fn(pkt.payload,pkt.len);
            }
        }
    }
    free(buf);
    return ESP_OK;


}

esp_err_t web_ws_start(ws_cfg_t *cfg )
{
    if(cfg == NULL) return 0;
    http_html  = cfg->html_code;//接收传过来函数的参数，取出html
    ws_receive_fn = cfg->receive_fn;//接收传过来函数的参数的函数
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();//取得默认配置
    httpd_start(&server_handle,&config);//创建http句柄
    httpd_uri_t uri_get =
    {
        .uri = "/",
        .method = HTTP_GET,
        .handler = get_http_req,
    };
    httpd_register_uri_handler(server_handle, &uri_get);

    httpd_uri_t uri_ws = 
    {
        .uri = "/ws",
        .method = HTTP_GET,
        .handler = handle_ws_req,
        .is_websocket = true, 
    };
    httpd_register_uri_handler(server_handle, &uri_ws);

    return 0 ;
}

esp_err_t web_ws_stop(void)
{
    if(server_handle)
    {
        httpd_stop(server_handle);
        server_handle = NULL;
    }
    return ESP_OK;
}

esp_err_t web_ws_send(uint8_t *data,int len)
{
    httpd_ws_frame_t pkt;
    memset(&pkt,0,sizeof(pkt));
    pkt.payload = data;
    pkt.len = len;
    pkt.type = HTTPD_WS_TYPE_TEXT;
    return httpd_ws_send_data(server_handle,client_fds,&pkt);

}