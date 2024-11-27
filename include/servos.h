#ifndef __SERVOS__
#define __SERVOS__

#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

typedef struct
{
    uint32_t timestamp;
    float canard_angle1;
    float canard_angle2;
    float canard_angle3;
    float canard_angle4;
    SemaphoreHandle_t mutex;
} canards_t;

bool init_servos();
void update_canards(canards_t *new_canards);
uint32_t angle_to_duty(float angle);

#endif //__SERVOS__