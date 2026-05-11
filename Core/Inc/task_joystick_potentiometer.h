/*
 * task_joystick_potentiometer.h
 *
 * This module handles reading the analog joystick and potentiometer
 * values from the ADC. Having both of these values in the same
 * module means that there is only one module reading the ADC.
 */

#ifndef INC_TASK_JOYSTICK_POT_H_
#define INC_TASK_JOYSTICK_POT_H_


#include <stdint.h>
#include <stdbool.h>


/*
 * This structure is passed into the
 * current display_fsm state when the
 * joystick changes position.
 */
typedef struct {
    uint16_t percent_x;
    uint16_t percent_y;
    bool is_left;
    bool is_up;
    bool x_at_rest;
    bool y_at_rest;
    bool at_rest;
    bool x_at_max;
    bool y_at_max;
    bool at_max;
} JOYSTICK_State;


void JOYPOT_init(void);

/*
 * Read the joystick and potentiometer values. Then
 * decide what functions need to be called
 * in 'display_fsm'.
 */
void JOYPOT_execute(void);


#endif /* INC_TASK_JOYSTICK_POT_H_ */
