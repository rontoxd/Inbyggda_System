#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "esp_adc/adc_oneshot.h"
typedef void (*threshold_callback_t)(int pin, int value);
typedef struct
{
    int pin;
    int unit; 
    int ulp; 
    int attenuation; 
    int bitwidth;
    int adc_value;
    int current_time;
    bool risingEdge;
    threshold_callback_t onTh;
    adc_oneshot_unit_handle_t adc_handle;
}potentio_t;

typedef struct {
    potentio_t *potentio;
    int threshold;
    bool risingEdge;
    threshold_callback_t callback;
} potentio_threshold_task_params_t;

void func(void);
void potentio_init(potentio_t *potentio, int pin, int unit, int channel, int attenuation, int bitwidth);
void potentio_update(potentio_t *potentio);
int potentio_getValue(potentio_t *potentio);
void potentio_setOnThreshold(potentio_t *potentio, int threshold, bool risingEdge, threshold_callback_t callback);

#endif // POTENTIOMETER_H