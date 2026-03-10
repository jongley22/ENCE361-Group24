/*
 * app.c
 *
 *  Created on: 24/02/2026
 *      Author: jon27
 */

#include "app.h"
#include "task_button_polling.h"
#include "task_led_blinking.h"
#include "task_joystick.h"
#include <stdint.h>
#include "task_display.h"

#define TICK_FREQUENCY_HZ 1000
#define HZ_TO_TICKS(FREQUENCY_HZ) (TICK_FREQUENCY_HZ/FREQUENCY_HZ)

#define BUTTON_POLLING_FREQUENCY_HZ 50
#define LED_BLINKING_FREQUENCY_HZ 2
#define JOYSTICK_FREQUENCY_HZ 50
#define DISPLAY_FREQUENCY_HZ 4

#define BUTTON_POLLING_PERIOD_TICKS (TICK_FREQUENCY_HZ/BUTTON_POLLING_FREQUENCY_HZ)
#define LED_BLINKING_PERIOD_TICKS (TICK_FREQUENCY_HZ/LED_BLINKING_FREQUENCY_HZ)
#define JOYSTICK_PERIOD_TICKS (TICK_FREQUENCY_HZ/JOYSTICK_FREQUENCY_HZ)
#define DISPLAY_PERIOD_TICKS (TICK_FREQUENCY_HZ/DISPLAY_FREQUENCY_HZ)


static uint32_t ButtonPollingNextRun = 0;
static uint32_t LedBlinkingNextRun = 0;
static uint32_t JoystickNextRun = 0;
static uint32_t DisplayNextRun = 0;

void app_main(void)
{
	button_polling_init();
	display_init();
	ButtonPollingNextRun = HAL_GetTick() + BUTTON_POLLING_PERIOD_TICKS;
	LedBlinkingNextRun = HAL_GetTick() + LED_BLINKING_PERIOD_TICKS;
	JoystickNextRun = HAL_GetTick() + JOYSTICK_PERIOD_TICKS;
	DisplayNextRun = HAL_GetTick() + DISPLAY_PERIOD_TICKS;

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

		if (ticks > JoystickNextRun)
		{
			joystick_execute();
			JoystickNextRun += JOYSTICK_PERIOD_TICKS;
		}
		if (ticks > DisplayNextRun)
		{
			display_execute();
			DisplayNextRun += DISPLAY_PERIOD_TICKS;
		}
	}
}
