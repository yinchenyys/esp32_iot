#include "button.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include <stdio.h>
#include <string.h>
static const char* TAG = "button";

typedef enum
{
    BUTTON_RELEASE,             //按键没有按下
    BUTTON_PRESS,               //按键按下了，等待一点延时（消抖），然后触发短按回调事件，进入BUTTON_HOLD
    BUTTON_HOLD,                //按住状态，如果时间长度超过设定的超时计数，将触发长按回调函数，进入BUTTON_LONG_PRESS_HOLD
    BUTTON_LONG_PRESS_HOLD,     //此状态等待电平消失，回到BUTTON_RELEASE状态
}BUTTON_STATE;

typedef struct Button
{
    button_config_t btn_cfg;    //按键配置
    BUTTON_STATE    state;      //当前状态
    int press_cnt;              //按下计数
    struct Button* next;        //下一个按键参数
}button_dev_t;

//按键处理列表
static button_dev_t *s_button_head = NULL;

//消抖过滤时间
#define FILITER_TIMER   20

//定时器释放运行标志
static bool g_is_timer_running = false;

//定时器句柄
static esp_timer_handle_t g_button_timer_handle;

static void button_handle(void *param);

/** 设置按键事件
 * @param cfg   配置结构体
 * @return ESP_OK or ESP_FAIL 
*/
esp_err_t button_event_set(button_config_t *cfg)
{
    gpio_config_t gpio_t = 
    {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ull<<cfg->gpio_num),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };



    //1.配置gpio gpio_config（多次调用该函数可以多次创建按键）
    ESP_ERROR_CHECK(gpio_config(&gpio_t));
    button_dev_t* btn = (button_dev_t*)malloc(sizeof(button_dev_t));//创建btn并分配了button_dev_t大小的button_dev_t
    if(!btn)
        return ESP_FAIL;
    memset(btn,0,sizeof(button_dev_t));//给btn全写入0
    if(!s_button_head)//如果类表头没有东西，把新建的btn作为类表头
    {
        s_button_head = btn;//列表头
        ESP_LOGI(TAG,"first btn add");
    }
    else
    {
        button_dev_t* btn_p = s_button_head;
        while(btn_p->next != NULL)
            btn_p = btn_p->next;//找到最后一个列表，把新建的按键放上去
        btn_p->next = btn;
        ESP_LOGI(TAG,"btn add");
    }
    memcpy(&btn->btn_cfg,cfg,sizeof(button_config_t));//把传递进来的配置内容保护好


    //2.先判断是否已经创建定时函数，否则创建esp_timer_create，并启动esp_timer_start_periodic
    if (false == g_is_timer_running) {//如果第一次进来，还没有创建定时器
        ESP_LOGI(TAG,"run button timer");
        static int timer_interval = 5;

        esp_timer_create_args_t button_timer;
        button_timer.arg = (void*)timer_interval;
        button_timer.callback = button_handle;              //创建定时处理函数
        button_timer.dispatch_method = ESP_TIMER_TASK;
        button_timer.name = "button_handle";                //定时器名字
        esp_timer_create(&button_timer, &g_button_timer_handle);
        
        esp_timer_start_periodic(g_button_timer_handle,  5000);//5000us进入一次
        g_is_timer_running = true;
    }


    return ESP_OK;
}

static void button_handle(void *param)
{
    int increase_cnt = (int)param;
    button_dev_t* btn_target = s_button_head;
    for(;btn_target;btn_target = btn_target->next)//遍历每一个按键(好处：简洁有条理，但不方便组合按键)
    {
        switch(btn_target->state)
        {
            case BUTTON_RELEASE:             //上一个状态：按键没有按下
                if(gpio_get_level(btn_target->btn_cfg.gpio_num) == btn_target->btn_cfg.active_level)//检测到目标电平
                {
                    btn_target->press_cnt += increase_cnt;//加5ms，为什么这里要加，不应该置0吗
                    //￥我觉得这里可以加上一个松开函数
                    btn_target->state = BUTTON_PRESS;
                }
                break;
            case BUTTON_PRESS:               //上一个状态：按键按下了，等待一点延时（消抖），然后触发短按回调事件，进入BUTTON_HOLD
                if(gpio_get_level(btn_target->btn_cfg.gpio_num) == btn_target->btn_cfg.active_level)
                {
                    btn_target->press_cnt += increase_cnt;
                    if(btn_target->press_cnt >= FILITER_TIMER)//大于消抖时间
                    {
                        ESP_LOGI(TAG,"short press detect");
                        if(btn_target->btn_cfg.short_cb)//有短按函数
                            btn_target->btn_cfg.short_cb();//执行短按函数
                        btn_target->state = BUTTON_HOLD;
                    }
                }
                else
                    btn_target->state = BUTTON_RELEASE;//按键释放（为什么不清0）
                break;
            case BUTTON_HOLD:                //按住状态，如果时间长度超过设定的超时计数，将触发长按回调函数，进入BUTTON_LONG_PRESS_HOLD
                if(gpio_get_level(btn_target->btn_cfg.gpio_num) == btn_target->btn_cfg.active_level)
                {
                    btn_target->press_cnt += increase_cnt;
                    if(btn_target->press_cnt >= btn_target->btn_cfg.long_press_time)
                    {
                        ESP_LOGI(TAG,"long press detect");
                        if(btn_target->btn_cfg.long_cb)
                            btn_target->btn_cfg.long_cb();
                        btn_target->state = BUTTON_LONG_PRESS_HOLD;
                    }
                }
                else
                    btn_target->state = BUTTON_RELEASE;
                break;
            case BUTTON_LONG_PRESS_HOLD:     //此状态等待电平消失，回到BUTTON_RELEASE状态
                if(gpio_get_level(btn_target->btn_cfg.gpio_num) != btn_target->btn_cfg.active_level)
                {
                    btn_target->state = BUTTON_RELEASE;
                    btn_target->press_cnt = 0;
                }
                break;
            default:break;
        }
    }
}
