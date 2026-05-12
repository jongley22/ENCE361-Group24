/*
 * task_led_blinking.c
 *
 * A task to blink the builtin LED so you can easily
 * tell when the task scheduler is working.
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
