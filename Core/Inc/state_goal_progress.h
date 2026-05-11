/*
 * state_goal_progress.h
 *
 * The FSM state for the goal progress screen.
 */

#ifndef STATE_GOAL_PROGRESS_H
#define STATE_GOAL_PROGRESS_H


#include <stddef.h>


/*
 * Draw the goal progress screen by filling in the 'display' array.
 */
void render_goal_progress(char* display, size_t max_chars_length);


#endif /* STATE_GOAL_PROGRESS_H */
