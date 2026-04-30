#include "pedometer.h"


#define DEFAULT_GOAL_STEPS  1000U
#define INITIAL_STEPS       0U


static uint32_t step_count = INITIAL_STEPS;
static uint32_t step_goal  = DEFAULT_GOAL_STEPS;


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

void PEDOMETER_add_steps(uint32_t steps)
{
    step_count += steps;
}

void PEDOMETER_remove_steps(uint32_t steps)
{
    if (steps > step_count) {
        step_count = 0;
    } else {
        step_count -= steps;
    }
}
