idf_component_register(SRCS  "ap_wifi.c" "ws_server.c" "wifi_manager.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_wifi esp_http_server lwip json spiffs  )#放到main不用,可能main默认编译所有组件？
spiffs_create_partition_image(html ../../html FLASH_IN_PROJECT)
