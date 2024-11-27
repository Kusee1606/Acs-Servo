#include "../include/servos.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SERVO_PIN_1 13
#define SERVO_PIN_2 12
#define SERVO_PIN_3 14
#define SERVO_PIN_4 27

#define LEDC_FREQ 50    // PWM signal frequency in HZ
#define LEDC_RES 10     // PWM signal resolution
#define MIN_PULSE 1000  // Pulse width for minimal position in μs
#define MAX_PULSE 2000  // Pulse width for maximal position in μs
#define SERVO_RANGE 180 // Movment range for servo in degrees

#define MIN_ANGLE (-1 * (SERVO_RANGE / 2))
#define MAX_ANGLE (SERVO_RANGE / 2)
#define LEDC_PERIOD (1000000 / LEDC_FREQ)

const uint8_t servo_pins[4] = {SERVO_PIN_1, SERVO_PIN_2, SERVO_PIN_3, SERVO_PIN_4};

bool init_servos()
{
    ledc_timer_config_t canards_pwm_timer =
        {
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .duty_resolution = LEDC_RES,
            .timer_num = LEDC_TIMER_0,
            .freq_hz = LEDC_FREQ,
            .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&canards_pwm_timer);

    const float start_duty = angle_to_duty(0);
    ledc_channel_config_t ledc_channel[4];
    for (int i = 0; i < 4; i++)
    {
        ledc_channel[i].gpio_num = servo_pins[i];
        ledc_channel[i].speed_mode = LEDC_HIGH_SPEED_MODE;
        ledc_channel[i].channel = i;
        ledc_channel[i].timer_sel = LEDC_TIMER_0;
        ledc_channel[i].intr_type = LEDC_INTR_DISABLE;
        ledc_channel[i].duty = start_duty;
        ledc_channel[i].hpoint = 0;

        ledc_channel_config(&ledc_channel[i]);
    }
    return 1;
}

void update_canards(canards_t *new_canards)
{
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, 0, angle_to_duty(new_canards->canard_angle1));
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, 1, angle_to_duty(new_canards->canard_angle2));
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, 2, angle_to_duty(new_canards->canard_angle3));
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, 3, angle_to_duty(new_canards->canard_angle4));
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, 0);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, 1);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, 2);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, 3);
}

uint32_t angle_to_duty(float angle)
{

    uint32_t duty = (unsigned int)((((((angle - MIN_ANGLE) / SERVO_RANGE) * (MAX_PULSE - MIN_PULSE)) + MIN_PULSE) / LEDC_PERIOD) * (1 << LEDC_RES));
    return duty;
}