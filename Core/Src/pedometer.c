#include "pedometer.h"


#define DEFAULT_GOAL_STEPS  1000U
#define INITIAL_STEPS       0U


static uint32_t step_count = INITIAL_STEPS;
static uint32_t step_goal  = DEFAULT_GOAL_STEPS;


/*
 * Calculate and return how far the user has reached
 * their goal as a percentage. The number returned
 * is an integer to the nearest
 * percent (e.g. integer 60 means 60%).
 */
uint32_t PEDOMETER_get_goal_percent(void)
{
    uint32_t steps = PEDOMETER_get_steps();
    uint32_t goal = PEDOMETER_get_goal();
    return (steps * 100) / goal;
}

uint32_t PEDOMETER_get_steps(void)
{
    return step_count;
}

/*
 * The value 'steps' replaces the current step count. No checking
 * is done in relation to the goal (ie. you can supply a value
 * greater than the current goal and it will be set).
 */
void PEDOMETER_set_steps(uint32_t steps)
{
    step_count = steps;
}

void PEDOMETER_set_goal(uint32_t new_goal)
{
    step_goal = new_goal;
}

uint32_t PEDOMETER_get_goal(void)
{
    return step_goal;
}

/*
 * Add 'steps' to the current count.
 *
 * No checking is done in relation to the current goal.
 */
void PEDOMETER_add_steps(uint32_t steps)
{
    step_count += steps;
}

/*
 * Subtract 'steps' from the current count.
 *
 * If 'steps' is greater than the current step
 * count, 0 is set (ie. overflow is prevented
 * automatically by this function).
 */
void PEDOMETER_remove_steps(uint32_t steps)
{
    if (steps > step_count) {
        step_count = 0;
    } else {
        step_count -= steps;
    }
}
