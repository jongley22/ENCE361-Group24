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

#define TICK_FREQUENCY_HZ 1000
#define HZ_TO_TICKS(FREQUENCY_HZ) (TICK_FREQUENCY_HZ/FREQUENCY_HZ)

#define BUTTON_POLLING_FREQUENCY_HZ 50
#define LED_BLINKING_FREQUENCY_HZ 2

#define BUTTON_POLLING_PERIOD_TICKS (TICK_FREQUENCY_HZ/BUTTON_POLLING_FREQUENCY_HZ)
#define LED_BLINKING_PERIOD_TICKS (TICK_FREQUENCY_HZ/LED_BLINKING_FREQUENCY_HZ)

static uint32_t ButtonPollingNextRun = 0;
static uint32_t LedBlinkingNextRun = 0;

void led_blinking_execute(void);
void button_polling_execute(void);

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
	ButtonPollingNextRun = HAL_GetTick() + BUTTON_POLLING_PERIOD_TICKS;
	LedBlinkingNextRun = HAL_GetTick() + LED_BLINKING_PERIOD_TICKS;
	while (1) {
		uint32_t ticks = HAL_GetTick();
		if(ticks > ButtonPollingNextRun)
		{
			button_polling_execute();
			ButtonPollingNextRun += BUTTON_POLLING_PERIOD_TICKS;
		}

		if (ticks > LedBlinkingNextRun)
		{
			led_blinking_execute();
			LedBlinkingNextRun += LED_BLINKING_PERIOD_TICKS;
		}
	}
}

void button_polling_execute(void)
{
	buttons_update();
	check_a_button(DOWN, RGB_DOWN);
	check_a_button(UP, RGB_UP);
	check_a_button(LEFT, RGB_LEFT);
	check_a_button(RIGHT, RGB_RIGHT);
}

void led_blinking_execute(void)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}
