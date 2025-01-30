#include <stdio.h>
#include "printer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace ovning_2
{
    int func(void){
        return CONFIG_PRINT_LEVEL;
    }

    void printNumbers(int start, int stop){
        for (int i = start; i < stop; i++){
            printf("i equals to %d\n", i);
        }
        
    }

    void takeTimeAndPrintEverySecond(int time) {
        int elapsedTime = 0;
          while (elapsedTime < time) {
          printf("Time elapsed: %d ms\n", elapsedTime);
          vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
          elapsedTime += 1000;
        }
    
        printf("Completed %d ms of execution.\n", time);
    }

    int multiply(int a, int b){
        int result = a * b;
        printf("multiplication result: %d\n", result);
        return result;
    }
}