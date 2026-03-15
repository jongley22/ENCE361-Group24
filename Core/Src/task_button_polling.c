#include "task_button_polling.h"

#include "task_display.h"

#include "buttons.h"
#include "rgb.h"
#include "tim.h"
#include "pwm.h"

#define PWM_MAX_DUTY_CYCLE 100
#define PWM_ADD_DUTY_CYCLE 10


static void check_a_button(buttonName_t button_name, rgb_led_t rgb_name);
static void pwm_increase(void);


void button_polling_init(void)
{
    buttons_init();
    rgb_colour_all_on();
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
}


void button_polling_execute(void)
{
    buttons_update();
    check_a_button(DOWN, RGB_DOWN);
    check_a_button(UP, RGB_UP);
    check_a_button(LEFT, RGB_LEFT);
    check_a_button(RIGHT, RGB_RIGHT);
}


static void pwm_increase(void)
{
    uint8_t dutyCycle = pwm_getDutyCycle(&htim2, TIM_CHANNEL_3);
    dutyCycle += PWM_ADD_DUTY_CYCLE;
    if (dutyCycle > PWM_MAX_DUTY_CYCLE) {
        dutyCycle = 0;
    }
    pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, dutyCycle);
}


static void check_a_button(buttonName_t button_name, rgb_led_t rgb_name)
{
    buttonState_t state = buttons_checkButton(button_name);

    if (button_name == UP)
    {
        if (state == PUSHED) {
            pwm_increase();
        }
    } else if (state == PUSHED){
        rgb_led_on(rgb_name);
        if (button_name == DOWN) {
            toggle_serial_debug();
        }
    } else if(state == RELEASED) {
        rgb_led_off(rgb_name);
    }
}
