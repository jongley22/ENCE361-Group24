/*
 * task_button_polling.h
 *
 *  Created on: 24/02/2026
 *      Author: jon27
 */

#ifndef INC_TASK_JOYSTICK_POT_H_
#define INC_TASK_JOYSTICK_POT_H_


#include <stdint.h>
#include <stdbool.h>


typedef struct {
    uint16_t percent_x;
    uint16_t percent_y;
    bool is_left;
    bool is_up;
    bool x_at_rest;
    bool y_at_rest;
    bool at_rest;
} JOYSTICK_State;


void JOYPOT_init(void);
void JOYPOT_execute(void);


#endif /* INC_TASK_JOYSTICK_POT_H_ */
