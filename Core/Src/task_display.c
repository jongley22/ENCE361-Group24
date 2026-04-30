/*
 * task_display.c
 *
 *  Created on: 10/03/2026
 *      Author: jon27
 */

#include "task_display.h"

#include "display_fsm.h"

#include <ssd1306_conf.h>
#include <ssd1306_fonts.h>
#include <ssd1306.h>
#include <stm32c0xx_hal_conf.h>
#include <usart.h>

#include <string.h>
#include <stdbool.h>
#include <stdio.h>


#define MAX_CHARS_LENGTH 600


static void write_line(char* line, uint16_t pos);
static void write_lines(char* chars);


void DISPLAY_execute(void)
{
    char chars[MAX_CHARS_LENGTH];
    DISP_FSM_get_display_chars(chars, MAX_CHARS_LENGTH);
    write_lines(chars);
}

void DISPLAY_init(void)
{
    ssd1306_Init();
}

static void write_line(char* line, uint16_t pos)
{
    ssd1306_SetCursor(0, pos);
    ssd1306_WriteString(line, Font_7x10, White);
}

static void write_lines(char* chars)
{
    ssd1306_Fill(Black);

    char current_line[strlen(chars)+1];
    current_line[0] = '\0';

    uint16_t display_pos = 10;
    size_t str_pos = 0;
    for(size_t i=0; chars[i] != '\0'; i++) {
        if(chars[i] == '\n') {
            current_line[str_pos] = '\0';
            write_line(current_line, display_pos);
            current_line[0] = '\0';
            display_pos += 20;
            str_pos = 0;
        } else {
            current_line[str_pos] = chars[i];
            str_pos ++;
        }
    }
    current_line[str_pos] = '\0';
    write_line(current_line, display_pos);

    ssd1306_UpdateScreen();
}
