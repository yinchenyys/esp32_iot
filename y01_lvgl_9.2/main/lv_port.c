#include "lv_port.h"


/*
1.初始化和注册LVGL显示驱动
2.初始化和注册LVGL触摸驱动
3.初始化ST7789硬件接口
4.初始化CST816T硬件接口
5.提供一个定时器给LVGL使用
*/

#define TAG     "lv_port"

#define LCD_WIDTH       240
#define LCD_HEIGHT      280

// static lv_disp_drv_t  disp_drv;

static lv_disp_t *disp;      // 指向液晶屏
static lv_indev_t *disp_indev = NULL; // 指向触摸屏




// void lv_flush_done_cb(void* param)
// {
//     lv_disp_flush_ready(&disp_drv);
// }

// void disp_flush(struct _lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
// {
//     st7789_flush(area->x1,area->x2 + 1,area->y1+20,area->y2+20 + 1,color_p);
// }

// void lv_disp_init(void)
// {
//     //初始化方法8.3与9.3不兼容
//     static lv_disp_draw_buf_t   disp_buf;
//     const size_t disp_buf_size = LCD_WIDTH*(LCD_HEIGHT/7);

//     lv_color_t *disp1 = heap_caps_malloc(disp_buf_size*sizeof(lv_color_t),MALLOC_CAP_INTERNAL|MALLOC_CAP_DMA);
//     lv_color_t *disp2 = heap_caps_malloc(disp_buf_size*sizeof(lv_color_t),MALLOC_CAP_INTERNAL|MALLOC_CAP_DMA);
//     if(!disp1 || !disp2)
//     {
//         ESP_LOGE(TAG,"disp buff malloc fail!");
//         return;
//     }
//     lv_disp_draw_buf_init(&disp_buf,disp1,disp2,disp_buf_size);



//     // 创建绘制函数，lvgl9.3有必要也可以用

//     lv_disp_drv_init(&disp_drv);

//     disp_drv.hor_res = LCD_WIDTH;
//     disp_drv.ver_res = LCD_HEIGHT;
//     disp_drv.draw_buf = &disp_buf;
//     disp_drv.flush_cb = disp_flush;
//     lv_disp_drv_register(&disp_drv);
// }



void lcd_set_color(uint16_t color)  //采用RGB565
{
    // 分配内存 这里分配了液晶屏一行数据需要的大小
    uint16_t *buffer = (uint16_t *)heap_caps_malloc(LCD_HEIGHT * sizeof(uint16_t), MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    
    if (NULL == buffer)
    {
        ESP_LOGE(TAG, "Memory for bitmap is not enough");
    }
    else
    {
        for (size_t i = 0; i < LCD_HEIGHT; i++) // 给缓存中放入颜色数据
        {
            buffer[i] = color;
        }
        for (int y = 0; y < LCD_WIDTH; y++) // 显示整屏颜色
        {
            esp_lcd_panel_draw_bitmap(panel_handle, 0, y, LCD_HEIGHT, y+1, buffer);
        }
        free(buffer); // 释放内存
    }
}

static lv_disp_t *bsp_display_lcd_init(void)
{
    /* 初始化液晶屏 */
    // 液晶屏驱动初始化
    lcd_set_color(0xffff); // 设置整屏背景白色 
    esp_lcd_panel_disp_on_off(panel_handle, true); // 打开液晶屏显示


    const lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = 4,         /* LVGL task priority 优先级*/
        .task_stack = 8192,         /* LVGL task stack size 栈空间*/
        .task_affinity = 1,        /* LVGL task pinned to core (-1 is no affinity) 运行在哪一个核心上*/
        .task_max_sleep_ms = 500,   /* Maximum sleep in LVGL task */
        .timer_period_ms = 5        /* LVGL timer tick period in ms */
    };
    lvgl_port_init(&lvgl_cfg);
    // ESP_RETURN_ON_ERROR(lvgl_port_init(&lvgl_cfg), TAG, "LVGL port initialization failed");

    /* 液晶屏添加LVGL接口 */
    ESP_LOGD(TAG, "Add LCD screen");
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = lcd_io_handle,
        .panel_handle = panel_handle,
        .buffer_size = LCD_WIDTH * LCD_HEIGHT/7,   // LVGL缓存大小，太高占用pram(￥原先不除7出错了)
        .double_buffer = false, // 是否开启双缓存，能提高刷屏效果但会占用双倍buffer_size
        .monochrome = false,  // 是否单色显示器
        /* Rotation的值必须和液晶屏初始化里面设置的 翻转 和 镜像 一样 */
        .hres = LCD_HEIGHT, // 液晶屏的宽
        .vres = LCD_WIDTH, // 液晶屏的高   
        .color_format = LV_COLOR_FORMAT_RGB565,
        #if show_y
        .rotation = {
             #if show_f
            .swap_xy = false,  // 是否翻转,swap_xy并非只是逆时针旋转90度,还会镜像，因为绘制必然要从左上角开始
            .mirror_x = true, // x方向是否镜像
            .mirror_y = true, // y方向是否镜像//要和屏幕一致

             #else
            .swap_xy = false,  // 是否翻转
            .mirror_x = false, // x方向是否镜像
            .mirror_y = false, // y方向是否镜像//要和屏幕一致
            #endif
           
        }, 
        #else 
        .rotation = {
            #if show_f
            .swap_xy = true,  // 是否翻转
            .mirror_x = false, // x方向是否镜像
            .mirror_y = true, // y方向是否镜像//要和屏幕一致

             #else
            .swap_xy = true,  // 是否翻转
            .mirror_x = true, // x方向是否镜像
            .mirror_y = false, // y方向是否镜像//要和屏幕一致
            #endif
        },      
        #endif

        .flags = {
            .buff_dma = false,  // 是否使用DMA 注意：dma与spiram不能同时为true
            .buff_spiram = true, // 是否使用PSRAM 注意：dma与spiram不能同时为true
            .swap_bytes = true,//天蓝变黄绿，交互一下颜色字节顺序
        },
        // .swap
    };

    return lvgl_port_add_disp(&disp_cfg);
}

