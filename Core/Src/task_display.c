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


static bool serialIsDebugging = true;


void display_execute(void)
{
    unsigned char x_str[20];
    unsigned char y_str[20];

    uint16_t x = get_joystick_x();
    uint16_t y = get_joystick_y();

    snprintf(x_str, 30, "x = %4hu\r\n", x);
    snprintf(y_str, 30, "y = %4hu\r\n", y);

    ssd1306_SetCursor(0, 20);
    ssd1306_WriteString(x_str, Font_7x10, White);

    ssd1306_SetCursor(0, 40);
    ssd1306_WriteString(y_str, Font_7x10, White);

    ssd1306_UpdateScreen();

    if (serialIsDebugging) {
        HAL_UART_Transmit(&huart2, x_str, strlen(x_str), 10000);
        HAL_UART_Transmit(&huart2, y_str, strlen(y_str), 10000);
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
