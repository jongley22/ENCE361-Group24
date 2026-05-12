/*
 * task_buzzer_goal_complete.c
 *
 * Checks if the current steps is greater than the current
 * goal. If it is, the buzzer starts buzzing and the display
 * FSM is changed to a state telling the user that the
 * goal is completed.
 */

#include "task_buzzer_goal_complete.h"

#include "display_fsm.h"
#include "fsm_states.h"
#include "pedometer.h"

#include "pwm.h"

#include <stdbool.h>
#include <stdio.h>


#define BUZZER_DURATION_TICKS 20


static bool buzzing_now = false;
static uint8_t buzzing_tick_count = 0;


static void buzzer_start(void);
static void buzzer_stop(void);
static void start_buzzing_if_needed(void);
static void stop_buzzing_if_needed(void);
static void to_goal_completed_screen(void);


/*
 * The task scheduler requires an init & execute function for every task.
 * If we need to initialise for button in future this can be used.w
 */
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

/*
 * Goal complete screen styled and made.
 */
void render_goal_completed(char* display, size_t max_chars_length)
{
    snprintf(
        display,
        max_chars_length,
        "Goal Complete!!!\n"
        "steps: %lu\n"
        "goal:  %lu",
        PEDOMETER_get_steps(),
        PEDOMETER_get_goal()
    );
}

/*
 * This also affects the screen shown if needed.
 */
static void start_buzzing_if_needed(void)
{
    if (PEDOMETER_get_steps() >= PEDOMETER_get_goal()) {
        if (buzzing_tick_count == 0) {
            buzzer_start();
            to_goal_completed_screen();
        }
    } else {
        buzzing_tick_count = 0;
    }
}

/*
 * Resets the screen.
 */
static void stop_buzzing_if_needed(void)
{
    if (buzzing_tick_count > BUZZER_DURATION_TICKS) {
        buzzer_stop();
        DISP_FSM_to_previous_state();
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

/*
 * Goal complete screen called, when step count exceeds goal.
 */
static void to_goal_completed_screen(void)
{
    DISP_FSM_State* state = DISP_FSM_to_new_state();
    state->get_display_chars = &render_goal_completed;
}
