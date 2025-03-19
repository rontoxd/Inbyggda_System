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
       // ESP_LOGI(TAG, "ADC VALUE %d\n", potentio->adc_value);
}

int potentio_getValue(potentio_t *potentio){
    if (!potentio) {
        ESP_LOGE(TAG, "Potentiometer struct is NULL");
        return -1; // om structen inte är initierad, returnera error
    }
    return potentio->adc_value;
}

void potentio_threshold_task(void *arg) {
    potentio_threshold_task_params_t *params = (potentio_threshold_task_params_t *)arg;
    potentio_t *potentio = params->potentio;
    int threshold = params->threshold;
    threshold_callback_t callback = params->callback;

    bool was_above_threshold = false;  // State tracking

    while (1) {
        potentio_update(potentio);
        
        if (potentio->adc_value >= threshold) {
            if (!was_above_threshold) {  // Trigger callback only on first crossing
                ESP_LOGI(TAG, "Threshold reached: %d", potentio->adc_value);
                callback();
                was_above_threshold = true;  // Mark that we have crossed the threshold
            }
        } else {
            was_above_threshold = false;  // Reset state when value goes below threshold
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void potentio_setOnThreshold(potentio_t *potentio, int threshold, threshold_callback_t callback) {
    if (!potentio || !callback) {
        ESP_LOGE(TAG, "Invalid parameters: potentiometer struct or callback is NULL!");
        return;
    }

    // Allocate memory for task parameters
    potentio_threshold_task_params_t *params = malloc(sizeof(potentio_threshold_task_params_t));
    if (!params) {
        ESP_LOGE(TAG, "Failed to allocate memory for threshold task parameters");
        return;
    }

    params->potentio = potentio;
    params->threshold = threshold;
    params->callback = callback;

    xTaskCreate(potentio_threshold_task, "pot_threshold_task", 2048, params, 1, NULL);
}