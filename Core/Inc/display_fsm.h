/*
 * display_fsm.h
 *
 * Defines a state machine for the display.
 */

#ifndef INC_DISP_FSM_H_
#define INC_DISP_FSM_H_


#include "task_joystick_potentiometer.h"

// for size_t
#include <stddef.h>


/*
 * A structure to store a display state. Every item in
 * this struct is a function pointer that is triggered
 * for different events. For example,
 * the 'button_test_tap' function is called when you quickly double-tap SW2.
 */
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


/* STATE SWITCHING FUNCTIONS */

/*
 * Create a new blank state, return it and switch
 * the display FSM to this state. The function
 * calling this one should fill in the returned state.
 */
DISP_FSM_State* DISP_FSM_to_new_state(void);

/*
 * Navigate the display fsm to the previous state. This can
 * be used, for example, to exit test mode or stop showing
 * the goal completed screen.
 */
void DISP_FSM_to_previous_state(void);



/* CALLED BY OTHER MODULES WHEN EVENTS OCCUR
 *
 * Other modules (such as 'task_button_polling') will
 * call these functions when specific events occur (such
 * as button press). These functions will then call
 * the currenet state's function pointer if it is
 * not set to NULL.
 *
 */


/*
 * Called when joystick middle button is pressed for less than 1 second.
 */
void DISP_FSM_trig_joystick_short_press();

/*
 * Called for more than 1 second.
 */
void DISP_FSM_trig_joystick_long_press();

/*
 * Called when SW2 is quickly double-tapped. On
 * most (but not all) states, this enters test mode.
 */
void DISP_FSM_trig_button_test_tap();

/*
 * Called every single time the joystick changes position.
 */
void DISP_FSM_trig_joystick_change_state(
    JOYSTICK_State* old_state,
    JOYSTICK_State* new_state
);

/*
 * Called every time the potentiometer changes. old_pot
 * and new_pot can be used to calculate the direction
 * or speed of change if needed.
 */
void DISP_FSM_trig_potentiometer_change(uint16_t* old_pot, uint16_t* new_pot);

/*
 * Called whenever the 'task_display' runs. The
 * parameter 'characters' should have a string
 * written to it. This string will then be
 * written to the display.
 */
void DISP_FSM_get_display_chars(char* characters, size_t max_chars_length);


#endif /* INC_DISP_FSM_H_ */
