#include "task_button_polling.h"

#include "buttons.h"
#include "rgb.h"
#include "tim.h"
#include "pwm.h"
static void check_a_button(buttonName_t button_name, rgb_led_t rgb_name);

#define PWM_MAX_DUTY_CYCLE 100
#define PWM_ADD_DUTY_CYCLE 25


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


static void pwm_increase()
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

	if(state == PUSHED){
		if(button_name == UP) {
			pwm_increase();
		} else {
			rgb_led_on(rgb_name);
		}
	}
	else if(state == RELEASED) {
		rgb_led_off(rgb_name);
	}
}
