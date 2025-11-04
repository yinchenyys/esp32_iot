#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ntc.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"



#define TAG     "adc"
//启用ADC2无法使用wifi功能
/*//仅对应的8个IO口能使用ADC功能
GPIO32  ADC1_CH4
GPIO33  ADC1_CH5
GPIO34  ADC1_CH6
GPIO35  ADC1_CH7
GPIO36  ADC1_CH0
GPIO37  ADC1_CH1
GPIO38  ADC1_CH2
GPIO39  ADC1_CH3
*/
#define TEMP_ADC_CHANNEL     ADC_CHANNEL_0      //ADC转换通道(ADC1有8个通道，对应gpio32 - gpio39)

#define NTC_RES             10000               //NTC电阻标称值(在电路中和NTC一起串进电路的那个电阻,一般是10K，100K)
#define ADC_V_MAX           3300                //最大接入电压值

#define ADC_VALUE_NUM       10                  //每次采样的电压

static bool do_calibration1 = false;            //是否需要校准

static volatile float s_temp_value = 0.0f;      //室内温度


static int s_adc_raw[ADC_VALUE_NUM];              //ADC采样值
static int s_voltage_raw[ADC_VALUE_NUM];              //转换后的电压值


typedef struct
{
    int8_t temp;        //温度
    uint32_t res;       //温度对应的阻值
}temp_res_t;

//NTC温度表
static const temp_res_t s_ntc_table[] =
{
    {-10,51815},
    {-9,49283},
    {-8,46889},
    {-7,44624},
    {-6,42481},
    {-5,40450},
    {-4,38526},
    {-3,36702},
    {-2,34971},
    {-1,33329},
    {0,31770},
    {1,30253},
    {2,28815},
    {3,27453},
    {4,26160},
    {5,24935},
    {6,23772},
    {7,22668},
    {8,21620},
    {9,20626},
    {10,19681},
    {11,18784},
    {12,17932},
    {13,17122},
    {14,16353},
    {15,15621},
    {16,14925},
    {17,14263},
    {18,13634},
    {19,13035},
    {20,12465},
    {21,11922},
    {22,11406},
    {23,10914},
    {24,10446},
    {25,10000},
    {26,9574},
    {27,9169},
    {28,8783},
    {29,8415},
    {30,8064},
    {31,7729},
    {32,7410},
    {33,7105},
    {34,6814},
    {35,6537},
    {36,6272},
    {37,6019},
    {38,5778},
    {39,5547},
    {40,5327},
    {41,5116},
    {42,4915},
    {43,4722},
    {44,4539},
    {45,4363},
    {46,4195},
    {47,4034},
    {48,3880},
    {49,3733},
    {50,3592},
};


// static bool example_adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle)
// {
//     adc_cali_handle_t handle = NULL;
//     esp_err_t ret = ESP_FAIL;
//     bool calibrated = false;

// #if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
//     if (!calibrated) {
//         ESP_LOGI(TAG, "calibration scheme version is %s", "Curve Fitting");
//         adc_cali_curve_fitting_config_t cali_config = {
//             .unit_id = unit,
//             .atten = atten,
//             .bitwidth = ADC_BITWIDTH_DEFAULT,
//         };
//         ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
//         if (ret == ESP_OK) {
//             calibrated = true;
//         }
//     }
// #endif

// #if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
//     if (!calibrated) {
//         ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
//         adc_cali_line_fitting_config_t cali_config = {
//             .unit_id = unit,
//             .atten = atten,
//             .bitwidth = ADC_BITWIDTH_DEFAULT,
//         };
//         ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
//         if (ret == ESP_OK) {
//             calibrated = true;
//         }
//     }
// #endif

//     *out_handle = handle;
//     if (ret == ESP_OK) {
//         ESP_LOGI(TAG, "Calibration Success");
//     } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) {
//         ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
//     } else {
//         ESP_LOGE(TAG, "Invalid arg or no memory");
//     }
//     return calibrated;
// }





/////////////////////////////       第一遍        /////////////////////////////////////////////////

// static const uint16_t ntc_table_long = (sizeof(s_ntc_table))/(sizeof(s_ntc_table[0]));

// static adc_oneshot_unit_handle_t NTC_handle;

// static adc_cali_handle_t NTC_out_handle;

