#ifndef STATE_GOAL_SET_H
#define STATE_GOAL_SET_H


#include <stdint.h>
#include <stddef.h>


void set_goal_from_pot(uint16_t* old_pot, uint16_t* current_pot);
void save_new_goal(void);
void render_goal_set(char* display, size_t max_chars_length);


#endif /* STATE_GOAL_SET_H */
