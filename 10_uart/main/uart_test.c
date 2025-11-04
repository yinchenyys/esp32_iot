// #include <stdio.h>
// #include "driver/gpio.h"
// #include "driver/uart.h"
// #include "esp_log.h"

// QueueHandle_t uart_queue;

// uint8_t buf[1024];

// void app_main(void)
// {
//     uart_driver_install(UART_NUM_2,1024,1024,20,&uart_queue,0);
//     uart_config_t uart_config =
//     {
//         .baud_rate=115200,
//         .data_bits=UART_DATA_8_BITS,
//         .flow_ctrl=UART_HW_FLOWCTRL_DISABLE,//无硬件流控制
//         //.rx_flow_ctrl_thresh=,
//         .source_clk=UART_SCLK_DEFAULT,
//         .stop_bits=UART_STOP_BITS_1

//     };
//     uart_param_config(UART_NUM_2,&uart_config);
//     uart_set_pin(UART_NUM_2,GPIO_NUM_32,GPIO_NUM_33,UART_PIN_NO_CHANGE,UART_PIN_NO_CHANGE);

//     while(1)
//     {
//         int ret=uart_read_bytes(UART_NUM_2,buf,1024,pdMS_TO_TICKS(2000));
//         if(ret)
//         {
//             uart_write_bytes(UART_NUM_2,buf,ret);
//         }
//     }

// }


#include <stdio.h>
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"

QueueHandle_t uart_queue;

uint8_t buff[1024];

void app_main(void)
{
    uart_driver_install(UART_NUM_2,1024,1024,20,&uart_queue,0);
    uart_config_t uart_config =
    {
        .baud_rate=115200,
        .data_bits=UART_DATA_8_BITS,
        .flow_ctrl=UART_HW_FLOWCTRL_DISABLE,
        .stop_bits=UART_STOP_BITS_1,
        .source_clk=UART_SCLK_DEFAULT,
        .parity=UART_PARITY_DISABLE
    };
    uart_param_config(UART_NUM_2,&uart_config);
    uart_set_pin(UART_NUM_2,GPIO_NUM_32,GPIO_NUM_33,UART_PIN_NO_CHANGE,UART_PIN_NO_CHANGE);

    while(1)
    {
        int ret = uart_read_bytes(UART_NUM_2,buff,1024,pdMS_TO_TICKS(1000));
        if(ret)
        {
            uart_write_bytes(UART_NUM_2,buff,ret);
        }
    }


}