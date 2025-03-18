#ifndef ANALOG_LED_H
#define ANALOG_LED_H

#include "driver/ledc.h"

// Struktur för att hantera en analog LED
typedef struct {
    int pin;
    int duty_res;
    int ledc_channel;
    int ledc_mode;
} analog_led;

// Funktioner för att styra den analoga LED:en
void an_led_init(analog_led *led, int pin, int timer_num, int duty_res, int ledc_mode, int freq, int ledc_channel);
void an_led_update(analog_led *led, int duty);
void an_set_brightness(analog_led *led, int brightness);
void an_led_sin(analog_led *led);  // Startar sinus-animation som körs i en separat task

#endif // ANALOG_LED_H