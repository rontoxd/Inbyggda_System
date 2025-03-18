#include <stdio.h>
#include "analog-led.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include <math.h>

#define PI 3.14159269
#define TAG "ANALOG_LED"

void an_led_init(analog_led *led, int pin, int timer_num, int duty_res,
                 int ledc_mode, int freq, int ledc_channel) {
    if (!led) {
        ESP_LOGE(TAG, "LED struct is NULL");
        return;
    }

    led->pin = pin;
    led->duty_res = duty_res;
    led->ledc_channel = ledc_channel;
    led->ledc_mode = ledc_mode;

    gpio_config_t config = {
        .pin_bit_mask = (1ULL << pin),
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };

    esp_err_t err = gpio_config(&config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "GPIO config failed: %s", esp_err_to_name(err));
        return;
    }

    ledc_timer_config_t led_timer = {
        .timer_num = timer_num,
        .clk_cfg = LEDC_AUTO_CLK,
        .duty_resolution = duty_res,
        .freq_hz = freq,
        .speed_mode = ledc_mode
    };

    err = ledc_timer_config(&led_timer);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "LEDC timer config failed: %s", esp_err_to_name(err));
        return;
    }

    ledc_channel_config_t led_chn = {
        .channel = ledc_channel,
        .duty = 0,
        .speed_mode = ledc_mode,
        .gpio_num = led->pin,
        .intr_type = LEDC_INTR_DISABLE,
        .hpoint = 0,
        .timer_sel = timer_num
    };

    err = ledc_channel_config(&led_chn);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "LEDC channel config failed: %s", esp_err_to_name(err));
        return;
    }
}

void an_led_update(analog_led *led, int duty) {
    if (!led) {
        ESP_LOGE(TAG, "LED struct is NULL");
        return;
    }

    if (duty < 0) duty = 0;
    if (duty > (1 << led->duty_res) - 1) duty = (1 << led->duty_res) - 1;

    esp_err_t err = ledc_set_duty(led->ledc_mode, led->ledc_channel, duty);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "LEDC set duty failed: %s", esp_err_to_name(err));
        return;
    }

    err = ledc_update_duty(led->ledc_mode, led->ledc_channel);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "LEDC update duty failed: %s", esp_err_to_name(err));
    }
}

void an_set_brightness(analog_led *led, int brightness) {
    if (!led) {
        ESP_LOGE(TAG, "LED struct is NULL");
        return;
    }

    if (brightness < 0) brightness = 0;
    if (brightness > 100) brightness = 100;

    int duty = (brightness * ((1 << led->duty_res) - 1)) / 100;
    an_led_update(led, duty);
}

// Task for sine wave brightness animation
void an_led_sin_task(void *param) {
    analog_led *led = (analog_led *)param;
    int duration_ms = 2000; // Total duration of one cycle
    int steps = 50;         // Number of brightness steps per cycle

    while (1) {
        for (int i = 0; i < steps; i++) {
            float angle = (2 * PI * i) / steps;
            float brightness = (sinf(angle) + 1) / 2 * 100;

            an_set_brightness(led, (int)brightness);
            vTaskDelay(pdMS_TO_TICKS(duration_ms / steps));
        }
    }
}

void an_led_sin(analog_led *led) {
    if (!led) {
        ESP_LOGE(TAG, "LED struct is NULL");
        return;
    }

    xTaskCreate(an_led_sin_task, "led_sin_task", 2048, led, 1, NULL);
}