#include <stdio.h>
#include "printer.h"
#include "helper_component.h"
#include "esp_log.h"

extern "C" void app_main(void)
{
    int a = ovning_2::func();
    ESP_LOGI("MAIN", "got value %d", a);
    a = inbyggda_iot24::func();
    ESP_LOGI("MAIN", "got value %d", a);

    ovning_2::printNumbers(5, 10);

    ovning_2::takeTimeAndPrintEverySecond(10000);

    int product = ovning_2::multiply(2, 3);
    ESP_LOGI("MAIN", "product: %d", product);

}

