// #include "freertos/FreeRTOS.h"
// #include <freertos/task.h>
// #include "driver/gpio.h"
// #include "driver/rmt_rx.h"
// #include "esp_log.h"
// #include "esp32/rom/ets_sys.h"


// QueueHandle_t dth11_message;

// rmt_channel_handle_t dth11_ret_chan;

// uint8_t dth11_GPIO_NUM;


// bool IRAM_ATTR rmt_dth11_rx_done_callback(rmt_channel_handle_t rx_chan, const rmt_rx_done_event_data_t *edata, void *user_ctx)
// {
//     BaseType_t err=0;
//     xQueueSendFromISR(dth11_message,edata,&err);
//     return err==1;

// }

// void dht11_init(uint8_t GPIO_NUM)
// {
//     dth11_GPIO_NUM=GPIO_NUM;
//     rmt_rx_channel_config_t dth11_config = 
//     {
//         .clk_src=RMT_CLK_SRC_DEFAULT,
//         .flags.invert_in=0,
//         .flags.with_dma=0,
//         .gpio_num=dth11_GPIO_NUM,
//         .mem_block_symbols=64,
//         .resolution_hz=1000*1000
//     };
//     rmt_new_rx_channel(&dth11_config,&dth11_ret_chan);

//     rmt_rx_event_callbacks_t cbs=
//     {
//         .on_recv_done=rmt_dth11_rx_done_callback
//     };


//     dth11_message=xQueueCreate(20,sizeof(rmt_rx_done_event_data_t));
//     assert(dth11_message);

//     rmt_rx_register_event_callbacks(dth11_ret_chan,&cbs,NULL);

//     rmt_enable(dth11_ret_chan);
// }









//  bool dth11_data_deal(rmt_symbol_word_t* data,int num,uint16_t *demp,uint16_t *temp)
// {
//     uint8_t i=0,check_save=0;
//     uint16_t demp_save=0,temp_save=0,duration=0;

//     if(num<41) return 0;

//     if(num>41) data++;

//     for(i=0;i<16;i++,data++)
//     {
//         duration=0;
//         if(data->level0)
//             duration = data->duration0;
//         else 
//             duration = data->duration1;
//         demp_save = (demp_save << 1) + (duration < 35 ? 0:1);
//     }

//     for(i=0;i<16;i++,data++)
//     {
//         duration=0;
//         if(data->level0)
//             duration = data->duration0;
//         else 
//             duration = data->duration1;
//         temp_save = (temp_save<<1) + (duration < 35 ? 0:1);
//     }


//     for(i=0;i<8;i++,data++)
//     {
//         duration=0;
//         if(data->level0)
//             duration = data->duration0;
//         else 
//             duration = data->duration1;
//         check_save = (check_save<<1) + (duration < 35 ? 0:1);
//     }

//     if((((temp_save>>8)+temp_save+demp_save+(demp_save>>8))&0xFF)  != check_save)
//     {
//         ESP_LOGI("receive","Checksum failure %4X %4X %2X\n", temp_save, demp_save, check_save);
//         return 0;
//     }


//     demp_save=demp_save>>8;
//     temp_save=(temp_save>>8)*10+ (temp_save&0xFF);

//     if(demp_save < 100)
//         *demp=demp_save;
//     if(temp_save<600)
//         *temp=temp_save;
//     return 1;
    

// }


// bool dht11_start(uint16_t *temp,uint16_t *demp)
// {
//     gpio_set_direction(dth11_GPIO_NUM,GPIO_MODE_OUTPUT);
//     gpio_set_level(dth11_GPIO_NUM,1);
//     ets_delay_us(1000);
//     //vTaskDelay(1000);

//     gpio_set_level(dth11_GPIO_NUM,0);
//     ets_delay_us(20000);
//     //vTaskDelay(20000);

//     gpio_set_level(dth11_GPIO_NUM,1);
//     ets_delay_us(20);
//     //vTaskDelay(20);

//     gpio_set_direction(dth11_GPIO_NUM,GPIO_MODE_INPUT);
//     gpio_set_pull_mode(dth11_GPIO_NUM,GPIO_PULLUP_ONLY);

//     rmt_receive_config_t config=
//     {
//         .signal_range_max_ns=1000*1000,
//         .signal_range_min_ns=100,
//     };
//    static rmt_symbol_word_t raw_symbols[128];	//接收缓存,可接收128位
//     rmt_receive(dth11_ret_chan,raw_symbols,sizeof(raw_symbols),&config);


  
//      rmt_rx_done_event_data_t data;
//     if(xQueueReceive(dth11_message,&data,pdMS_TO_TICKS(1000)) == pdTRUE)
//     {
//        return dth11_data_deal(data.received_symbols,data.num_symbols,demp,temp);
//     }
//     return 0;

