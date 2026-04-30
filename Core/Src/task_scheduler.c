/*
 * task_scheduler.c
 *
 *  Created on: 24/02/2026
 *      Author: jon27
 */

#include "task_scheduler.h"

#include <stm32c0xx_hal.h>

#include <stdint.h>


#define MAX_NUM_TASKS 20
#define TICK_FREQUENCY_HZ 1000
#define HZ_TO_TICKS(FREQUENCY_HZ) (TICK_FREQUENCY_HZ/FREQUENCY_HZ)


typedef struct {
    uint16_t period_ticks;
    uint32_t next_run;
    void(*init_func)(void);
    void(*execute_func)(void);
} Task;

static uint8_t num_tasks = 0;
static Task tasks[MAX_NUM_TASKS];


static void init_tasks(void);


void SCHEDULER_add_task(
    void(*init_func)(void),
    void(*execute_func)(void),
    uint16_t frequency)
{
    Task* task = tasks+num_tasks;
    task->period_ticks = HZ_TO_TICKS(frequency);
    task->period_ticks = HZ_TO_TICKS(frequency);
    task->init_func = init_func;
    task->execute_func = execute_func;
    num_tasks ++;
}

void SCHEDULER_run_tasks(void)
{
    init_tasks();
    Task* task;
    uint32_t ticks;
    while (1) {
        ticks = HAL_GetTick();
        for (task=tasks; task<tasks+num_tasks; task++) {
            if (ticks > task->next_run) {
                (*task->execute_func)();
                task->next_run += task->period_ticks;
            }
        }
    }
}

static void init_tasks(void)
{
    for (Task* task=tasks; task<tasks+num_tasks; task++) {
        (*task->init_func)();
        task->next_run = HAL_GetTick() + task->period_ticks;
    }
}
