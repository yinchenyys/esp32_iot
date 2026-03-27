// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/ledc.h"

// #define GPIO_LED GPIO_NUM_27

// #define FULL_EV_BIT  BIT0
// #define EMPTY_EV_BIT BIT1

// static EventGroupHandle_t ledc_event_handle;

// bool IRAM_ATTR ledc_finish_cb(const ledc_cb_param_t *param, void *user_arg)//中断里面调用的，所以调用IRAM_ATTR，放在内存，而不是flash
// {
//     BaseType_t taskWoken;
//     if(param->duty)//占空比是什么
//     {
//         //第三个函数是否有更高优先级就绪
//         xEventGroupSetBitsFromISR(ledc_event_handle,FULL_EV_BIT,&taskWoken);//中断版本的事件
//     }
//     else
//     {
//         xEventGroupSetBitsFromISR(ledc_event_handle,EMPTY_EV_BIT,&taskWoken);//中断版本的事件
//     }

//     return taskWoken;

// }

// void Task_LED_TUEN(void *param)
// { 
//     EventBits_t ev;
//     while(1)
//     {
//         ev = xEventGroupWaitBits(ledc_event_handle,FULL_EV_BIT|EMPTY_EV_BIT,pdTRUE,pdFALSE,pdMS_TO_TICKS(5000));
//         if(ev & FULL_EV_BIT)
//         {
//             ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,0,2000);//2000毫秒内，渐变成8191
//             ledc_fade_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,LEDC_FADE_NO_WAIT);//最后一个参数，表示，不用等待，立刻返回

//         }
//         if(ev & EMPTY_EV_BIT)
//         {
//             ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,8191,2000);//2000毫秒内，渐变成8191
//             ledc_fade_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,LEDC_FADE_NO_WAIT);//最后一个参数，表示，不用等待，立刻返回
//         }


//         ledc_cbs_t cbs =
//         {
//             .fade_cb=ledc_finish_cb,
    
//         };//渐变完成回调函数
    
//         ledc_cb_register(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,&cbs,NULL);
   
//     }
// }


// void app_main(void)
// {
//     //1.初始化gpio_config
//     gpio_config_t led_cfg =
//     {
//         .pull_up_en=GPIO_PULLUP_DISABLE,
//         .pull_down_en=GPIO_PULLUP_DISABLE,
//         .pin_bit_mask=(1<<GPIO_LED),
//         .intr_type=GPIO_INTR_DISABLE,
//         .mode=GPIO_MODE_DEF_OUTPUT //这个不知有什么用
//     };
//     gpio_config(&led_cfg);


//     //2.初始化定时器ledc_timer_config
//     ledc_timer_config_t ledc_timer=//配置定时器
//     {
//         .speed_mode =LEDC_LOW_SPEED_MODE,//低速模式
//         .timer_num = LEDC_TIMER_0, //使用哪一个定时器
//         .clk_cfg=LEDC_AUTO_CLK,//选择时钟源
//         .duty_resolution=LEDC_TIMER_13_BIT,//精确度
//         .freq_hz = 5000,
//         .deconfigure=0,//是否重新配置？
//     };
//     ledc_timer_config(&ledc_timer);


//     //3.初始化通道ledc_channel_config
//     ledc_channel_config_t ledc_channel=
//     {
//         .speed_mode=LEDC_LOW_SPEED_MODE,
//         .channel=LEDC_CHANNEL_0,//8个通道，选0
//         .timer_sel=LEDC_TIMER_0,//定时器
//         .gpio_num=GPIO_LED,//哪一个引脚
//         .duty=0,//初始化占空比
//         .intr_type=LEDC_INTR_DISABLE,
//     };
//     ledc_channel_config(&ledc_channel);



//     //4.开启PWM
//     ledc_fade_func_install(0);//开启pwm模块工作
//     ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,8191,2000);//开启渐变：2000毫秒内，渐变成8191
//     ledc_fade_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,LEDC_FADE_NO_WAIT);//最后一个参数，表示，不用等待渐变，继续往下执行



   



//     //5.创建渐变回调函数ledc_cb_register（注意：因为该函数为中断函数，所以要加上 IRAM_ATTR 表示放在内核中运行，且事件标志set用中断的set）
//     ledc_cbs_t cbs =
//     {
//         .fade_cb=ledc_finish_cb,

//     };//渐变完成回调函数
//     ledc_cb_register(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,&cbs,NULL);//注册函数，渐变完成调用函数


