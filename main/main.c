#include <stdio.h>
#include <string.h>
#include <time.h>

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// I2C driver
#include "driver/i2c.h"

// SHT4x driver
#include "sht4x.h"

// Error library
#include "esp_err.h"
#include "esp_log.h"

// UART driver
#include "driver/uart.h"

#define USE_UART false
#define REPORT_OVER_CONSOLE false

// UART definition
#define BUF_SIZE (1024)
#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)

#define SHT4X_SDA_GPIO CONFIG_SHT4X_I2C_SDA /*!< gpio number for I2C master data set up on the sdkconfig */
#define SHT4X_SCL_GPIO CONFIG_SHT4X_I2C_SCL /*!< gpio number for I2C master clock */

static const char *TAG = "SHT4X:";
static const char *I2CTAG = "I2C:";

i2c_master_dev_handle_t sht4x_handle;

void sensor_report_data(void *pvParameter)
{
    float temperature, humidity;

    while (1)
    {

        esp_err_t err = sht4x_start_measurement(sht4x_handle, SHT4X_CMD_READ_MEASUREMENT_HIGH);
        // delay in between the request of a conversion to a
        vTaskDelay(pdMS_TO_TICKS(50));
        err = sht4x_read_measurement(sht4x_handle, &temperature, &humidity);

        if (err == ESP_OK)
        {
            if (USE_UART)
            {
                // Implementation of reporting over RS485
            }
            else
                ESP_LOGI(TAG, "Temperature: %.2f C, Humidity: %.2f %%", temperature, humidity);
        }
        else
        {
            if (REPORT_OVER_CONSOLE)
                ESP_LOGE(TAG, "Failed to read temperature and humidity");
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
// setup the I²C bus with error checks

i2c_master_bus_handle_t i2c_bus_init(uint8_t sda_io, uint8_t scl_io)
{
    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = CONFIG_SHT4X_I2C_NUM,
        .sda_io_num = sda_io,
        .scl_io_num = scl_io,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t bus_handle;

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &bus_handle));
    ESP_LOGI(I2CTAG, "I2C master bus created");

    return bus_handle;
}

void app_main(void)
{

    // Initialize the I2C
    i2c_master_bus_handle_t bus_handle = i2c_bus_init(SHT4X_SDA_GPIO, SHT4X_SCL_GPIO);
    sht4x_handle = sht4x_device_create(bus_handle, SHT4X_I2C_ADDR_0, CONFIG_SHT4X_I2C_CLK_SPEED_HZ);
    if (REPORT_OVER_CONSOLE)
        ESP_LOGI(TAG, "Sensor initialization success");

    // Probe the sensor to check if it is connected to the bus with a 10ms timeout
    esp_err_t err = i2c_master_probe(bus_handle, SHT4X_I2C_ADDR_0, 200);

    if (err == ESP_OK)
    {
        if (REPORT_OVER_CONSOLE)
            ESP_LOGI(TAG, "SHT4X sensor found");
        xTaskCreate(sensor_report_data, "sensor_report_data", 4096, NULL, 5, NULL);
    }
    else
    {
        if (REPORT_OVER_CONSOLE)
            ESP_LOGE(TAG, "SHT4X sensor not found");
        sht4x_device_delete(sht4x_handle);
    }
}
