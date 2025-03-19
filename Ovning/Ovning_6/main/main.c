#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "button.h"
#include "Potentiometer.h"
#include "analog-led.h"

#define BUTTON_GPIO GPIO_NUM_20  // byt till rätt pin
#define POTENTIOMETER_GPIO ADC_CHANNEL_4 // byt till rätt ADC-kanal
#define POTENTIOMETER_UNIT ADC_UNIT_1
#define POTENTIOMETER_ATTEN ADC_ATTEN_DB_12
#define POTENTIOMETER_WIDTH ADC_BITWIDTH_DEFAULT
#define POTENTIOMETER_THRESHOLD 2000 // exempelvärde på threshold

#define LED_GPIO GPIO_NUM_2
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_10_BIT
#define LEDC_FREQUENCY 5000 // 5 kHz PWM frequency


button_t button;
potentio_t potentiometer;
analog_led led;

void onTh(){
    printf("th: %d\n", potentio_getValue(&potentiometer));
}

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

    potentio_setOnThreshold(&potentiometer, 2000, onTh);

    // initialize analog LED
    an_led_init(&led, LED_GPIO, LEDC_TIMER, LEDC_DUTY_RES, LEDC_MODE, LEDC_FREQUENCY, LEDC_CHANNEL);
    an_led_sin(&led);

    printf("Running while\n"); //checking if this code runs
    int e = 0;
    while (1) {
        button_update(&button);
        potentio_update(&potentiometer);

        if (e%30 == 0){
            int a = potentio_getValue(&potentiometer);
            printf("pot = %d\n", a);
        }
        e++;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