//     //6.创建消息队列告诉渐变函数渐变完成
//     ledc_event_handle = xEventGroupCreate();//通过消息句柄完成通信

//     //7.创建渐变函数
//     xTaskCreatePinnedToCore(Task_LED_TUEN,"Task_LED_TUEN",2048,NULL,3,NULL,1);

// }




/******************************************   第二遍    ********************************************** */
// #include "stdio.h"
// #include "driver/gpio.h"
// #include "driver/ledc.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/event_groups.h"
// #include "freertos/task.h"




// EventGroupHandle_t even_handle;



// bool IRAM_ATTR ledc_cb_t_use(const ledc_cb_param_t *param, void *user_arg)
// {
//     BaseType_t pxHigherPriority;//是否有优先级等待
//     if(param->duty)
//     {
//       xEventGroupSetBitsFromISR(even_handle,BIT1,&pxHigherPriority);  
//     }
//     else
//       xEventGroupSetBitsFromISR(even_handle,BIT0,&pxHigherPriority);
      
//     return pxHigherPriority;
// }



// void ledc_ture(void* param)
// {
//     EventBits_t ev;
//     while(1)
//     {
//         ev=xEventGroupWaitBits(even_handle,BIT0|BIT1,true,false,pdMS_TO_TICKS(3000));
//         if(ev&BIT0)
//         {
//             ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,8191,2000);
//             ledc_fade_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,LEDC_FADE_NO_WAIT);

//         }
//         else if(ev&BIT1)
//         {
//             ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,0,2000);
//             ledc_fade_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,LEDC_FADE_NO_WAIT);
//         }


//         ledc_cbs_t ledc_cbs=
//         {
//             .fade_cb=ledc_cb_t_use
//         };
//         ledc_cb_register(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,&ledc_cbs,NULL);
//     }

// }






// void app_main(void)
// {
//     //这个不影响
//     // gpio_config_t gpio_con=
//     // {
//     //     .intr_type=GPIO_INTR_DISABLE,
//     //     .mode=GPIO_MODE_OUTPUT,
//     //     .pin_bit_mask=GPIO_NUM_27,
//     //     .pull_down_en=GPIO_PULLDOWN_DISABLE,
//     //     .pull_up_en=GPIO_PULLDOWN_DISABLE,
//     // };
//     // gpio_config(&gpio_con);

//     ledc_timer_config_t ledc_time_con=
//     {
//         .clk_cfg=LEDC_AUTO_CLK,//时钟源
//         .deconfigure=0,//是否取消之前的配置，是1则不亮
//         .duty_resolution=LEDC_TIMER_13_BIT,
//         .freq_hz=5000,
//         .speed_mode=LEDC_LOW_SPEED_MODE,
//         .timer_num=LEDC_TIMER_0,
//     };
//     ledc_timer_config(&ledc_time_con);


//     ledc_channel_config_t ledc_channel_con =
//     {
//         .channel=LEDC_CHANNEL_0,
//         .duty=0,
//         //.flags=1,
//         .gpio_num=GPIO_NUM_27,
//         //.hpoint=8191,
//         .intr_type=LEDC_INTR_DISABLE,
//         .speed_mode=LEDC_LOW_SPEED_MODE,
//         .timer_sel=LEDC_TIMER_0

//     };
//     ledc_channel_config(&ledc_channel_con);



//     ledc_fade_func_install(0);//为0开启
//     ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,0,2000);
//     ledc_fade_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,LEDC_FADE_NO_WAIT);


//     ledc_cbs_t ledc_cbs=
//     {
//         .fade_cb=ledc_cb_t_use
//     };
//     ledc_cb_register(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,&ledc_cbs,NULL);


//     even_handle=xEventGroupCreate();

//     xTaskCreatePinnedToCore(ledc_ture,"led",2048,NULL,3,NULL,1);


// }


/************************************** 第三遍    ********************************************************* */
// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/event_groups.h"
// #include "driver/gpio.h"
// #include "driver/ledc.h"
// #include "driver/timer.h"
// #include "esp_log.h"


// EventGroupHandle_t event_handle;



// bool IRAM_ATTR ledc_cb_t_use(const ledc_cb_param_t *param, void *user_arg)
// {
//     BaseType_t BaseIns;
//     if(param->duty)
//     {
//         xEventGroupSetBitsFromISR(event_handle,BIT1,&BaseIns);
//     }

