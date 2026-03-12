/*
 * task_display.c
 *
 *  Created on: 10/03/2026
 *      Author: jon27
 */
#include  "task_display.h"
#include  "ssd1306_conf.h"
#include  "ssd1306_fonts.h"
#include  "stm32c0xx_hal_conf.h"
#include "task_joystick.h"
#include <stdio.h>
#include "usart.h"



void display_execute(void)
{
	char x_str[20];
	char y_str[20];

	uint16_t* raw_adc = get_raw_adc();

	snprintf(x_str, 30, "x = %4hu\r\n", raw_adc[0]);
	snprintf(y_str, 30, "y = %4hu\r\n", raw_adc[1]);

	ssd1306_SetCursor(0, 20);
	ssd1306_WriteString(x_str, Font_7x10, White);

	ssd1306_SetCursor(0, 40);
	ssd1306_WriteString(y_str, Font_7x10, White);

    ssd1306_UpdateScreen();
    //HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout)
    HAL_UART_Transmit(&huart2, x_str, strlen(x_str), 10000);
    HAL_UART_Transmit(&huart2, y_str, strlen(y_str), 10000);

}

void display_init(void)
{
	ssd1306_Init();
	ssd1306_SetCursor(0,0);
	ssd1306_WriteString("Hello world!", Font_7x10, White);
}
