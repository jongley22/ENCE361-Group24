/*
 * task_led_blinking.c
 *
 *  Created on: 10/03/2026
 *      Author: jon27
 */
#include "task_led_blinking.h"

#include "gpio.h"


void led_blinking_init(void)
{
}


void led_blinking_execute(void)
{
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}