//     else 
//     {
//         xEventGroupSetBitsFromISR(event_handle,BIT0,&BaseIns);
//     }
//     return BaseIns;
// }

// void TaskA(void *param)
// {
//     EventBits_t EventBit;
//     while (1)
//     {
//         EventBit = xEventGroupWaitBits(event_handle,BIT0|BIT1,true,false,5000);
//         if(EventBit == BIT1)
//         {
//             ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,0,2000);
//             ledc_fade_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,LEDC_FADE_NO_WAIT);
//         }
//         else if(EventBit == BIT0)
//         {
//             ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,8192,2000);
//             ledc_fade_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,LEDC_FADE_NO_WAIT);
//         }


//         ledc_cbs_t ledc_cb =
//         {
//             .fade_cb=ledc_cb_t_use
//         };
//         ledc_cb_register(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,&ledc_cb,NULL);
//         /* code */
//     }
    

// }



// void app_main(void)
// {
//     ledc_timer_config_t ledc_timer_con =
//     {
//         .clk_cfg=LEDC_AUTO_CLK,
//         .deconfigure=0,
//         .freq_hz=5000,
//         .duty_resolution=LEDC_TIMER_13_BIT,
//         .speed_mode=LEDC_LOW_SPEED_MODE,
//         .timer_num=LEDC_TIMER_0,

//     };
//     ledc_timer_config(&ledc_timer_con);


//     ledc_channel_config_t ledc_channel_con =
//     {
//         .channel=LEDC_CHANNEL_0,
//         .duty=0,
//         //.flags=0,
//         .gpio_num=GPIO_NUM_27,
//         .intr_type=LEDC_INTR_DISABLE,
//         .speed_mode=LEDC_LOW_SPEED_MODE,
//         .timer_sel=LEDC_CHANNEL_0,

//     };
//     ledc_channel_config(&ledc_channel_con);


//     ledc_fade_func_install(0);//ESP_INTR_FLAG_中断优先级？
//     ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,0,2000);
//     ledc_fade_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,LEDC_FADE_NO_WAIT);

//     ledc_cbs_t ledc_cb =
//     {
//         .fade_cb=ledc_cb_t_use
//     };

//     ledc_cb_register(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,&ledc_cb,NULL);


//     event_handle = xEventGroupCreate();
//     xTaskCreatePinnedToCore(TaskA,"TaskA",2048,NULL,3,NULL,1);



// }


/*****************************************第四遍********************************************************* */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "button.h"
#include "esp_log.h"

EventGroupHandle_t Event_handle;

#define SHORT_EV    BIT0    //短按
#define LONG_EV     BIT1    //长按
#define BTN_GPIO    GPIO_NUM_39
#define LED_GPIO    GPIO_NUM_27





bool IRAM_ATTR  ledc_cb_t_use(const ledc_cb_param_t *param, void *user_arg)
{
    BaseType_t xReturn;
    if(param->duty)
    {
        xEventGroupSetBitsFromISR(Event_handle,BIT1,&xReturn);
    }
    else
    {
        xEventGroupSetBitsFromISR(Event_handle,BIT0,&xReturn);
    }
    return xReturn;
}


void TaskA(void* param)
{
    EventBits_t num;
    while(1)
    {
        num=xEventGroupWaitBits(Event_handle,BIT0|BIT1,1,0,pdTICKS_TO_MS(10000));
        if(num & BIT0)
        {
            //ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,0,8192,pdTICKS_TO_MS(2000));
            ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,8192,2000);
        }
        else if(num & BIT1)
        {
            //ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,8192,0,pdTICKS_TO_MS(2000));
            ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,0,2000);
        }

        ledc_fade_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,LEDC_FADE_NO_WAIT);
        ledc_cbs_t cbs =
        {
            .fade_cb=ledc_cb_t_use
        };
        ledc_cb_register(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,&cbs,NULL);

    }

}


