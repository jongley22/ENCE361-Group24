#include "task_led_blinking.h"
#include "gpio.h"


void led_blinking_execute(void)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}
