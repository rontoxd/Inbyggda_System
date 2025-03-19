#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "button.h"
#include "Potentiometer.h"

#define BUTTON_GPIO GPIO_NUM_20  // byt till rätt pin
#define POTENTIOMETER_GPIO ADC_CHANNEL_4 // byt till rätt ADC-kanal
#define POTENTIOMETER_UNIT ADC_UNIT_1
#define POTENTIOMETER_ATTEN ADC_ATTEN_DB_12
#define POTENTIOMETER_WIDTH ADC_BITWIDTH_DEFAULT
#define POTENTIOMETER_THRESHOLD 2000 // exempelvärde på threshold

button_t button;
potentio_t potentiometer;

void button_pressed_callback(int pin, int state) {
    printf("Button pressed! Pin: %d, State: %d\n", pin, state);
}

void potentiometer_threshold_callback() {
    printf("Potentiometer value exceeded threshold!\n");
}

void app_main() {
    // initialize button
    button_init(&button, BUTTON_GPIO, 1, false, true);
    setOnPressed(&button, button_pressed_callback);

    // initialize potentiometer
    potentio_init(&potentiometer, ADC_CHANNEL_4, ADC_UNIT_1, ADC_ULP_MODE_DISABLE, ADC_ATTEN_DB_12, ADC_BITWIDTH_DEFAULT);
    
    // skapar en FreeRTOS task för threshold monitoring
    /*xTaskCreatePinnedToCore(
        (void *)potentio_setOnThreshold,
        "Potentiometer Task",
        4096,
        &potentiometer, 
        1, 
        NULL, 
        tskNO_AFFINITY
    );*/


    while (1) {
        button_update(&button);
        potentio_update(&potentiometer);
        //printf("Potentiometer Value: %d\n", potentio_getValue(&potentiometer));
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
