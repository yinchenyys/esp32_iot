#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>
#include <driver/rmt_rx.h>
#include <driver/rmt_tx.h>
#include <soc/rmt_reg.h>
#include "driver/gpio.h" 
#include <esp_log.h>
#include <freertos/queue.h>
#include "esp32/rom/ets_sys.h"
#include "dht11.h"

#define DHT11_GPIO	25		// DHT11引脚定义
const static char *TAG = "DHT11_Demo";

// 温度 湿度变量
//int temp = 0,hum = 0;
uint16_t temp = 0, hump = 0;

// 主函数
void app_main(void)
{
	ESP_ERROR_CHECK(nvs_flash_init());
	vTaskDelay(100 / portTICK_PERIOD_MS);

	ESP_LOGI(TAG, "[APP] APP Is Start!~\r\n");
	ESP_LOGI(TAG, "[APP] IDF Version is %d.%d.%d",ESP_IDF_VERSION_MAJOR,ESP_IDF_VERSION_MINOR,ESP_IDF_VERSION_PATCH);
	ESP_LOGI(TAG, "[APP] Free memory: %lu bytes", esp_get_free_heap_size());
	ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());
	
	//DHT11_Init(DHT11_GPIO);
	dht11_init(DHT11_GPIO);
	ESP_LOGI(TAG,"START");
	while (1){
		//if (DHT11_StartGet(&temp, &hump))
		if (dht11_start(&temp, &hump))
		{
			ESP_LOGI(TAG, "temp->%i.%i Chum->%i%%", temp / 10, temp % 10, hump);
		}
		ESP_LOGI(TAG,"END");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
