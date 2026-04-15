/*
 * task_display.c
 *
 *  Created on: 10/03/2026
 *      Author: jon27
 */
#include "task_display.h"
#include "task_joystick.h"
#include "pedometer.h"

#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
#include "ssd1306.h"
#include "stm32c0xx_hal_conf.h"
#include "usart.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>


static bool serialIsDebugging = false;
static bool testModeActive    = false;


static void render_joystick_values(
    char* xy_str,
    char* percent_str,
    char* direction_str,
    uint16_t lengths
);
static void render_test_mode(void);


static void render_joystick_values(
    char* xy_str,
    char* percent_str,
    char* direction_str,
    uint16_t lengths)
{
    char left_right[10];
    char up_down[10];

    uint16_t x = get_joystick_x();
    uint16_t y = get_joystick_y();

    int16_t percent_x = get_joystick_x_percent();
    int16_t percent_y = get_joystick_y_percent();

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

    snprintf(xy_str,        lengths, "x,y=(%4hu, %4hu)\r\n",    x, y);
    snprintf(percent_str,   lengths, "x,y=(%4hd%%,%4hd%%)\r\n", percent_x, percent_y);
    snprintf(direction_str, lengths, "x,y=(%s, %s)      \r\n",  left_right, up_down);
}


static void render_test_mode(void)
{
    char steps_str[24];
    char goal_str[24];

    snprintf(steps_str, sizeof(steps_str), "Steps: %lu", Pedometer_GetSteps());
    snprintf(goal_str,  sizeof(goal_str),  "Goal:  %lu", Pedometer_GetGoal());

    ssd1306_Fill(Black);

    ssd1306_SetCursor(0, 10);
    ssd1306_WriteString("-- TEST MODE --", Font_7x10, White);

    ssd1306_SetCursor(0, 25);
    ssd1306_WriteString(steps_str, Font_7x10, White);

    ssd1306_SetCursor(0, 40);
    ssd1306_WriteString(goal_str, Font_7x10, White);

    ssd1306_UpdateScreen();
}


void display_execute(void)
{
    if (testModeActive)
    {
        render_test_mode();
        return;
    }

    uint16_t lengths = 40;
    char xy_str[lengths];
    char percent_str[lengths];
    char direction_str[lengths];

    render_joystick_values(xy_str, percent_str, direction_str, lengths);

    ssd1306_SetCursor(0, 10);
    ssd1306_WriteString(xy_str, Font_7x10, White);

    ssd1306_SetCursor(0, 20);
    ssd1306_WriteString(percent_str, Font_7x10, White);

    ssd1306_SetCursor(0, 30);
    ssd1306_WriteString(direction_str, Font_7x10, White);

    ssd1306_UpdateScreen();

    if (serialIsDebugging) {
        HAL_UART_Transmit(&huart2, xy_str,        strlen(xy_str),        10000);
        HAL_UART_Transmit(&huart2, percent_str,   strlen(percent_str),   10000);
        HAL_UART_Transmit(&huart2, direction_str, strlen(direction_str), 10000);
    }
}


void display_init(void)
{
    ssd1306_Init();
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Hello world!", Font_7x10, White);
    ssd1306_UpdateScreen();
}


void toggle_serial_debug(void)
{
    serialIsDebugging = !serialIsDebugging;
}

void display_toggle_test_mode(void)
{
    testModeActive = !testModeActive;
}
bool display_is_test_mode(void)             /* M2.3 */
{
    return testModeActive;
}
