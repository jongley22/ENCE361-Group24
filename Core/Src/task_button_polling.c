#include "task_button_polling.h"

#include "buttons.h"
#include "rgb.h"


static void check_a_button(buttonName_t button_name, rgb_led_t rgb_name);


void button_polling_init(void)
{
	buttons_init();
	rgb_colour_all_on();
}



void button_polling_execute(void)
{
	buttons_update();
	check_a_button(DOWN, RGB_DOWN);
	check_a_button(UP, RGB_UP);
	check_a_button(LEFT, RGB_LEFT);
	check_a_button(RIGHT, RGB_RIGHT);
}


static void check_a_button(buttonName_t button_name, rgb_led_t rgb_name)
{
	buttonState_t state = buttons_checkButton(button_name);
	if(state == PUSHED)
		rgb_led_on(rgb_name);
	else if(state == RELEASED)
		rgb_led_off(rgb_name);
}
