#include "display_fsm.h"


static DISP_FSM_State current_state = {
    .on_state_exit = NULL // make sure exit function is not called
};

static DISP_FSM_State previous_state;


static void trigger_func(void(*func)(void));


DISP_FSM_State* DISP_FSM_to_new_state(void)
{
    trigger_func(current_state.on_state_exit);

    // save current state for use in DISP_FSM_previous_state()
    previous_state = current_state;

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
    current_state = previous_state;
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
