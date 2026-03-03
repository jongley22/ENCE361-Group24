/*
 * app.c
 *
 *  Created on: 24/02/2026
 *      Author: jon27
 */

#include "app.h"
#include "gpio.h"
#include "buttons.h"
#include "rgb.h"



static void check_a_button(buttonName_t button_name, rgb_led_t rgb_name)
{
	buttonState_t state = buttons_checkButton(button_name);
	if(state == PUSHED)
		rgb_led_on(rgb_name);
	else if(state == RELEASED)
		rgb_led_off(rgb_name);
}

void app_main(void)
{
	buttons_init();
	rgb_colour_all_on();
	while (1) {
		buttons_update();
		HAL_Delay(200);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		check_a_button(DOWN, RGB_DOWN);
		check_a_button(UP, RGB_UP);
		check_a_button(LEFT, RGB_LEFT);
		check_a_button(RIGHT, RGB_RIGHT);
	}
}
