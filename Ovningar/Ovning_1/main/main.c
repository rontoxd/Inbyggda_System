#include <stdio.h>
#include "esp_log.h"

#define ANSI_RED "\033[0;31m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_BLUE "\033[0;34m"
#define ANSI_MAGENTA "\033[0;35m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_RESET "\033[0m"
#define NEW_LINE "\n"
#define NEW_WORLD_COUNT 1
#define TAG_MAIN "MAIN:"
#define TAG_ERROR "ERROR:"
#define TEST_STRING "test"

//MACRO-funktion - kompilerar funktionen i runtime, kallar på den som macro med förbestämda argument, format: MACRO_NAME(arg, arg, ...)
#define PRINTF_COLOR(color, format_string, ...) \
    printf("%s" format_string "%s", color, ##__VA_ARGS__, ANSI_RESET) //i det här fallet matar vi in färgmacro, %s och sedan valfritt antal strängar (%s)

//MACRO-funktion som kallar på en annan MACRO-funktion två gånger. Första gången printar den "Group 1" i blått, sedan strängen vi har som argument i main
#define PRINTF_GROUP_1(format, ...) \
    PRINTF_COLOR(ANSI_BLUE, "Group 1 "); \
    PRINTF_COLOR(ANSI_RESET, format, ##__VA_ARGS__) //Första PRINTF_COLOR blir blått och andra i default-text

#define PRINTF_COLOR_WITH_LINE(color, format_string, ...) \
    PRINTF_COLOR(ANSI_MAGENTA, "[%s:%d] ", __FILE__, __LINE__); \
    PRINTF_COLOR(color, format_string, ##__VA_ARGS__) \

#define PRINTF_CUSTOM_GROUP(tag, format_string, ...) \
    PRINTF_COLOR(ANSI_YELLOW, tag); \
    PRINTF_COLOR(ANSI_CYAN, format_string, ##__VA_ARGS__)

const char *TAG = TAG_MAIN;

void app_main(void)
{
    ESP_LOGI(TAG, "Hello World for my %d time", NEW_WORLD_COUNT); //TAG blir då "[string]:"

    PRINTF_COLOR(ANSI_RED, "%s" NEW_LINE, "test"); //printar "test" i rött
    PRINTF_GROUP_1("Hello World %d" NEW_LINE, NEW_WORLD_COUNT); //printar "Group 1" i blått och "Hello World" i default-färg (vitt)
    PRINTF_COLOR_WITH_LINE(ANSI_GREEN, "Hello world %d" NEW_LINE, NEW_WORLD_COUNT);
    PRINTF_CUSTOM_GROUP(TAG_MAIN, "Hello World");
}