// static uint32_t ntc_res;


// int find_in_table(uint32_t res,uint16_t left_cnt,uint16_t right_cnt )
// {
//     ESP_LOGD("NTC","res is %ld",res);
//     uint16_t middle_cnt = (right_cnt + left_cnt) /2;
//     if(res > s_ntc_table[left_cnt].res) return left_cnt;
//     if(res < s_ntc_table[right_cnt].res) return right_cnt;
//     if(res == s_ntc_table[middle_cnt].res) return middle_cnt;
//     if(middle_cnt == left_cnt) return left_cnt;
//     else if(s_ntc_table[middle_cnt].res < res  ) return find_in_table(res,left_cnt,middle_cnt);
//     else if(s_ntc_table[middle_cnt].res > res  ) return find_in_table(res,middle_cnt,right_cnt);
//     return 0;

// }





// float read_ntc_temp(uint32_t res)
// {
//     uint16_t left_cnt=0;
//     uint16_t right_cnt=ntc_table_long-1;

//     int index = find_in_table(res,left_cnt,right_cnt);

//     //ESP_LOGI("ntc"," index is %d",index);
//     vTaskDelay(pdMS_TO_TICKS(1000));

//     if(res==s_ntc_table[index].res) return s_ntc_table[index].temp;
//     else if((index==left_cnt) || (index==right_cnt)) return  s_ntc_table[index].temp;
//     else 
//     {
//         // ESP_LOGI("ntc"," res is %d",(int)res);
//         // ESP_LOGI("ntc","s_ntc_table[index].res is %d",(int)s_ntc_table[index].res);
//         float inter_temp = (float)s_ntc_table[index].temp-s_ntc_table[index+1].temp;
//         float  inter_res = s_ntc_table[index].res-s_ntc_table[index+1].res;
//         //ESP_LOGI("ntc","%f(inter_temp) / %f(inter_res) is %f",inter_temp,inter_res,inter_temp/inter_res);
//         return  (inter_temp/inter_res*(s_ntc_table[index].res-res)+s_ntc_table[index].temp);
//         //return  (s_ntc_table[index].temp-s_ntc_table[index+1].temp)/(float)(s_ntc_table[index].res-s_ntc_table[index+1].res)*(res-s_ntc_table[index].res)+ s_ntc_table[index].temp;
//     } 
// }




// void Task_NTC_read(void *param)
// {
//     uint8_t cnt=0;
//     while(1)
//     {
//         adc_oneshot_read(NTC_handle,TEMP_ADC_CHANNEL,&s_adc_raw[cnt]);
//         if(do_calibration1)
//         {
//             adc_cali_raw_to_voltage(NTC_out_handle,s_adc_raw[cnt],&s_voltage_raw[cnt]);
//         }
//         cnt++;
//         if(cnt>=10)
//         {
//             uint32_t voltage=0;
//             for(uint8_t i=0;i<ADC_VALUE_NUM;i++)
//             {
//                 voltage+=s_voltage_raw[i];
//             }
//             voltage =voltage / ADC_VALUE_NUM;

//             if(voltage < ADC_V_MAX)
//             {
//                 ntc_res=(voltage*NTC_RES)/(ADC_V_MAX-voltage);
//                 //  ntc_res=voltage/((ADC_V_MAX-voltage)/NTC_RES) ; //这条公司有问题（先除10000，会得到0，再除0，就崩溃了）
//                 //s_temp_value = get_ntc_temp(ntc_res);
//             }
//             cnt = 0;
//         }
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }

// }


// float NTC_read(void)
// {
//     s_temp_value =   read_ntc_temp(ntc_res);
//     return s_temp_value;
// }




// void NTC_Init(void)
// {
//     adc_oneshot_unit_init_cfg_t init_config = 
//     {
//         .unit_id=ADC_UNIT_1,
//     };
//     adc_oneshot_new_unit(&init_config,&NTC_handle);


//     adc_oneshot_chan_cfg_t config =
//     {
//         .atten=ADC_ATTEN_DB_12,
//         .bitwidth=ADC_BITWIDTH_12,
//     };
//     adc_oneshot_config_channel(NTC_handle,TEMP_ADC_CHANNEL,&config);//ERROR:第二个传参