// void IRAM_ATTR indev_read(struct _lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
// {
//     int16_t x,y;
//     int state;
//     cst816t_read(&x,&y,&state);
//     data->point.x = x;
//     data->point.y = y;
//     data->state = state;
// }

// void lv_indev_init(void)
// {
//     //初始化方法8.3与9.3不兼容
//     static lv_indev_drv_t indev_drv;
//     lv_indev_drv_init(&indev_drv);
//     indev_drv.type = LV_INDEV_TYPE_POINTER;
//     indev_drv.read_cb = indev_read;
//     lv_indev_drv_register(&indev_drv);
// }


// 1. 读取回调函数，函数签名有变化
// 关键变化：参数从 struct _lv_indev_drv_t* 变成了 lv_indev_t*
void IRAM_ATTR indev_read(lv_indev_t * indev, lv_indev_data_t * data)
{
    int16_t x, y;
    int state;
    // 调用你的触摸芯片驱动读取函数
    cst816t_read(&x, &y, &state);// 将读取到的坐标和状态赋值给 data 结构体
    data->point.x = y ;
    data->point.y = LCD_WIDTH - x ;
    // LVGL 9 中，状态通常使用 LV_INDEV_STATE_PRESSED / LV_INDEV_STATE_RELEASED
    data->state = state ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

// 2. 输入设备初始化函数
lv_indev_t * bsp_display_indev_init(lv_display_t * disp) 
{
    
    lv_indev_t * indev = lv_indev_create();// 1. 创建输入设备对象
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);// 2. 设置设备类型（例如：指针、按键、旋钮等）
    lv_indev_set_read_cb(indev, indev_read);// 3. 设置读取回调函数
    lv_indev_set_display(indev, disp);
    return indev;
    // 注意：LVGL 9 不再需要 lv_indev_drv_init 和 lv_indev_drv_register
}




void st7789_hw_init(void)
{
    st7789_cfg_t st7789_config = {
        .bl = GPIO_NUM_26,
        .clk = GPIO_NUM_18,
        .cs = GPIO_NUM_5,
        .dc = GPIO_NUM_17,
        .mosi = GPIO_NUM_19,
        .rst = GPIO_NUM_21,
        .spi_fre = 40*1000*1000,
        .height = LCD_HEIGHT,
        .width = LCD_WIDTH,
        .spin = 0,
        .done_cb = NULL,//lv_flush_done_cb
        .cb_param = NULL,//&disp_drv 已经不需要绘制
    };

    st7789_driver_hw_init(&st7789_config);
}

void cst816t_hw_init(void)
{
    cst816t_cfg_t cst816t_config = 
    {
        .scl = GPIO_NUM_22,
        .sda = GPIO_NUM_23,
        .fre = 300*1000,
        .x_limit = LCD_WIDTH,
        .y_limit = LCD_HEIGHT,
    };
    cst816t_init(&cst816t_config);
}

void lv_timer_cb(void* arg)
{
    uint32_t tick_interval = *((uint32_t*)arg);
    lv_tick_inc(tick_interval);
}

void lv_tick_init(void)
{
    static uint32_t tick_interval = 5;
    const esp_timer_create_args_t arg = 
    {
        .arg = &tick_interval,
        .callback = lv_timer_cb,
        .name = "",
        .dispatch_method = ESP_TIMER_TASK,
        .skip_unhandled_events = true,
    };

    esp_timer_handle_t timer_handle;
    esp_timer_create(&arg,&timer_handle);
    esp_timer_start_periodic(timer_handle,tick_interval*1000);
}


void bsp_lvgl_start(void)
{
    /* 初始化LVGL */
    lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    lvgl_port_init(&lvgl_cfg);

    /* 初始化液晶屏 并添加LVGL接口 */
    disp = bsp_display_lcd_init();

    if (disp) 
    {
        lv_display_set_offset(disp, 20, 0); 
    }

    /* 初始化触摸屏 并添加LVGL接口 */
    disp_indev = bsp_display_indev_init(disp);

    /* 打开液晶屏背光 */
    // bsp_display_brightness_set(100);
    st7789_lcd_backlight(1);

}

void lv_port_init(void)
{
    lv_init();
    st7789_hw_init();
    cst816t_hw_init();
    // lv_disp_init();//这个lvgl是如何知道绘画到哪里去的，回调函数
    // lv_indev_init();
    // lv_tick_init();
    bsp_lvgl_start();
}
