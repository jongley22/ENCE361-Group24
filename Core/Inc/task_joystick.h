/*
 * task_button_polling.h
 *
 *  Created on: 24/02/2026
 *      Author: jon27
 */

#ifndef INC_TASK_JOYSTICK_H_
#define INC_TASK_JOYSTICK_H_
#include <stdint.h>


void joystick_init(void);
void joystick_execute(void);

uint16_t get_joystick_x(void);
uint16_t get_joystick_y(void);

int16_t get_joystick_x_percent(void);
int16_t get_joystick_y_percent(void);


#endif /* INC_TASK_JOYSTICK_H_ */