//     do_calibration1 = example_adc_calibration_init(ADC_UNIT_1,ADC_ATTEN_DB_12,&NTC_out_handle);

//     xTaskCreatePinnedToCore(Task_NTC_read,"Task_NTC_read",2048,NULL,3,NULL,1);
// }






//////////////////////////////////   第二遍   ///////////////////////////////////////////////

// adc_oneshot_unit_handle_t ntc_handle;

// adc_cali_handle_t ntc_out_handle;

// uint32_t  NTC_res;

// uint16_t table_num = sizeof(s_ntc_table)/sizeof(temp_res_t);


// uint16_t find_in_table(uint32_t res,uint16_t left,uint16_t right)
// {
//     uint16_t middle = (left + right)/2;
//     if(res < s_ntc_table[right].res )  return right;
//     if(res > s_ntc_table[left].res )  return left;//这两句话仅在第一次进来可能用上，如果想节约资源，可以放到一个函数内
//     if(middle==left) return middle;
    
//     if(res < s_ntc_table[middle].res) return find_in_table(res,middle,right);
//     if(res > s_ntc_table[middle].res) return find_in_table(res,left,middle);
//     if(res == s_ntc_table[middle].res) return middle;
//     return 0;
// }

// float NTC_get_temp(void)
// {
//     uint16_t index=0;
//     index = find_in_table(NTC_res,0,table_num);

//     if(index==0 || index==table_num || (NTC_res == s_ntc_table[index].res)) return  s_ntc_table[index].temp;
//     else
//     {
//         int32_t intercal_res =s_ntc_table[index+1].res - s_ntc_table[index].res;//错误在这，结果为倒数
//         int8_t intercal_temp  =s_ntc_table[index+1].temp -s_ntc_table[index].temp;
//         float k = (float)intercal_temp/(float)intercal_res;
//         ESP_LOGI("NTC","k is %f",k);
//         int32_t b =(NTC_res - s_ntc_table[index].res);
//         ESP_LOGI("NTC","b is %ld",b);
//         return (float)s_ntc_table[index].temp + k * b ;
        
//     }


// }



// float NTC_read(void)
// {
//     return NTC_get_temp();

// }


// void NTC_get_res(void *param )
// {
//     uint8_t cnt=0;
//     while (1)
//     {
//         adc_oneshot_read(ntc_handle,ADC_CHANNEL_0,&s_adc_raw[cnt]);
//         // if(cnt == 1) ESP_LOGI("NTC","s_adc_raw is %u",s_adc_raw[cnt]);
//         if(do_calibration1)
//         {
//            adc_cali_raw_to_voltage(ntc_out_handle,s_adc_raw[cnt],&s_voltage_raw[cnt]);//采样数据转换成电压
//         //    if(cnt == 1) ESP_LOGI("NTC","s_voltage_raw is %u",s_voltage_raw[cnt]);
//         }
//         cnt++;
//         if (cnt>=10)
//         {
//             int voltage=0;
//             for(uint8_t i=0;i<ADC_VALUE_NUM;i++)
//             {
//                 voltage+=s_voltage_raw[i];
//             }
//             voltage  = voltage / ADC_VALUE_NUM ;
//             if(voltage < ADC_V_MAX)
//             {
//                  NTC_res= ( voltage/ ( (ADC_V_MAX -  voltage)/(float)NTC_RES  ) );
//             }
//             cnt = 0;

//         }
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
    

// }


// void NTC_Init(void)
// {
//     adc_oneshot_unit_init_cfg_t init_config =
//     {
//         .clk_src=ADC_RTC_CLK_SRC_DEFAULT,
//         .unit_id=ADC_UNIT_1,
//     };
//     adc_oneshot_new_unit(&init_config,&ntc_handle);

//     adc_oneshot_chan_cfg_t config =
//     {
//         .atten=ADC_ATTEN_DB_12,
//         .bitwidth=ADC_BITWIDTH_12,
//     };
//     adc_oneshot_config_channel(ntc_handle,ADC_CHANNEL_0,&config);


