/*
 * state_current_steps.h
 *
 * The FSM state for the current steps screen.
 */

#ifndef STATE_CURRENT_STEPS_H
#define STATE_CURRENT_STEPS_H


#include <stddef.h>


/*
 * Toggles the units between the number of
 * steps and percentage of the goal reached.
 */
void toggle_step_units(void);

/*
 * Fill in the 'display' array with the current steps string.
 */
void render_current_steps(char* display, size_t max_chars_length);


#endif /* STATE_CURRENT_STEPS_H */
