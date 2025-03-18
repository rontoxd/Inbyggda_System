#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    uint8_t pin;
    uint8_t state;
    int64_t last_change_time;
}button_t;

void func(void);
void button_init(uint8_t pin, uint8_t iomode, bool pulldown, bool pullup);
void button_update(button_t *button);
bool is_pressed(button_t *button);