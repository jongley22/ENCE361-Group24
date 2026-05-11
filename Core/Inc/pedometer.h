/*
 * pedometer.h
 *
 * Stores the current step count and current goal. Also
 * provides functions to calculate information from these
 * values (such as goal percentage).
 */

#ifndef PEDOMETER_H
#define PEDOMETER_H

#include <stdint.h>


uint32_t PEDOMETER_get_steps(void);

/*
 * The value 'steps' replaces the current step count. No checking
 * is done in relation to the goal (ie. you can supply a value
 * greater than the current goal and it will be set).
 */
void PEDOMETER_set_steps(uint32_t steps);

/*
 * Add 'steps' to the current count.
 *
 * No checking is done in relation to the current goal.
 */
void PEDOMETER_add_steps(uint32_t steps);

/*
 * Subtract 'steps' from the current count.
 *
 * If 'steps' is greater than the current step
 * count, 0 is set (ie. overflow is prevented
 * automatically by this function).
 */
void PEDOMETER_remove_steps(uint32_t steps);

uint32_t PEDOMETER_get_goal(void);

void PEDOMETER_set_goal(uint32_t goal);

/*
 * Calculate and return how far the user has reached
 * their goal as a percentage. The number returned
 * is an integer to the nearest
 * percent (e.g. integer 60 means 60%).
 */
uint32_t PEDOMETER_get_goal_percent(void);


#endif /* PEDOMETER_H */
