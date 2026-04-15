#include "pedometer.h"
#define DEFAULT_GOAL_STEPS  1000U
#define INITIAL_STEPS       0U
#define INITIAL_DISTANCE    0.0f

static uint32_t stepCount = INITIAL_STEPS;
static float    distance  = INITIAL_DISTANCE;
static uint32_t stepGoal  = DEFAULT_GOAL_STEPS;


uint32_t Pedometer_GetSteps(void)
{
    return stepCount;
}

float Pedometer_GetDistance(void)
{
    return distance;
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
