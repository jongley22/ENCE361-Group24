/*
 * task_scheduler.h
 *
 * A time-based task scheduler implementation using the free-running
 * timer. Every task can have an independant frequency, but the
 * frequency of a task cannot be changed at runtime.
 */

#ifndef INC_TASK_SCHEDULER_H
#define INC_TASK_SCHEDULER_H


#include <stdint.h>


/*
 * Run the task scheduler. This function blocks
 * forever with a while(1) loop.
 */
void SCHEDULER_run_tasks(void);

/*
 * Add a task to the scheduler. This is used at
 * program initialization.
 */
void SCHEDULER_add_task(

    /*
     * called once at beginning of program.
     */
    void(*init_func)(void),

    /*
     * Called at the requested frequency.
     */
    void(*execute_func)(void),

    /*
     * Should be in HZ.
     */
    uint16_t frequency
);


#endif /* INC_TASK_SCHEDULER_H_ */
