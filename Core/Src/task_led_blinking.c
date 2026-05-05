/*
 * task_led_blinking.c
 *
 *  Created on: 10/03/2026
 *      Author: jon27
 */

#include "task_led_blinking.h"

#include <gpio.h>


void LED_BLINKING_init(void)
{
}

void LED_BLINKING_execute(void)
{
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}
