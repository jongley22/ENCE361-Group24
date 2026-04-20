/*
 * task_display.h
 *
 *  Created on: 24/02/2026
 *      Author: jon27
 */

#ifndef INC_TASK_DISPLAY_H_
#define INC_TASK_DISPLAY_H_

#include <stdbool.h>

void display_execute(void);
void display_init(void);
void toggle_serial_debug(void);
void display_toggle_test_mode(void);
bool display_is_test_mode(void);
void next_display_screen(bool);
void display_unit_switch(void);
void display_joystick_long_press(void);
void display_joystick_short_press(void);

#endif /* INC_TASK_DISPLAY_H_ */
