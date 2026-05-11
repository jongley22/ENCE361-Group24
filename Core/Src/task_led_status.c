#include "task_led_status.h"

#include "pwm.h"
#include "rgb.h"
#include "pedometer.h"


#define PWM_MAX_DUTY_CYCLE 100


static void up_led_dutycycle(void);


void LED_STATUS_init(void)
{
    rgb_colour_all_on();
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
}

/*
 * Activating the LEDS combinations for the relevant goal progress
 */
void LED_STATUS_execute(void)
{
    up_led_dutycycle();
    if (PEDOMETER_get_goal_percent() > 75) {
        rgb_led_on(RGB_LEFT);
        rgb_led_on(RGB_DOWN);
        rgb_led_on(RGB_RIGHT);
    } else if (PEDOMETER_get_goal_percent() > 50) {
        rgb_led_off(RGB_LEFT);
        rgb_led_on(RGB_DOWN);
        rgb_led_on(RGB_RIGHT);
    } else if (PEDOMETER_get_goal_percent() > 25) {
        rgb_led_off(RGB_LEFT);
        rgb_led_off(RGB_DOWN);
        rgb_led_on(RGB_RIGHT);
    } else if (PEDOMETER_get_goal_percent() > 0) {
        rgb_led_off(RGB_LEFT);
        rgb_led_off(RGB_DOWN);
        rgb_led_off(RGB_RIGHT);
    }
}

/*
 * Calculating the duty cycle for the UP led PWM for brightness
 */
static void up_led_dutycycle(void) {
    if (PEDOMETER_get_goal_percent() < 25){
        // Assuming Max Duty_cycle is 100
        uint8_t Duty_cycle = (PEDOMETER_get_goal_percent()*4);
        pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, Duty_cycle);
    } else {
        // turn on constantly
        pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, PWM_MAX_DUTY_CYCLE);
    }
}
