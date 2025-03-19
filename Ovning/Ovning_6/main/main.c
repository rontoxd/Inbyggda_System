#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "button.h"

#define BUTTON_GPIO GPIO_NUM_20  // Change to your actual button pin

button_t button;

void button_pressed_callback(int pin, int state) {
    printf("Button pressed! Pin: %d, State: %d\n", pin, state);
}

void app_main() {
    // Initialize the button
    button_init(&button, BUTTON_GPIO, 1, false, true);
    setOnPressed(&button, button_pressed_callback);
    
    while (1) {
        button_update(&button);
        vTaskDelay(pdMS_TO_TICKS(10)); // Small delay to avoid excessive CPU usage
    }
}
