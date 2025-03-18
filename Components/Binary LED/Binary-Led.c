#include <stdio.h>
#include "Binary-Led.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

void bin_led_init(binary_led_t *led, int pin)
{
    led->pin = pin;

    gpio_config_t config = {
        .pin_bit_mask = (1ULL << pin),
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE};
    
    gpio_config(&config);
    gpio_set_level(pin, 0);
    led->state = 0;
}

void bin_led_update(binary_led_t *led){
    gpio_set_level(led->pin, led->state);
}

void bin_set_led(binary_led_t *led, int value){
    if (led->state != value)
    {
        led->state = value;
        bin_led_update(led);
    }
}

void bin_led_blink(binary_led_t *led, int MS_on, int MS_off){
    bin_set_led(led, 1);
    vTaskDelay(pdMS_TO_TICKS(MS_on));
    bin_set_led(led, 0);
    vTaskDelay(pdMS_TO_TICKS(MS_off));
}