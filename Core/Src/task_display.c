/*
 * task_display.c
 *
 *  Created on: 10/03/2026
 *      Author: jon27
 */
#include "task_display.h"
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
#include "stm32c0xx_hal_conf.h"
#include "task_joystick.h"
#include "usart.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>


static bool serialIsDebugging = false;


void display_execute(void)
{
    char xy_str[40];
    char percent_str[40];
    char direction_str[40];

    char left_right[10];
    char up_down[10];

    uint16_t x = get_joystick_x();
    uint16_t y = get_joystick_y();

    int16_t diff_x = (int16_t)((JOYSTICK_X_MAX - JOYSTICK_X_MIN) / 2);
    int16_t diff_y = (int16_t)((JOYSTICK_Y_MAX - JOYSTICK_Y_MIN) / 2);
    int16_t percent_x = ((x - JOYSTICK_X_MIN - diff_x) * 100) / diff_x;
    int16_t percent_y = ((y - JOYSTICK_Y_MIN - diff_y) * 100) / diff_y;

    if (percent_x < 10 && percent_x > -10) {
        snprintf(left_right, 10, "rest");
	} else if (percent_x > 0) {
        snprintf(left_right, 10, "left");
    } else {
        snprintf(left_right, 10, "right");
    }

    if (percent_y < 10 && percent_y > -10) {
        snprintf(up_down, 10, "rest");
    } else if (percent_y > 0) {
        snprintf(up_down, 10, "down");
    } else {
        snprintf(up_down, 10, "up");
    }

    snprintf(xy_str, 40, "x,y=(%4hu, %4hu)\r\n", x, y);
    snprintf(percent_str, 40, "x,y=(%4hd, %4hd)\r\n", percent_x, percent_y);
    snprintf(direction_str, 40, "x,y=(%s, %s)      \r\n", left_right, up_down);

    ssd1306_SetCursor(0, 10);
    ssd1306_WriteString(xy_str, Font_7x10, White);

    ssd1306_SetCursor(0, 20);
    ssd1306_WriteString(percent_str, Font_7x10, White);

    ssd1306_SetCursor(0, 30);
    ssd1306_WriteString(direction_str, Font_7x10, White);

    ssd1306_UpdateScreen();

    if (serialIsDebugging) {
        HAL_UART_Transmit(&huart2, xy_str, strlen(xy_str), 10000);
        HAL_UART_Transmit(&huart2, percent_str, strlen(percent_str), 10000);
        HAL_UART_Transmit(&huart2, direction_str, strlen(direction_str), 10000);
    }
}


void display_init(void)
{
    ssd1306_Init();
    ssd1306_SetCursor(0,0);
    ssd1306_WriteString("Hello world!", Font_7x10, White);
}


void toggle_serial_debug(void)
{
    serialIsDebugging = !serialIsDebugging;
}
