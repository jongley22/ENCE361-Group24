/*
 * app.c
 *
 *  Created on: 24/02/2026
 *      Author: jon27
 */

#include "app.h"
#include "gpio.h"
#include "rgb.h"
#include "stdint.h"

void app_main(void)
{
	while (1) {
		HAL_Delay(700);
		//rgb_led_toggle(RGB_RIGHT);
		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

		/*if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin)) {
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
		}
		else {
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
		}*/
	}
}
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	static unsigned int count =0;
	if (GPIO_Pin & GPIO_PIN_10)
	{
		count++;
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
}
