/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/


/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"
#include <time.h>
#include <string.h>
#include "lv_port.h"
#include "aida64.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**
 * Create a demo application
 */





extern int screen_home_digital_clock_1_hour_value;
extern int screen_home_digital_clock_1_min_value;
extern int screen_home_digital_clock_1_sec_value;

 int screen_home_digital_clock_1_year_value;
 int screen_home_digital_clock_1_month_value;
 int screen_home_digital_clock_1_day_value;
 int screen_home_digital_clock_1_w_day_value;




void set_home_time(lv_ui *ui ,int year , int month , int day , int w_day, int hour , int min , int sec)
{

    screen_home_digital_clock_1_year_value=year;
    screen_home_digital_clock_1_month_value=month;
    screen_home_digital_clock_1_day_value=day;
    screen_home_digital_clock_1_w_day_value=w_day;

    screen_home_digital_clock_1_hour_value=hour;
    screen_home_digital_clock_1_min_value=min;
    screen_home_digital_clock_1_sec_value=sec;

    char text[64];
    snprintf(text,sizeof(text),"%d/%d/%d",year,month,day);

    char week_text[64];
    static const char *week_day_text[] = 
    {
        "星期日",
        "星期一",
        "星期二",
        "星期三",
        "星期四",
        "星期五",
        "星期六",
    };
    snprintf(week_text,sizeof(week_text),"%s",week_day_text[w_day]);
    lvgl_port_lock(0);
    lv_label_set_text(ui->screen_home_label_data,text);
    lv_label_set_text(ui->screen_home_label_week,week_text);
    lvgl_port_unlock();

}


void set_today_img(lv_ui *ui,const char* img_path, int low , int high)
{
    char temp_text[32];
    lvgl_port_lock(0);
    lv_img_set_src(ui->screen_home_img_day0_weather,img_path);//该函数会自动识别传进来的图片是代码还是路径
    snprintf(temp_text,sizeof(temp_text),"%d-%d℃",low,high);
    lv_label_set_text(ui->screen_home_label_temp1,temp_text);
    lvgl_port_unlock();
}


void set_tommorrow_img(lv_ui *ui,const char* img_path, int low , int high)
{
    char temp_text[32];
    lvgl_port_lock(0);
    lv_img_set_src(ui->screen_home_img_day1_weather,img_path);//该函数会自动识别传进来的图片是代码还是路径
    snprintf(temp_text,sizeof(temp_text),"%d-%d℃",low,high);
    lv_label_set_text(ui->screen_home_label_temp2,temp_text);
    lvgl_port_unlock();
}


void set_after_img(lv_ui *ui,const char* img_path, int low , int high)
{
    char temp_text[32];
    lvgl_port_lock(0);
    lv_img_set_src(ui->screen_home_img_day2_weather,img_path);//该函数会自动识别传进来的图片是代码还是路径
    snprintf(temp_text,sizeof(temp_text),"%d-%d℃",low,high);
    lv_label_set_text(ui->screen_home_label_temp3,temp_text);
    lvgl_port_unlock();
}

void set_local_where(lv_ui *ui,const char* place)
{
    char place_text[48];
    lvgl_port_lock(0);
    snprintf(place_text,sizeof(place_text),place);
    lv_label_set_text(ui->screen_home_label_where,place_text);
    lvgl_port_unlock();
}


void set_cpu_num(lv_ui *ui,int cpu_rate,int cpu_temp,int emm_rate,int emm_use)
{
    char value[16];
    lvgl_port_lock(0);

    snprintf(value,sizeof(value),"%d%%",cpu_rate);
    lv_label_set_text(ui->screen_1_label_100,value);

    snprintf(value,sizeof(value),"%d℃",cpu_temp);
    lv_label_set_text(ui->screen_1_label_num_cpu,value);

    snprintf(value,sizeof(value),"%d%%",emm_rate);
    lv_label_set_text(ui->screen_1_label_rota_emm,value);

    snprintf(value,sizeof(value),"%dMB",emm_use);
    lv_label_set_text(ui->screen_1_label_MB_emm,value);

    lvgl_port_unlock();
}

void set_link_state(lv_ui *ui,bool state)
{
    lvgl_port_lock(0);
    if(state) lv_label_set_text(ui->screen_1_label_state_link,"连接");
    else lv_label_set_text(ui->screen_1_label_state_link,"未连接");
    lvgl_port_unlock();
}


void custom_init(lv_ui *ui)
{
    
    /* Add your codes here */
}

