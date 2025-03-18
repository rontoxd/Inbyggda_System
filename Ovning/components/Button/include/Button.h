#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef void (*button_callback_t)(int pin, int state);

typedef struct
{
    int pin;
    int state;
    int last_change_time;
    button_callback_t onPressed;
}button_t;

void func(void);
void button_init(button_t *button, uint8_t pin, uint8_t iomode, bool pulldown, bool pullup); //PIN = GPIO_NUM_X  1 = INPUT, 0 = OUTPUT
void button_update(button_t *button);
bool is_pressed(button_t *button);
void setOnPressed(button_t *button, void (*onPressed)(int pin, int state));