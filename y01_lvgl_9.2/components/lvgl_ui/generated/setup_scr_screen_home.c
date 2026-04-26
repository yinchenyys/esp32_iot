/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



int screen_home_digital_clock_1_min_value = 25;
int screen_home_digital_clock_1_hour_value = 11;
int screen_home_digital_clock_1_sec_value = 50;
void setup_scr_screen_home(lv_ui *ui)
{
    //Write codes screen_home
    ui->screen_home = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_home, 280, 240);
    lv_obj_set_scrollbar_mode(ui->screen_home, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_home, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_home, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_home, lv_color_hex(0x283938), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_home, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_digital_clock_1
    static bool screen_home_digital_clock_1_timer_enabled = false;
    ui->screen_home_digital_clock_1 = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_digital_clock_1, 20, 34);
    lv_obj_set_size(ui->screen_home_digital_clock_1, 120, 41);
    lv_label_set_text(ui->screen_home_digital_clock_1, "11:25:50");
    if (!screen_home_digital_clock_1_timer_enabled) {
        lv_timer_create(screen_home_digital_clock_1_timer, 2000, NULL);
        screen_home_digital_clock_1_timer_enabled = true;
    }

    //Write style for screen_home_digital_clock_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->screen_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_digital_clock_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_digital_clock_1, &lv_font_ZiTiQuanWeiJunHeiW22_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_digital_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_digital_clock_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_digital_clock_1, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_data
    ui->screen_home_label_data = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_label_data, 162, 39);
    lv_obj_set_size(ui->screen_home_label_data, 105, 18);
    lv_label_set_text(ui->screen_home_label_data, "2026/04/12");
    lv_label_set_long_mode(ui->screen_home_label_data, LV_LABEL_LONG_WRAP);

    //Write style for screen_home_label_data, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_data, lv_color_hex(0xd5ed42), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_data, &lv_customer_font_ZiTiQuanWeiJunHeiW22_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_data, 234, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_data, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_data, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_week
    ui->screen_home_label_week = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_label_week, 158, 64);
    lv_obj_set_size(ui->screen_home_label_week, 100, 24);
    lv_label_set_text(ui->screen_home_label_week, "星期日");
    lv_label_set_long_mode(ui->screen_home_label_week, LV_LABEL_LONG_WRAP);

    //Write style for screen_home_label_week, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_week, lv_color_hex(0xabf28e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_week, &lv_customer_font_ZiTiQuanWeiJunHeiW22_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_week, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_week, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_where
    ui->screen_home_label_where = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_label_where, 212, 6);
    lv_obj_set_size(ui->screen_home_label_where, 56, 28);
    lv_label_set_text(ui->screen_home_label_where, "珠海");
    lv_label_set_long_mode(ui->screen_home_label_where, LV_LABEL_LONG_WRAP);

    //Write style for screen_home_label_where, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_where, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_where, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_where, lv_color_hex(0xabf28e), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_where, &lv_customer_font_ZiTiQuanWeiJunHeiW22_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_where, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_where, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_where, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_where, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_where, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_where, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_where, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_where, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_where, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_where, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_day0
    ui->screen_home_label_day0 = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_label_day0, 4, 104);
    lv_obj_set_size(ui->screen_home_label_day0, 94, 25);
    lv_label_set_text(ui->screen_home_label_day0, "今日");
    lv_label_set_long_mode(ui->screen_home_label_day0, LV_LABEL_LONG_WRAP);

    //Write style for screen_home_label_day0, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_day0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_day0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_day0, lv_color_hex(0xd8fff8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_day0, &lv_customer_font_ZiTiQuanWeiJunHeiW22_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_day0, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_day0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_day0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_day0, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_day0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_day0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_day0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_day0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_day0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_day0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_day1
    ui->screen_home_label_day1 = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_label_day1, 91, 104);
    lv_obj_set_size(ui->screen_home_label_day1, 100, 24);
    lv_label_set_text(ui->screen_home_label_day1, "明日");
    lv_label_set_long_mode(ui->screen_home_label_day1, LV_LABEL_LONG_WRAP);

    //Write style for screen_home_label_day1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_day1, lv_color_hex(0xd8fff8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_day1, &lv_customer_font_ZiTiQuanWeiJunHeiW22_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_day1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_day1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_day1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_day2
    ui->screen_home_label_day2 = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_label_day2, 191, 104);
    lv_obj_set_size(ui->screen_home_label_day2, 81, 20);
    lv_label_set_text(ui->screen_home_label_day2, "后日");
    lv_label_set_long_mode(ui->screen_home_label_day2, LV_LABEL_LONG_WRAP);

    //Write style for screen_home_label_day2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_day2, lv_color_hex(0xd8fff8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_day2, &lv_customer_font_ZiTiQuanWeiJunHeiW22_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_day2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_day2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_day2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_wifi
    ui->screen_home_img_wifi = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_wifi, 7, 6);
    lv_obj_set_size(ui->screen_home_img_wifi, 32, 32);
    lv_obj_add_flag(ui->screen_home_img_wifi, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_wifi, &_wifi_RGB565A8_32x32);
    lv_image_set_pivot(ui->screen_home_img_wifi, 50,50);
    lv_image_set_rotation(ui->screen_home_img_wifi, 0);

    //Write style for screen_home_img_wifi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_wifi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_line
    ui->screen_home_img_line = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_line, 12, 81);
    lv_obj_set_size(ui->screen_home_img_line, 257, 10);
    lv_obj_add_flag(ui->screen_home_img_line, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_line, &_line_RGB565A8_257x10);
    lv_image_set_pivot(ui->screen_home_img_line, 50,50);
    lv_image_set_rotation(ui->screen_home_img_line, 0);

    //Write style for screen_home_img_line, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_line, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_line, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_day0_weather
    ui->screen_home_img_day0_weather = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_day0_weather, 25, 137);
    lv_obj_set_size(ui->screen_home_img_day0_weather, 62, 66);
    lv_obj_add_flag(ui->screen_home_img_day0_weather, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_day0_weather, &_yu_RGB565A8_62x66);
    lv_image_set_pivot(ui->screen_home_img_day0_weather, 50,50);
    lv_image_set_rotation(ui->screen_home_img_day0_weather, 0);

    //Write style for screen_home_img_day0_weather, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_day0_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_day0_weather, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_day1_weather
    ui->screen_home_img_day1_weather = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_day1_weather, 110, 137);
    lv_obj_set_size(ui->screen_home_img_day1_weather, 62, 66);
    lv_obj_add_flag(ui->screen_home_img_day1_weather, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_day1_weather, &_yu_RGB565A8_62x66);
    lv_image_set_pivot(ui->screen_home_img_day1_weather, 50,50);
    lv_image_set_rotation(ui->screen_home_img_day1_weather, 0);

    //Write style for screen_home_img_day1_weather, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_day1_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_day1_weather, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_day2_weather
    ui->screen_home_img_day2_weather = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_day2_weather, 200, 137);
    lv_obj_set_size(ui->screen_home_img_day2_weather, 62, 66);
    lv_obj_add_flag(ui->screen_home_img_day2_weather, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_day2_weather, &_yu_RGB565A8_62x66);
    lv_image_set_pivot(ui->screen_home_img_day2_weather, 50,50);
    lv_image_set_rotation(ui->screen_home_img_day2_weather, 0);

    //Write style for screen_home_img_day2_weather, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_day2_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_day2_weather, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_temp1
    ui->screen_home_label_temp1 = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_label_temp1, 4, 210);
    lv_obj_set_size(ui->screen_home_label_temp1, 97, 21);
    lv_label_set_text(ui->screen_home_label_temp1, "32-38℃");
    lv_label_set_long_mode(ui->screen_home_label_temp1, LV_LABEL_LONG_WRAP);

    //Write style for screen_home_label_temp1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_temp1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_temp1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_temp1, lv_color_hex(0xd8fff8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_temp1, &lv_customer_font_ZiTiQuanWeiJunHeiW22_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_temp1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_temp1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_temp1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_temp1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_temp1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_temp1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_temp1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_temp1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_temp1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_temp1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_temp2
    ui->screen_home_label_temp2 = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_label_temp2, 91, 210);
    lv_obj_set_size(ui->screen_home_label_temp2, 97, 21);
    lv_label_set_text(ui->screen_home_label_temp2, "32-38℃");
    lv_label_set_long_mode(ui->screen_home_label_temp2, LV_LABEL_LONG_WRAP);

    //Write style for screen_home_label_temp2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_temp2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_temp2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_temp2, lv_color_hex(0xd8fff8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_temp2, &lv_customer_font_ZiTiQuanWeiJunHeiW22_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_temp2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_temp2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_temp2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_temp2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_temp2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_temp2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_temp2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_temp2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_temp2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_temp2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_temp3
    ui->screen_home_label_temp3 = lv_label_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_label_temp3, 181, 210);
    lv_obj_set_size(ui->screen_home_label_temp3, 89, 21);
    lv_label_set_text(ui->screen_home_label_temp3, "32-38℃");
    lv_label_set_long_mode(ui->screen_home_label_temp3, LV_LABEL_LONG_WRAP);

    //Write style for screen_home_label_temp3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_home_label_temp3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_home_label_temp3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_home_label_temp3, lv_color_hex(0xd8fff8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_home_label_temp3, &lv_customer_font_ZiTiQuanWeiJunHeiW22_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_home_label_temp3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_home_label_temp3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_home_label_temp3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_home_label_temp3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_home_label_temp3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_home_label_temp3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_home_label_temp3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_home_label_temp3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_home_label_temp3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_home_label_temp3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_home.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_home);

    //Init events for screen.
    events_init_screen_home(ui);
}
