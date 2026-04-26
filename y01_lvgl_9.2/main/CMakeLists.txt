set(INCLUDE "." "./../components/bsp"  "./../components/lvgl_ui/custom" "./../components/lvgl_ui/generated")
file(GLOB_RECURSE SOURCE_MAIN ./*.c )
file(GLOB_RECURSE SOURCE_COMPONENTS ./../components/*.c )

idf_component_register(SRCS ${SOURCE_MAIN} ${SOURCE_COMPONENTS}
                    INCLUDE_DIRS ${INCLUDE}
                    )
spiffs_create_partition_image(img  ../img FLASH_IN_PROJECT)

#idf_component_register(SRCS "aida64.c" "weather.c" "ui_led.c" "lvgl_display.c" "lv_port.c" "ui_led.c" "ui_home.c" 
#                        "img/humidity_img.c" "img/temp_img.c"
#                    INCLUDE_DIRS ".")


#REQUIRES driver esp_timer