// }



/******************************************第二遍************************************************************ */


// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_log.h"
// #include "driver/gpio.h"
// #include "driver/rmt_rx.h"
// #include "esp32/rom/ets_sys.h"

// uint8_t dth11_gpio;

// rmt_channel_handle_t dth11_ret_chan;

// QueueHandle_t dth11_handle;


// bool IRAM_ATTR rmt_dth11_rx_done_callback(rmt_channel_handle_t rx_chan, const rmt_rx_done_event_data_t *edata, void *user_ctx)
// {
//     BaseType_t error=0;
//     xQueueSendFromISR(dth11_handle,edata,&error);
//     return error==1;

// }


// void dht11_init(uint8_t num)
// {
//     dth11_gpio=num;
//     rmt_rx_channel_config_t config=
//     {
//       .clk_src=RMT_CLK_SRC_DEFAULT,
//       .flags.invert_in=0,
//       .flags.with_dma=0,
//       .gpio_num=dth11_gpio,
//       .mem_block_symbols=64,
//       .resolution_hz=1000*1000,
//     };

//     rmt_rx_event_callbacks_t cbs=
//     {
//         .on_recv_done=rmt_dth11_rx_done_callback
//     };

//     dth11_handle = xQueueCreate(20,sizeof(rmt_rx_done_event_data_t));
//     assert(dth11_handle);

//     rmt_new_rx_channel(&config,&dth11_ret_chan);
//     rmt_rx_register_event_callbacks(dth11_ret_chan,&cbs,NULL);
//     rmt_enable(dth11_ret_chan);
// }





// bool dth11_data_deal(rmt_symbol_word_t *received_data,size_t num, uint16_t* temp,uint16_t* demp)
// {
//     uint8_t i=0,save_cheak=0;
//     uint16_t save_demp=0,save_temp=0,dartion=0;

//     if(num<42) return 0;
//     if(num>=42) received_data++;

//     //由于我们并没有告诉rmt，什么是低数据位，什么是高数据位，所以仅通过时间判断高低电平
//     for(i=0;i<16;i++,received_data++)
//     {
//         dartion=0;
//         if(received_data->level0)
//             dartion=received_data->duration0;
//         else
//             dartion=received_data->duration1;
//         save_demp=(save_demp << 1)+(dartion >35? 1:0);
//     }


//     for(i=0;i<16;i++,received_data++)
//     {
//         dartion=0;
//         if(received_data->level0)
//             dartion=received_data->duration0;
//         else
//             dartion=received_data->duration1;
//         save_temp=(save_temp << 1)+(dartion >35? 1:0);
//     }


//     for(i=0;i<8;i++,received_data++)
//     {
//         dartion=0;
//         if(received_data->level0)
//             dartion=received_data->duration0;
//         else
//             dartion=received_data->duration1;
//         save_cheak=(save_cheak << 1)+(dartion >35? 1:0);
//     }

//     if((((save_temp >> 8) + save_temp + (save_demp >>  8) + save_demp) & 0xFF) != save_cheak )//错在>>写成<<
//     {
//         ESP_LOGI("dth11","data error");
//         return 0;
//     }
        

//     *temp=(save_temp>>8)*10 + (save_temp & 0xFF) ;
//     *demp=save_demp>>8;

//     return 1;


// }


// bool dht11_start(uint16_t* temp, uint16_t *demp )
// {
//     gpio_set_direction(dth11_gpio,GPIO_MODE_OUTPUT);
//     gpio_set_level(dth11_gpio,1);
//     ets_delay_us(1000);

//     gpio_set_level(dth11_gpio,0);
//     ets_delay_us(20000);

//     gpio_set_level(dth11_gpio,1);
//     ets_delay_us(20);
    
//     gpio_set_direction(dth11_gpio,GPIO_MODE_INPUT);
//     gpio_set_pull_mode(dth11_gpio,GPIO_PULLUP_ONLY);

//     rmt_receive_config_t config = 
//     {
//         .signal_range_max_ns=1000*1000,
//         .signal_range_min_ns=100,
//     };

//     static rmt_symbol_word_t rmt_receive_buf[128];
//     rmt_receive(dth11_ret_chan,rmt_receive_buf,sizeof(rmt_receive_buf),&config);

//     rmt_rx_done_event_data_t data;
//     if(xQueueReceive(dth11_handle,&data,pdMS_TO_TICKS(1000)) == pdTRUE )
//     {
//        return  dth11_data_deal(data.received_symbols,data.num_symbols,temp,demp);  
//     }
//     return 0;

// }




