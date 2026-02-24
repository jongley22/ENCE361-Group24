/*
 * app.c
 *
 *  Created on: 24/02/2026
 *      Author: jon27
 */

#include "app.h"
#include "gpio.h"

void app_main(void)
{
	while (1) {
		HAL_Delay(200);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

		if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin)) {
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
		}
		else {
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
		}
	}
}
