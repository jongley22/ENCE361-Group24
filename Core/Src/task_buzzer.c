/*
 * task_led_blinking.c
 *
 *  Created on: 10/03/2026
 *      Author: jon27
 */

#include "task_buzzer.h"

#include "pedometer.h"

#include "pwm.h"
#include <stdbool.h>


#define BUZZER_DURATION_TICKS 4


static bool buzzing_now = false;
static uint8_t buzzing_tick_count = 0;


static void buzzer_start(void);
static void buzzer_stop(void);
static void start_buzzing_if_needed(void);
static void stop_buzzing_if_needed(void);


void BUZZER_init(void)
{
}

void BUZZER_execute(void)
{
    if(buzzing_now) {
        buzzing_tick_count ++;
        stop_buzzing_if_needed();
    } else {
        start_buzzing_if_needed();
    }
}

static void start_buzzing_if_needed(void)
{
    if (PEDOMETER_get_steps() >= PEDOMETER_get_goal()) {
	    if (buzzing_tick_count == 0) {
                buzzer_start();
	    }
    } else {
        buzzing_tick_count = 0;
    }
}

static void stop_buzzing_if_needed(void)
{
    if (buzzing_tick_count > BUZZER_DURATION_TICKS) {
        buzzer_stop();
    }
}

static void buzzer_start(void)
{
    HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
    buzzing_now = true;
}

static void buzzer_stop(void)
{
    HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
    buzzing_now = false;
}
