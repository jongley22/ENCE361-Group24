/*
 * state_test_mode.h
 *
 * The FSM state for the test mode screen.
 *
 * This also has a task called from the task scheduler.
 */

#ifndef STATE_TEST_MODE_H
#define STATE_TEST_MODE_H


#include "task_joystick_potentiometer.h"

#include <stddef.h>


/*
 * This function is called from the task
 * scheduler. This function will increment
 * the current steps value up or down
 * depending on the joystick position. If
 * the joystick is at reset or the test
 * mode screen is not active, this
 * function will do nothing.
 */
void TEST_MODE_called_at_frequency();

/*
 * This function is called when the test mode screen
 * exits. It is needed so that
 * the 'TEST_MODE_called_at_frequency' function
 * will stop incrementing the current steps when
 * if test mode is exitted when the joystick is
 * held up or down.
 */
void on_test_mode_exit(void);

/*
 * This function is called when the joystick is
 * moved. It calculates the
 * amount 'TEST_MODE_called_at_frequency' should
 * increment (or decrement) the current
 * steps each call.
 */
void test_mode_joystick_state_change(
    JOYSTICK_State* old_state,
    JOYSTICK_State* new_state
);

/*
 * Draw the test mode string in the 'display' array.
 */
void render_test_mode(char* display, size_t max_chars_length);


#endif /* STATE_TEST_MODE_H */
