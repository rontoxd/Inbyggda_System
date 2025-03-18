#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "Potentiometer.h"

#define TAG "POTENTIOMETER"

void potentio_init(potentio_t *potentio, int pin, int unit, int ulp, int attenuation, int bitwidth){
    potentio->pin = pin;
    potentio->unit = unit;
    potentio->ulp = ulp;
    potentio->attenuation = attenuation;
    potentio->bitwidth = bitwidth;

    adc_oneshot_unit_init_cfg_t adcConfig = {};
    adcConfig.unit_id = unit;
    adcConfig.ulp_mode = ulp;

    esp_err_t err = adc_oneshot_new_unit(&adcConfig, &potentio->adc_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "ADC unit init failed: %s\n", esp_err_to_name(err));
        return;
    }

    adc_oneshot_chan_cfg_t config = {
        .atten = attenuation,
        .bitwidth = bitwidth
    };
    err = adc_oneshot_config_channel(potentio->adc_handle, pin, &config);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG,"ADC channel config failed: %s\n", esp_err_to_name(err));
        return;
    }
}

void potentio_update(potentio_t *potentio){
    esp_err_t err = adc_oneshot_read(potentio->adc_handle, potentio->pin, &potentio->adc_value);  
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "ADC read failed: %s\n", esp_err_to_name(err));
            return;
        }
        ESP_LOGI(TAG, "ADC VALUE %d\n", potentio->adc_value);
}

int potentio_getValue(potentio_t *potentio){
    if (!potentio) {
        ESP_LOGE(TAG, "Potentiometer struct is NULL");
        return -1; // Return error if struct is not initialized
    }
    return potentio->adc_value;
}

void potentio_setOnThreshold(potentio_t *potentio, int threshold, threshold_callback_t callback){
    if (!potentio || !callback) {
        ESP_LOGE(TAG, "invalid parameters: potentiometer struct or callback is NULL!");
        return;
    }

    while (1){
        potentio_update(potentio);

        if (potentio->adc_value >= threshold){
            ESP_LOGI(TAG, "Threshold reached: %d", potentio->adc_value);
            callback();
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}