///////////////////////////////        第三遍        /////////////////////////////////////////////////////
// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/rmt_rx.h"
// #include "esp_log.h"
// #include "esp32/rom/ets_sys.h"

// uint8_t dth11_GPIO;
// rmt_channel_handle_t dth11_ret_chan;
// QueueHandle_t xQueue_handle;



// bool IRAM_ATTR rmt_rx_done_callback(rmt_channel_handle_t rx_chan, const rmt_rx_done_event_data_t *edata, void *user_ctx)
// {
//     BaseType_t error=0;
//     xQueueSendFromISR(xQueue_handle,edata,&error);
//     return error==1;
// }

// void dht11_init(uint8_t num)
// {
//     dth11_GPIO=num;
//     rmt_rx_channel_config_t config = 
//     {
//         .clk_src=RMT_CLK_SRC_DEFAULT,
//         .mem_block_symbols = 64,          // 内存块大小，即 64 * 4 = 256 字节
//         .flags.invert_in=0,
//         .flags.with_dma=0,
//         .gpio_num=dth11_GPIO,
//         .resolution_hz=1000000,
//     };
//     rmt_new_rx_channel(&config,&dth11_ret_chan);

//     rmt_rx_event_callbacks_t cbs = 
//     {
//         .on_recv_done=rmt_rx_done_callback,
//     };
//     rmt_rx_register_event_callbacks(dth11_ret_chan,&cbs,NULL);

//     xQueue_handle = xQueueCreate(20,sizeof(rmt_rx_done_event_data_t));
//     assert(xQueue_handle);

//     rmt_enable(dth11_ret_chan);

// }



// bool dth11_data_deal(uint16_t* temp,uint16_t* hemp)
// {
//     uint8_t i=0,dth11_check=0;
//     uint16_t duration=0,dth11_temp=0,dth11_hemp=0;
//     ESP_LOGI("dth11","dth11_data_deal");

//     rmt_rx_done_event_data_t  rx_data;
//     if(xQueueReceive(xQueue_handle, &rx_data,pdMS_TO_TICKS(10000))== pdTRUE)
//     {
//         ESP_LOGI("dth11","xQueueReceive get");
//         if( rx_data.num_symbols<41) return 1;
//         if( rx_data.num_symbols>41)  rx_data.received_symbols++;
   
//         for(i=0;i<16;i++, rx_data.received_symbols++)
//         {
//             duration=0;
//             if( rx_data.received_symbols->level0) duration= rx_data.received_symbols->duration0;
//             else duration= rx_data.received_symbols->duration1;
//             dth11_hemp=(dth11_hemp<<1)+(duration>35?1:0);
//         }
//         ESP_LOGI("dth11","dth11_hemp is %x",dth11_hemp);

//         for(i=0;i<16;i++, rx_data.received_symbols++)
//         {
//             duration=0;
//             if( rx_data.received_symbols->level0) duration= rx_data.received_symbols->duration0;
//             else duration= rx_data.received_symbols->duration1;
//             dth11_temp=(dth11_temp<<1)+(duration>35?1:0);
//         }
//         ESP_LOGI("dth11","dth11_temp is %x",dth11_temp);

//         for(i=0;i<8;i++, rx_data.received_symbols++)
//         {
//             duration=0;
//             if( rx_data.received_symbols->level0) duration= rx_data.received_symbols->duration0;
//             else duration= rx_data.received_symbols->duration1;
//             dth11_check=(dth11_check<<1)+(duration>35?1:0);
//         }
//         ESP_LOGI("dth11","dth11_check is %x",dth11_check);

//         if((((dth11_temp>>8)+dth11_temp+(dth11_hemp>>8)+dth11_hemp) & 0xFF) != dth11_check) 
//         {
//             ESP_LOGI("dth11","all is %x",(((dth11_temp<<8)+dth11_temp+(dth11_hemp<<8)+dth11_hemp)& 0xFF));
//             ESP_LOGI("dth11","dth11_check error");
//            return 1; 
//         }
//         *temp = (dth11_temp>>8)*10 + (dth11_temp & 0xFF);
//         *hemp = dth11_hemp>>8;
//         return 0;
//     }
//     else return 1;
// }


// bool dht11_start(uint16_t* temp,uint16_t* hemp)
// {
//     gpio_set_direction(dth11_GPIO,GPIO_MODE_OUTPUT);
//     gpio_set_level(dth11_GPIO,1);
//     ets_delay_us(1000);

//     gpio_set_level(dth11_GPIO,0);
//     ets_delay_us(20000);

//     gpio_set_level(dth11_GPIO,1);
//     ets_delay_us(20);

//     gpio_set_direction(dth11_GPIO,GPIO_MODE_INPUT);
//     gpio_set_pull_mode(dth11_GPIO,GPIO_PULLUP_ONLY);

