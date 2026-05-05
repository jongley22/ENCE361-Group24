/*
 * app.h
 *
 *  Created on: 24/02/2026
 *      Author: jon27
 */

#ifndef INC_TASK_SCHEDULER_H
#define INC_TASK_SCHEDULER_H


#include <stdint.h>


void SCHEDULER_run_tasks(void);

void SCHEDULER_add_task(
    void(*init_func)(void),
    void(*execute_func)(void),
    uint16_t frequency
);


#endif /* INC_TASK_SCHEDULER_H_ */
