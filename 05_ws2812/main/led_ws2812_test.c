


/////////////////////////////             化简(有问题，还无法使用)            ////////////////////////////////////////////////////////
 


#include "esp_check.h"
#include "led_ws2812.h"
#include "driver/rmt_tx.h"

static const char *TAG = "led_encoder";

#define LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz 分辨率, 也就是1tick = 0.1us，也就是可以控制的最小时间单元，低于0.1us的脉冲无法产生


//WS2812驱动的描述符
    rmt_channel_handle_t handle_led_chan;          //rmt通道
    rmt_encoder_handle_t handle_led_encoder;       //rmt编码器
    uint8_t *handle_led_buffer;                    //rgb数据
    int handle_led_num;                            //led个数


    rmt_encoder_t led_encoder_base;                     //编码器，里面包含三个需要用户实现的回调函数，encode,del,ret
    rmt_encoder_t *led_encoder_bytes_encoder;           //字节编码器，调用rmt_new_bytes_encoder函数后创建
    rmt_encoder_t *led_encoder_copy_encoder;            //拷贝编码器，调用rmt_new_copy_encoder函数后创建
    int led_encoder_state;                              //状态控制
    rmt_symbol_word_t led_encoder_reset_code;           //结束位的时序



size_t sw_2812_encode(rmt_encoder_t *encoder, rmt_channel_handle_t tx_channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state)
{
    rmt_encoder_handle_t bytes_encoder   = led_encoder_bytes_encoder;        //取出字节编码器
    rmt_encoder_handle_t copy_encoder    = led_encoder_copy_encoder;          //取出拷贝编码器
    rmt_encode_state_t session_state     = RMT_ENCODING_RESET;
    rmt_encode_state_t state             = RMT_ENCODING_RESET;
    size_t encoded_symbols = 0;//记录encode编码成功返回个数
    switch (led_encoder_state) {   //led_encoder_state是自定义的状态，这里只有两种值，0是发送RGB数据，1是发送复位码
    case 0: // send RGB data
        //￥对数据进行编码，即RNT符号，完成状态写到session_state
        encoded_symbols += bytes_encoder->encode(bytes_encoder, tx_channel, primary_data, data_size, &session_state);//encode由内部提供，返回编码成功个数，编码状态保存到session_state
        if (session_state & RMT_ENCODING_COMPLETE) {    //字节编码完成
            led_encoder_state = 1; //￥下次执行跳到 case 1
        }
        if (session_state & RMT_ENCODING_MEM_FULL) {    //缓存不足，本次退出
            state |= RMT_ENCODING_MEM_FULL;
            goto out; // yield if there's no free space for encoding artifacts
        }

    // fall-through
    case 1: // send reset code
        //运行拷贝编码器,内容放在自定义的.mem_block_symbols = 64
        encoded_symbols += copy_encoder->encode(copy_encoder, tx_channel, &led_encoder_reset_code,
                                                sizeof(led_encoder_reset_code), &session_state);
        if (session_state & RMT_ENCODING_COMPLETE) {
            led_encoder_state = RMT_ENCODING_RESET; // back to the initial encoding session
            state |= RMT_ENCODING_COMPLETE;
        }
        if (session_state & RMT_ENCODING_MEM_FULL) {
            state |= RMT_ENCODING_MEM_FULL;
            goto out; // yield if there's no free space for encoding artifacts
        }
    }
out:
    *ret_state = state;
    return encoded_symbols;

}

esp_err_t sw_2812_del(rmt_encoder_t *encoder)
{
    rmt_del_encoder(led_encoder_bytes_encoder);
    rmt_del_encoder(led_encoder_copy_encoder);
    return ESP_OK;

}


esp_err_t sw_2812_reset(rmt_encoder_t *encoder)
{
    rmt_encoder_reset(led_encoder_bytes_encoder);
    rmt_encoder_reset(led_encoder_copy_encoder);
    led_encoder_state = RMT_ENCODING_RESET;
    return ESP_OK;

}


























