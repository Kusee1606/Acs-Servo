// TODO:
//  dodanie ESP_ERROR_CHECK()
#include <stdio.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

const uint8_t servo_pins[4] = {13, 12, 14, 27};

void init_canards(uint8_t pins[4], uint32_t frequency);
uint32_t angle_to_duty(float angle);
void set_canards(float cannard_angle1, float cannard_angle2, float cannard_angle3, float cannard_angle4);

void app_main(void)
{
    init_canards(servo_pins, 50);
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1)
    {
        set_canards(-30, -30, -30, -30);
        vTaskDelay(pdMS_TO_TICKS(1000));
        set_canards(30, 30, 30, 30);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// TODO: Ustawienie rozdzielczości pwm'a z użyciem funkcji ledc_find_suitable_duty_resolution()

void init_canards(uint8_t pins[4], uint32_t frequency)
{
    uint8_t resolution = 10;
    ledc_timer_config_t cannards_pwm_timer =
        {
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .duty_resolution = resolution,
            .timer_num = LEDC_TIMER_0,
            .freq_hz = frequency,
            .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&cannards_pwm_timer);

    float start_duty = angle_to_duty(0);
    ledc_channel_config_t ledc_channel[4];
    for (int i = 0; i < 4; i++)
    {
        ledc_channel[i].gpio_num = pins[i];
        ledc_channel[i].speed_mode = LEDC_HIGH_SPEED_MODE;
        ledc_channel[i].channel = i;
        ledc_channel[i].timer_sel = LEDC_TIMER_0;
        ledc_channel[i].intr_type = LEDC_INTR_DISABLE;
        ledc_channel[i].duty = start_duty;
        ledc_channel[i].hpoint = 0;

        ledc_channel_config(&ledc_channel[i]);
    }
}

uint32_t angle_to_duty(float angle)
{
    const uint8_t resolution = 10;
    const uint32_t period = 1000000 / ledc_get_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0);
    const float minAngle = -90;
    const float maxAngle = 90;
    const float minDuty = 1000;
    const float maxDuty = 2000;

    uint32_t duty = (unsigned int)((((((angle - minAngle) / (maxAngle - minAngle)) * (maxDuty - minDuty)) + minDuty) / period) * (1 << resolution));
    return duty;
}

void set_canards(float cannard_angle1, float cannard_angle2, float cannard_angle3, float cannard_angle4)
{
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, 0, angle_to_duty(cannard_angle1));
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, 1, angle_to_duty(cannard_angle2));
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, 2, angle_to_duty(cannard_angle3));
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, 3, angle_to_duty(cannard_angle4));
    ledc_update_duty(LEDC_HIGH_SPEED_MODE,0);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE,1);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE,2);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE,3);
}
