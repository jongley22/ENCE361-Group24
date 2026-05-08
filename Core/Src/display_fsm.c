#include "display_fsm.h"


#define NUM_PREVIOUS_STATES 10


static DISP_FSM_State current_state = {
    .on_state_exit = NULL // make sure exit function is not called
};

static DISP_FSM_State previous_states[NUM_PREVIOUS_STATES];


static void trigger_func(void(*func)(void));
static void save_previous_state(void);


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

void DISP_FSM_to_previous_state(void)
{
    trigger_func(current_state.on_state_exit);
    current_state = previous_states[0];

    // shift everything left (opposite of save_state operation)
    for(uint8_t i=0; i<NUM_PREVIOUS_STATES-1; i++) {
        previous_states[i] = previous_states[i+1];
    }
}

void DISP_FSM_trig_joystick_short_press(void)
{
    trigger_func(current_state.joystick_short_press);
}

void DISP_FSM_trig_joystick_long_press(void)
{
    trigger_func(current_state.joystick_long_press);
}

void DISP_FSM_trig_button_test_tap(void)
{
    trigger_func(current_state.button_test_tap);
}

void DISP_FSM_trig_joystick_change_state(JOYSTICK_State* old_state, JOYSTICK_State* new_state)
{
    if (!old_state->at_max && new_state->at_max) {
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

void DISP_FSM_trig_potentiometer_change(uint16_t* old_pot, uint16_t* new_pot)
{
    if (current_state.potentiometer_change != NULL) {
        (*current_state.potentiometer_change)(old_pot, new_pot);
    }
}

void DISP_FSM_get_display_chars(char* characters, size_t max_chars_length)
{
    if (current_state.get_display_chars != NULL) {
        (*current_state.get_display_chars)(characters, max_chars_length);
    }
}

static void trigger_func(void(*func)(void))
{
    if (func != NULL) {
        (*func)();
    }
}

static void save_previous_state(void)
{
    // shift everything to the right, making 0 empty
    for(uint8_t i=NUM_PREVIOUS_STATES-1; i>0; i--) {
        previous_states[i] = previous_states[i-1];
    }

    // write to 0
    previous_states[0] = current_state;
}
