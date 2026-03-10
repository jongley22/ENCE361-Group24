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



void display_execute(void)
{
	char x_str[30];
	char y_str[30];

	uint16_t* raw_adc = get_raw_adc();

	snprintf(x_str, 30, "x = %hu", raw_adc[0]);
	snprintf(y_str, 30, "y = %hu", raw_adc[1]);

	ssd1306_SetCursor(0, 20);
	ssd1306_WriteString(x_str, Font_7x10, White);

	ssd1306_SetCursor(0, 40);
	ssd1306_WriteString(y_str, Font_7x10, White);

    ssd1306_UpdateScreen();
}

void display_init(void)
{
	ssd1306_Init();
	ssd1306_SetCursor(0,0);
	ssd1306_WriteString("Hello world!", Font_7x10, White);
}
