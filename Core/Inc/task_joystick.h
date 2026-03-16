/*
 * task_button_polling.h
 *
 *  Created on: 24/02/2026
 *      Author: jon27
 */

#ifndef INC_TASK_JOYSTICK_H_
#define INC_TASK_JOYSTICK_H_

#include <stdint.h>

#define JOYSTICK_X_MIN 200
#define JOYSTICK_Y_MIN 240
#define JOYSTICK_X_MAX 4095
#define JOYSTICK_Y_MAX 4095


void joystick_execute(void);
uint16_t get_joystick_x(void);
uint16_t get_joystick_y(void);


#endif /* INC_TASK_JOYSTICK_H_ */
