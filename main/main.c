#include <stdio.h>

#include "driver/ledc.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../include/servos.h"

void app_main(void)
{
    if(init_servos()){
    vTaskDelay(pdMS_TO_TICKS(1000));
    canards_t new_canards;
    while (1)
    {
        new_canards = (canards_t){.canard_angle1 = -30, .canard_angle2 = -30, .canard_angle3 = -30, .canard_angle4 = -30};
        update_canards(&new_canards);
        vTaskDelay(pdMS_TO_TICKS(1000));
        new_canards = (canards_t){.canard_angle1 = 30, .canard_angle2 = 30, .canard_angle3 = 30, .canard_angle4 = 30};
        update_canards(&new_canards);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    }
}