/*
 * display_fsm.c
 *
 * Defines a state machine for the display.
 */

#include "display_fsm.h"


/*
 * Used to size the previous_states array. If
 * you try to go back more than 10 times
 * using 'DISP_FSM_to_previous_state', this
 * will result in undefined behaviour.
 */
#define NUM_PREVIOUS_STATES 10


static DISP_FSM_State current_state = {
    .on_state_exit = NULL  // make sure exit function is not called
};

static DISP_FSM_State previous_states[NUM_PREVIOUS_STATES];


static void trigger_func(void(*func)(void));
static void save_previous_state(void);


/*
 * Create a new blank state, return it and switch
 * the display FSM to this state. The function
 * calling this one should fill in the returned state.
 */
DISP_FSM_State* DISP_FSM_to_new_state(void)
{
    trigger_func(current_state.on_state_exit);

    save_previous_state();

    current_state.joystick_short_press = NULL;
    current_state.joystick_long_press = NULL;
    current_state.button_test_tap = NULL;
    current_state.joystick_change_state = NULL;
    current_state.potentiometer_change = NULL;
    current_state.joystick_up = NULL;
    current_state.joystick_down = NULL;
    current_state.joystick_left = NULL;
    current_state.joystick_right = NULL;
    current_state.get_display_chars = NULL;
    current_state.on_state_exit = NULL;

    return &current_state;
}

/*
 * Navigate the display fsm to the previous state. This can
 * be used, for example, to exit test mode or stop showing
 * the goal completed screen.
 */
void DISP_FSM_to_previous_state(void)
{
    trigger_func(current_state.on_state_exit);
    current_state = previous_states[0];

    // shift everything left (opposite of save_state operation)
    for(uint8_t i=0; i<NUM_PREVIOUS_STATES-1; i++) {
        previous_states[i] = previous_states[i+1];
    }
}

/*
 * Called when joystick middle button is pressed for less than 1 second.
 */
void DISP_FSM_trig_joystick_short_press(void)
{
    trigger_func(current_state.joystick_short_press);
}

/*
 * Called for more than 1 second.
 */
void DISP_FSM_trig_joystick_long_press(void)
{
    trigger_func(current_state.joystick_long_press);
}

/*
 * Called when SW2 is quickly double-tapped. On
 * most (but not all) states, this enters test mode.
 */
void DISP_FSM_trig_button_test_tap(void)
{
    trigger_func(current_state.button_test_tap);
}

/*
 * Called every single time the joystick changes position.
 */
void DISP_FSM_trig_joystick_change_state(
    JOYSTICK_State* old_state,
    JOYSTICK_State* new_state)
{
    // Check if joystick has just reached maximum position.
    if (!old_state->at_max && new_state->at_max) {
        // Figure out whether to call up, down, left or right.

        if (new_state->x_at_max) {
            if (new_state->is_left) {
                trigger_func(current_state.joystick_left);
            } else {
                trigger_func(current_state.joystick_right);
            }
        }
        if (new_state->y_at_max) {
            if (new_state->is_up) {
                trigger_func(current_state.joystick_up);
            } else {
                trigger_func(current_state.joystick_down);
            }
        }
    }

    if (current_state.joystick_change_state != NULL) {
        (*current_state.joystick_change_state)(old_state, new_state);
    }
}

/*
 * Called every time the potentiometer changes. old_pot
 * and new_pot can be used to calculate the direction
 * or speed of change if needed.
 */
void DISP_FSM_trig_potentiometer_change(uint16_t* old_pot, uint16_t* new_pot)
{
    if (current_state.potentiometer_change != NULL) {
        (*current_state.potentiometer_change)(old_pot, new_pot);
    }
}

/*
 * Called whenever the 'task_display' runs. The
 * parameter 'characters' should have a string
 * written to it. This string will then be
 * written to the display.
 */
void DISP_FSM_get_display_chars(char* characters, size_t max_chars_length)
{
    if (current_state.get_display_chars != NULL) {
        (*current_state.get_display_chars)(characters, max_chars_length);
    }
}

/*
 * When an event occurs and a function in the current
 * state needs to be called, this function is used to
 * call that function pointer. Because not all
 * functions are used in every state (such as
 * potentiometer being ignored in current steps
 * screen), this function only calls the pointer
 * if it is not NULL.
 */
static void trigger_func(void(*func)(void))
{
    if (func != NULL) {
        (*func)();
    }
}

/*
 * Add to the previous_states array the current
 * state. This is used so that
 * the 'DISP_FSM_to_previous_state' function
 * has states to roll back to.
 */
static void save_previous_state(void)
{
    // shift everything to the right, making 0 empty
    for(uint8_t i=NUM_PREVIOUS_STATES-1; i>0; i--) {
        previous_states[i] = previous_states[i-1];
    }

    // write to 0
    previous_states[0] = current_state;
}
