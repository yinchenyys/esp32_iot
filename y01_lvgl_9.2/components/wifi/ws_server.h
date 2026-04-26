#ifndef _WS_SERVER__
#define _WS_SWRVER__

#include "esp_err.h"
#include "esp_http_server.h"
#include "esp_log.h"

typedef void(*ws_receive_cb)(uint8_t* payload , int len);

typedef struct 
{
    const char * html_code;
    ws_receive_cb receive_fn;
}ws_cfg_t;

esp_err_t web_ws_start(ws_cfg_t *cfg );
esp_err_t web_ws_stop(void);
esp_err_t web_ws_send(uint8_t *data,int len);

#endif // !_WS_SERVER__
