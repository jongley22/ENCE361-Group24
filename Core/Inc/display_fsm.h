/*
 * task_display.h
 *
 *  Created on: 24/02/2026
 *      Author: jon27
 */

#ifndef INC_DISP_FSM_H_
#define INC_DISP_FSM_H_


#include "task_joystick_potentiometer.h"

#include <stddef.h>


typedef struct {
    void(*joystick_short_press)(void);
    void(*joystick_long_press)(void);
    void(*button_test_tap)(void);
    void(*joystick_change_state)(JOYSTICK_State* old_state, JOYSTICK_State* new_state);
    void(*joystick_up)(void);
    void(*joystick_down)(void);
    void(*joystick_left)(void);
    void(*joystick_right)(void);
    void(*potentiometer_change)(uint16_t* old_pot, uint16_t* new_pot);
    void(*get_display_chars)(char* characters, size_t max_chars_length);
    void(*on_state_exit)(void);
} DISP_FSM_State;


/* State Switching Functions */
DISP_FSM_State* DISP_FSM_to_new_state(void);
void DISP_FSM_to_previous_state(void);

/* Called by other modules when events occur */
void DISP_FSM_trig_joystick_short_press();
void DISP_FSM_trig_joystick_long_press();
void DISP_FSM_trig_button_test_tap();
void DISP_FSM_trig_joystick_change_state(JOYSTICK_State* old_state, JOYSTICK_State* new_state);
void DISP_FSM_trig_potentiometer_change(uint16_t* old_pot, uint16_t* new_pot);
void DISP_FSM_get_display_chars(char* characters, size_t max_chars_length);


#endif /* INC_DISP_FSM_H_ */
