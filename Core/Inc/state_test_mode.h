#ifndef STATE_TEST_MODE_H
#define STATE_TEST_MODE_H


#include "task_joystick_potentiometer.h"

#include <stddef.h>


void TEST_MODE_called_at_frequency();
void on_test_mode_exit(void);

void test_mode_joystick_state_change(
    JOYSTICK_State* old_state,
    JOYSTICK_State* new_state
);

void render_test_mode(char* display, size_t max_chars_length);


#endif /* STATE_TEST_MODE_H */
