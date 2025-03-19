#include <stdio.h>
#include "Button.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define DEBOUNCE_TIME_US 10000

// initiera knappen
void button_init(button_t *button, uint8_t pin, uint8_t iomode, bool pulldown, bool pullup){
    button->pin                 = pin;
    button->state               = gpio_get_level(pin);
    button->last_change_time    = esp_timer_get_time();
    button->onPressed           = NULL;
    
    //konfigurera GPIO
    gpio_config_t io_conf = {};
    io_conf.mode                = (iomode == 1) ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT;
    io_conf.intr_type           = GPIO_INTR_DISABLE;
    io_conf.pin_bit_mask        = (1ULL << pin);
    io_conf.pull_down_en        = pulldown ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en          = pullup ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
}

// uppdaterar knappens "state" (kallas i en loop)
void button_update(button_t *button){
    int current_state = gpio_get_level(button->pin);
    int64_t now = esp_timer_get_time();

    // kollar om knappens "state" har ändrats och debounce time har passerat
    if (current_state != button->state && (now - button->last_change_time) > DEBOUNCE_TIME_US){
        button->state = current_state;
        button->last_change_time = now;

        printf("Button %d changed state to %d\n", button->pin, button->state);

        // kalla den registrerade callbackfunktionen om knappen trycks
        if (button->state == 1 && button->onPressed != NULL)
        {
            button->onPressed(button->pin, button->state);
        }
        
    }
}

//kollar om knappen är nedtryckt
bool is_pressed(button_t *button){
    return button->state == 1;
}


// sätter callbackfunktion när knappen trycks
void setOnPressed(button_t *button, void (*onPressed)(int pin, int state)){
    button->onPressed = onPressed;
};