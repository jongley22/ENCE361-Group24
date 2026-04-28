#include "pedometer.h"
#define DEFAULT_GOAL_STEPS  1000U
#define INITIAL_STEPS       0U

static uint32_t stepCount = INITIAL_STEPS;
static uint32_t stepGoal  = DEFAULT_GOAL_STEPS;


uint32_t Pedometer_GetSteps(void)
{
    return stepCount;
}
void Pedometer_SetGoal(uint32_t newGoal)
{
    stepGoal = newGoal;
}

uint32_t Pedometer_GetGoal(void)
{
    return stepGoal;
}
void Pedometer_AddSteps(uint32_t steps)
{
    stepCount += steps;
}
void Pedometer_RemoveSteps(uint32_t steps)
{
    if (steps > stepCount) {
        stepCount = 0;
    } else {
        stepCount -= steps;
    }
}

