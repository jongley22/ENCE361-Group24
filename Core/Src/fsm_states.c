#include "fsm_states.h"

#include "display_fsm.h"

#include "state_current_steps.h"
#include "state_distance_travelled.h"
#include "state_goal_progress.h"
#include "state_goal_set.h"
#include "state_test_mode.h"


// states that display info
static void to_current_steps(void);
static void to_distance_travelled(void);
static void to_goal_progress(void);

// unusual states entered at other times
static void to_goal_set(void);
static void to_test_mode(void);

// state related binding functions
static void save_goal_then_back(void);


void FSM_STATES_to_initial_state(void) {
    to_current_steps();
}

static void save_goal_then_back(void)
{
    save_new_goal();
    to_goal_progress();
}

static void to_current_steps(void)
{
    DISP_FSM_State* state = DISP_FSM_to_new_state();

    state->get_display_chars = &render_current_steps;
    state->button_test_tap = &to_test_mode;
    state->joystick_up = &toggle_step_units;
    state->joystick_left = &to_distance_travelled;
    state->joystick_right = &to_goal_progress;
}

static void to_goal_progress(void)
{
    DISP_FSM_State* state = DISP_FSM_to_new_state();

    state->get_display_chars = &render_goal_progress;
    state->button_test_tap = &to_test_mode;
    state->joystick_left = &to_current_steps;
    state->joystick_long_press = &to_goal_set;
    state->joystick_right = &to_distance_travelled;
}

static void to_distance_travelled(void)
{
    DISP_FSM_State* state = DISP_FSM_to_new_state();

    state->get_display_chars = &render_distance_travelled;
    state->button_test_tap = &to_test_mode;
    state->joystick_up = &toggle_distance_units;
    state->joystick_left = &to_goal_progress;
    state->joystick_right = &to_current_steps;
}

static void to_goal_set(void)
{
    DISP_FSM_State* state = DISP_FSM_to_new_state();

    state->get_display_chars = &render_goal_set;
    state->button_test_tap = NULL;  // disable test mode
    state->potentiometer_change = &set_goal_from_pot;
    state->joystick_short_press = &to_goal_progress;
    state->joystick_long_press = &save_goal_then_back;
}

static void to_test_mode(void)
{
    DISP_FSM_State* state = DISP_FSM_to_new_state();

    state->on_state_exit = &on_test_mode_exit;
    state->get_display_chars = &render_test_mode;
    state->button_test_tap = &DISP_FSM_to_previous_state;
    state->joystick_change_state = &test_mode_joystick_state_change;
}