//     if (!do_calibration1) 
//     {
//         ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
//         adc_cali_line_fitting_config_t cali_config = {
//             .unit_id = ADC_UNIT_1,
//             .atten = ADC_BITWIDTH_12,
//             .bitwidth = ADC_BITWIDTH_DEFAULT,
//         };
//         esp_err_t ret = adc_cali_create_scheme_line_fitting(&cali_config, &ntc_out_handle);
//         if (ret == ESP_OK) 
//         {
//             do_calibration1 = true;
//         }
//         if (ret == ESP_OK) 
//         {
//         ESP_LOGI(TAG, "Calibration Success");
//         } 
//         else ESP_LOGI(TAG, "Calibration error");
//     }
    
    
//     do_calibration1 =  example_adc_calibration_init(ADC_UNIT_1,ADC_ATTEN_DB_12,&ntc_out_handle);

//     xTaskCreatePinnedToCore(NTC_get_res,"NTC_get_res",2048,NULL,3,NULL,1);



// }







/////////////////////////////////////////////  第三遍  ///////////////////////////////////////////////////////////////////////////////

adc_oneshot_unit_handle_t NTC_ret_unit;

adc_cali_handle_t NTC_ret_handle;

int res_buf[ADC_VALUE_NUM];
int voltage_buf[ADC_VALUE_NUM];

uint32_t NTC_res;

uint16_t table_num = (sizeof(s_ntc_table)/sizeof(temp_res_t));


void NTC_get_res(void *param)
{
    int cnt=0;
    while (1)
    {
        adc_oneshot_read(NTC_ret_unit,ADC_CHANNEL_0,&res_buf[cnt]);

        adc_cali_raw_to_voltage(NTC_ret_handle,res_buf[cnt],&voltage_buf[cnt]);

        cnt++;

        if(cnt>=ADC_VALUE_NUM)
        {
            int voltage = 0;
            for(uint8_t i=0;i<ADC_VALUE_NUM;i++)
            {
                voltage+=voltage_buf[i];
            }
            voltage  = voltage / ADC_VALUE_NUM;

            cnt = 0;

            NTC_res=(uint32_t)(voltage/((ADC_V_MAX - voltage)/((float)NTC_RES)));
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
    




}



void NTC_Init(void)
{
    adc_oneshot_unit_init_cfg_t init_config =
    {
        .clk_src=ADC_RTC_CLK_SRC_DEFAULT,
        .unit_id=ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_config,&NTC_ret_unit);

    adc_oneshot_chan_cfg_t config =
    {
        .atten=ADC_ATTEN_DB_12,
        .bitwidth=ADC_BITWIDTH_12,
    };

    adc_oneshot_config_channel(NTC_ret_unit,ADC_CHANNEL_0,&config);

    adc_cali_line_fitting_config_t config1 =
    {
        .atten=ADC_ATTEN_DB_12,
        .bitwidth=ADC_BITWIDTH_DEFAULT,
        .unit_id=ADC_UNIT_1,
    };
    adc_cali_create_scheme_line_fitting(&config1,&NTC_ret_handle);

    xTaskCreatePinnedToCore(NTC_get_res,"NTC_get_res",2048,NULL,3,NULL,1);

}



 uint16_t find_in_table(uint32_t res,uint16_t left,uint16_t right )
 {
    uint16_t middle = (left + right)/2;
    if(res < s_ntc_table[right].res) return right;
    if(res > s_ntc_table[left].res) return left;
    if(left == middle) return left;
    if( res < s_ntc_table[middle].res ) return find_in_table(res,middle,right);
    if( res > s_ntc_table[middle].res ) return find_in_table(res,left,middle);
    else return middle;
 }



float NTC_get_temp(void)
{
    uint16_t temp_num = find_in_table(NTC_res,0,table_num);
    if(temp_num ==0 ||  temp_num == table_num) return  (float)s_ntc_table[temp_num].temp;
    if(NTC_res == s_ntc_table[temp_num].res ) return  (float)s_ntc_table[temp_num].temp;
    else 
    {
         int8_t temp_intercal =  s_ntc_table[temp_num+1].temp -  s_ntc_table[temp_num].temp;
         uint32_t res_intercal =  s_ntc_table[temp_num].res - s_ntc_table[temp_num+1].res;
         float k= temp_intercal / (float)res_intercal;
         return  s_ntc_table[temp_num].temp + k* (  s_ntc_table[temp_num].res - NTC_res);
    }
}

float NTC_read(void)
{
    return NTC_get_temp();
}