#ifndef PEDOMETER_H
#define PEDOMETER_H

#include <stdint.h>

/* Default values */


/* Function prototypes */
uint32_t Pedometer_GetSteps(void);
float    Pedometer_GetDistance(void);
uint32_t Pedometer_GetGoal(void);
void     Pedometer_AddSteps(uint32_t steps);
void     Pedometer_RemoveSteps(uint32_t steps);
void     Pedometer_SetGoal(uint32_t goal);
#endif /* PEDOMETER_H */
