#ifndef PEDOMETER_H
#define PEDOMETER_H

#include <stdint.h>


float    PEDOMETER_get_distance(void);

uint32_t PEDOMETER_get_steps(void);
void     PEDOMETER_set_steps(uint32_t steps);
void     PEDOMETER_add_steps(uint32_t steps);
void     PEDOMETER_remove_steps(uint32_t steps);

uint32_t PEDOMETER_get_goal(void);
void     PEDOMETER_set_goal(uint32_t goal);
uint32_t PEDOMETER_get_goal_percent(void);


#endif /* PEDOMETER_H */