//     static rmt_symbol_word_t dth11_buff[128];
//     rmt_receive_config_t config = 
//     {
//         .signal_range_max_ns=1000*1000,
//         .signal_range_min_ns=100,
//     };
//     rmt_receive(dth11_ret_chan,dth11_buff,sizeof(dth11_buff),&config);


//     if(dth11_data_deal(temp,hemp)==0)
//     return 1;
//     else return 0;

// }






///////////////////////////////       第四遍        ////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "driver/rmt_rx.h"
#include "esp_log.h"
#include "esp32/rom/ets_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

rmt_channel_handle_t dht11_ret_chan;
uint8_t dth11_GPIO;
QueueHandle_t dth11_queue_handle;


bool IRAM_ATTR dth11_rmt_rx_done_callback(rmt_channel_handle_t rx_chan, const rmt_rx_done_event_data_t *edata, void *user_ctx)
{
    BaseType_t error=0;
    xQueueSendFromISR(dth11_queue_handle,edata,&error);
    return error == 1;
}


void dht11_init(uint8_t num)
{
    dth11_GPIO = num;
    rmt_rx_channel_config_t config =
    {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .flags.invert_in=0,
        .flags.with_dma=0,
        .gpio_num=dth11_GPIO,
        .resolution_hz=1000 * 1000,
        .mem_block_symbols=64,

    };
    rmt_new_rx_channel(&config,&dht11_ret_chan);
    

    rmt_rx_event_callbacks_t cbs =
    {
        .on_recv_done=dth11_rmt_rx_done_callback,
    };
    rmt_rx_register_event_callbacks(dht11_ret_chan,&cbs,NULL);

    dth11_queue_handle = xQueueCreate(20,sizeof(rmt_rx_done_event_data_t));
    assert(dth11_queue_handle);


    rmt_enable(dht11_ret_chan);

}




bool dth11_data_deal(rmt_symbol_word_t *received_symbols,size_t num_symbols,uint16_t* temp,uint16_t* hemp)
{
    uint8_t i=0,dth11_check=0;
    uint16_t dth11_temp=0,dth11_hemp=0,durtion=0;
    if (num_symbols<41) return 0;
    if (num_symbols>41) received_symbols++;

    for(i=0;i<16;i++,received_symbols++)
    {
        durtion=0;
        if(received_symbols->level0) durtion=received_symbols->level0;
        else durtion=received_symbols->level1;
        dth11_hemp = (dth11_hemp<<1)+(durtion>35?1:0);
    }

    for(i=0;i<16;i++,received_symbols++)
    {
        durtion=0;
        if(received_symbols->level0) durtion=received_symbols->level0;
        else durtion=received_symbols->level1;
        dth11_temp = (dth11_temp<<1)+(durtion>35?1:0);
    }

    for(i=0;i<8;i++,received_symbols++)
    {
        durtion=0;
        if(received_symbols->level0) durtion=received_symbols->level0;
        else durtion=received_symbols->level1;
        dth11_check = (dth11_check<<1)+(durtion>35?1:0);
    }

    if( (((dth11_hemp>>8)+dth11_hemp + dth11_temp + (dth11_temp>>8)) & 0xFF) != dth11_check ) 
    {
        ESP_LOGI("dth11","cheack err");
        return 0;
    }

    ESP_LOGI("dth11","cheack ok");
    *temp = (dth11_temp >>8)*10 + (dth11_temp & 0xff);
    *hemp = dth11_hemp >> 8;

    return 1;
}


bool dht11_start(uint16_t* temp,uint16_t* hemp)
{
    gpio_set_direction(dth11_GPIO,GPIO_MODE_OUTPUT);
    gpio_set_level(dth11_GPIO,1);
    ets_delay_us(1000);

    gpio_set_level(dth11_GPIO,0);
    ets_delay_us(20000);

    gpio_set_level(dth11_GPIO,1);
    ets_delay_us(20);

    gpio_set_direction(dth11_GPIO,GPIO_MODE_INPUT);
    gpio_set_pull_mode(dth11_GPIO,GPIO_PULLUP_ONLY);

    rmt_receive_config_t config =
    {
        .signal_range_max_ns=1000*1000,
        .signal_range_min_ns=100,
    };

    static rmt_symbol_word_t buff[128] ;
    rmt_receive(dht11_ret_chan,buff,sizeof(buff),&config);

    rmt_rx_done_event_data_t data;
    
    if(xQueueReceive(dth11_queue_handle,&data,pdMS_TO_TICKS(1000))==1)
    {
        ESP_LOGI("dth11","get ok");
        return dth11_data_deal(data.received_symbols,data.num_symbols,temp,hemp);
    }
    else return 0;




}