//从代码上看，前面不需要也不影响点灯
esp_err_t ws2812_init(gpio_num_t gpio,int maxled)
{
   handle_led_num=maxled;

   rmt_tx_channel_config_t rmt_tx_channel_config=
   {
    .clk_src=RMT_CLK_SRC_DEFAULT,
    // .flags=,
    .gpio_num=gpio,
    // .intr_priority=,
    .mem_block_symbols=64,//转化完的存储空间
    .resolution_hz=LED_STRIP_RESOLUTION_HZ,//最小为0.1us了
    .trans_queue_depth=4,//堆栈深度
   };
    rmt_new_tx_channel(&rmt_tx_channel_config,&handle_led_chan);

    rmt_bytes_encoder_config_t rmt_bytes_encoder_con=
    {
        .bit0=
        {//bit0的传输过程
            .level0=1,
            .duration0=30,

            .level1=0,
            .duration1=90,
        },

        .bit1=
        {//bit1的传输过程
            .level0=1,
            .duration0=90,

            .level1=0,
            .duration1=30,
        },

        .flags.msb_first=1//先传BIT1
    };
    rmt_new_bytes_encoder(&rmt_bytes_encoder_con,&led_encoder_bytes_encoder);//led_encoder_bytes_encoder:字节编码器句柄，里面有个encode函数进行编码

    rmt_copy_encoder_config_t rmt_copy_encoder_con={};//这个函数有什么用
    rmt_new_copy_encoder(&rmt_copy_encoder_con,&led_encoder_copy_encoder);//led_encoder_copy_encoder:拷贝编码器，里面有个encode函数进行编码reset

    uint32_t reset_ticks = LED_STRIP_RESOLUTION_HZ / 1000000 * 50 / 2;
    led_encoder_reset_code=(rmt_symbol_word_t)//￥一定要有这个（刚bit1和bit0的类型）
    {
            .level0=1,
            .duration0=reset_ticks,
            .level1=0,
            .duration1=reset_ticks,
    };


    /*有啥用（rmt_transmit发送接口需要）*/
    led_encoder_base.encode=sw_2812_encode;
    led_encoder_base.del=sw_2812_del;
    led_encoder_base.reset=sw_2812_reset;
    handle_led_encoder = &led_encoder_base;//要求定义三个函数，与linux相似


    rmt_enable(handle_led_chan);

    return ESP_OK;
}



/** 反初始化WS2812外设
 * @param handle 初始化的句柄
 * @return ESP_OK or ESP_FAIL
*/
// esp_err_t ws2812_deinit(ws2812_strip_handle_t handle)
// {
//     if(!handle)
//         return ESP_OK;
//     rmt_del_encoder(handle_led_encoder);
//     if(handle_led_buffer)
//         free(handle_led_buffer);
//     free(handle);
//     return ESP_OK;
// }





/** 向某个WS2812写入RGB数据
 * @param handle 句柄
 * @param index 第几个WS2812（0开始）
 * @param r,g,b RGB数据
 * @return ESP_OK or ESP_FAIL
*/
esp_err_t ws2812_write(uint32_t index,uint32_t r,uint32_t g,uint32_t b)
{
     rmt_transmit_config_t tx_config = {
        .loop_count = 0, //不循环发送
    };
    if(index >= handle_led_num)
        return ESP_FAIL;
    uint32_t start = index*3;
    handle_led_buffer[start+0] = g & 0xff;     //注意，WS2812的数据顺序时GRB
    handle_led_buffer[start+1] = r & 0xff;
    handle_led_buffer[start+2] = b & 0xff;

    //￥RMT发送接口
    return rmt_transmit(handle_led_chan, handle_led_encoder, handle_led_buffer, handle_led_num*3, &tx_config);
    //会自己找到handle_led_encoder中的sw_2812_encode进行解码操作
    
}

