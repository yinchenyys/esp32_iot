#ifndef _WEATHER_H_
#define _WEATHER_H_

typedef struct 
{
    int high_temp;
    int low_temp;
    char code[4];
    /* data */
}weather_data_pkt_t;

void weather_start(void);
void init_weaher_img_buffer();
#endif // !_WEA