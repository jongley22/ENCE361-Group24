/*
 * state_goal_set.h
 *
 * The FSM state for the goal set screen.
 */

#ifndef STATE_GOAL_SET_H
#define STATE_GOAL_SET_H


#include <stdint.h>
#include <stddef.h>


/*
 * Set the 'new' goal from the current potentiometer
 * value. The 'new' goal is not set until
 * the 'save_new_goal' function is called.
 */
void set_goal_from_pot(uint16_t* old_pot, uint16_t* current_pot);

/*
 * Set the actual goal in the 'pedometer' module to the 'new' goal.
 */
void save_new_goal(void);

/*
 * Draw the goal set screen to the 'display' array.
 */
void render_goal_set(char* display, size_t max_chars_length);


#endif /* STATE_GOAL_SET_H */