void app_main(void)
{

    ledc_channel_config_t ledc_conf =
    {
        .channel=LEDC_CHANNEL_0,
        .duty=0,
        //.flags=,
        .gpio_num=GPIO_NUM_27,
        //.hpoint=0,
        .intr_type=LEDC_INTR_DISABLE,
        .speed_mode=LEDC_LOW_SPEED_MODE,
        .timer_sel=LEDC_TIMER_0,
    };
    ledc_channel_config(&ledc_conf);

    ledc_timer_config_t timer_conf =
    {
        .clk_cfg=LEDC_AUTO_CLK,
        .deconfigure=0,
        .duty_resolution=LEDC_TIMER_13_BIT,
        .freq_hz=5000,
        .speed_mode=LEDC_LOW_SPEED_MODE,
        .timer_num=LEDC_TIMER_0,
    };
    ledc_timer_config(&timer_conf);

    ledc_fade_func_install(0);
    ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,8192,2000);
    ledc_fade_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,LEDC_FADE_NO_WAIT);

    ledc_cbs_t cbs =
    {
        .fade_cb=ledc_cb_t_use
    };
    ledc_cb_register(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,&cbs,NULL);

    Event_handle = xEventGroupCreate();
    xTaskCreatePinnedToCore(TaskA,"TaskA",2048,NULL,3,NULL,1);

}

//================================================================================================

// 单独设置GPIO控制灯亮
// void short_press_handle(void)
// {
//     // gpio_set_level(LED_GPIO,gpio_get_level(LED_GPIO)?0:1);
//     if(gpio_get_level(LED_GPIO)==0)
//         gpio_set_level(LED_GPIO,1);
//     else
//         gpio_set_level(LED_GPIO,0);
//     ESP_LOGI("LED","led level is %d",gpio_get_level(LED_GPIO));
// }


// void long_press_handle(void)
// {
//     ESP_LOGI("LED","get long press");
// }

// void app_main(void)
// {
//     //加入按键配置
//     button_config_t btn_cfg = 
//     {
//         .gpio_num = BTN_GPIO,       //gpio号
//         .active_level = 0,          //按下的电平
//         .long_press_time = 3000,    //长按时间
//         .short_cb = short_press_handle,           //短按回调函数
//         .long_cb = long_press_handle             //长按回调函数
//     };
//     button_event_set(&btn_cfg);     //添加按键响应事件处理


//     gpio_config_t led_gpio_t = 
//     {
//         .intr_type = GPIO_INTR_DISABLE,
//         .mode = GPIO_MODE_OUTPUT,
//         .pin_bit_mask = (1ull<<LED_GPIO),
//         .pull_down_en = GPIO_PULLDOWN_DISABLE,
//         .pull_up_en = GPIO_PULLUP_DISABLE,
//     };
//     ESP_ERROR_CHECK(gpio_config(&led_gpio_t));

//      gpio_set_level(LED_GPIO,0);
// }


//================================================================================================




//单独控制小的亮度不渐变
// esp_err_t bsp_display_brightness_init(void)//设置单通道亮度示例
// {
//     // Setup LEDC peripheral for PWM backlight control
//     const ledc_channel_config_t LCD_backlight_channel = {
//         .gpio_num = GPIO_NUM_27,//哪一个GPIO口
//         .speed_mode = LEDC_LOW_SPEED_MODE,
//         .channel = LCD_LEDC_CH,//想怎么强大的pwn通道，他们还有7个
//         .intr_type = LEDC_INTR_DISABLE,
//         .timer_sel = 1,
//         .duty = 0,
//         .hpoint = 0,
//         .flags.output_invert = true
//     };
//     const ledc_timer_config_t LCD_backlight_timer = {
//         .speed_mode = LEDC_LOW_SPEED_MODE,
//         .duty_resolution = LEDC_TIMER_10_BIT,
//         .timer_num = 1,
//         .freq_hz = 5000,
//         .clk_cfg = LEDC_AUTO_CLK
//     };

//     ESP_ERROR_CHECK(ledc_timer_config(&LCD_backlight_timer));
//     ESP_ERROR_CHECK(ledc_channel_config(&LCD_backlight_channel));

//     return ESP_OK;
// }


// esp_err_t bsp_display_brightness_set(int brightness_percent)
// {
//     if (brightness_percent > 100) {
//         brightness_percent = 100;
//     } else if (brightness_percent < 0) {
//         brightness_percent = 0;
//     }

//     ESP_LOGI(TAG, "Setting LCD backlight: %d%%", brightness_percent);
//     // LEDC resolution set to 10bits, thus: 100% = 1023
//     uint32_t duty_cycle = (1023 * brightness_percent) / 100;
//     ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH, duty_cycle));
//     ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH));

//     return ESP_OK;
// }

// void app_main(void)
// {
//      bsp_display_brightness_init();
//       bsp_display_brightness_set(50);
// }