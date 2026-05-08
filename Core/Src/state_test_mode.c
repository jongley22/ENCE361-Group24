#include "state_test_mode.h"

#include "pedometer.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


#define JOYSTICK_ONE_CHANGE_THRESHOLD 50


bool joystick_at_rest = true;

// steps increasing or decreasing (joystick up or down)
bool steps_increase = true;

// amount to increase steps
uint32_t step_change_per_call = 0;


static uint32_t calculate_step_change(uint32_t goal, uint16_t joystick_percent);
static void add_or_subtract_steps(uint32_t steps);


void TEST_MODE_called_at_frequency(void)
{
    if(!joystick_at_rest) {
        add_or_subtract_steps(step_change_per_call);
    }
}

void on_test_mode_exit(void)
{
    joystick_at_rest = true; // make sure step count isn't changing
}

void test_mode_joystick_state_change(
    JOYSTICK_State* old_state,
    JOYSTICK_State* new_state)
{
    static bool already_added_one = false;
    joystick_at_rest = new_state->y_at_rest;
    if (joystick_at_rest) {
        already_added_one = false;
    } else {
        steps_increase = new_state->is_up;
        if (new_state->percent_y > JOYSTICK_ONE_CHANGE_THRESHOLD) {
            step_change_per_call = calculate_step_change(
                PEDOMETER_get_goal(),
                new_state->percent_y
            );
        } else {
            if (!already_added_one) {
                // only add 1 when changing from at rest to not at rest.
                add_or_subtract_steps(1);
                already_added_one = true;
            }

            // force no later calls to update.
            joystick_at_rest = true;
        }
    }
}

void render_test_mode(char* display, size_t max_chars_length)
{
    snprintf(
        display,
        max_chars_length,
        "Test Mode\n"
        "Steps: %lu\n"
        "Goal: %lu",
        PEDOMETER_get_steps(),
        PEDOMETER_get_goal()
    );
}

static void add_or_subtract_steps(uint32_t steps)
{
    if(steps_increase) {
        uint32_t goal = PEDOMETER_get_goal();
        PEDOMETER_add_steps(steps);
        if(PEDOMETER_get_steps() > (goal-10)) {
            PEDOMETER_set_steps(goal-10);
        }
    } else {
        PEDOMETER_remove_steps(steps);
    }
}

static uint32_t calculate_step_change(uint32_t goal, uint16_t joystick_percent)
{
    // See working below for how this equation was derrived.
    //
    // I have purposely broken the magic numbers style rule
    // here. This is because the working below fully explains
    // these numbers, and I thought it would be easier to read
    // if the working was right below the only usage of
    // the equation.
    return goal / (550 - 5*joystick_percent);
}


/*
 * Working out calculate_step_change equation.
 *
 *
 *
 * Inputs (runtime):
 *     - joy = joystick_percent (0 to 100)
 *     - goal = current goal in steps
 *
 * Inputs (compile time):
 *     - freq = frequency calculate_step_change is called (Hz)
 *     - min_time = number of seconds from 0 to goal at joy=100%
 *     - max_time = number of seconds from 0 to
 *      goal at joy=10% (smallest joy value)
 *
 * Output:
 *     - steps_per_call = Number of steps to add each
 *                        call of 'TEST_MODE_called_at_frequency'
 *
 *
 * Let 'time' be the number of seconds from 0 to
 * goal at different values of 'joy'.
 *
 * at joy=100, time=min_time
 * at joy=10, time=max_time
 *
 * Next, fit time vs joy to a linear line.
 *
 * time = m*joy + c
 * equation one: min_time = m*100 + c
 * equation two: max_time = m*10 + c
 *
 * Next, solve for m and c.
 *
 * equation one: c = min_time - m*100
 * equation two: max_time = m*10 + c
 * combined: max_time = m*10 + min_time - m*100
 * combined: max_time = min_time - m*90
 * combined: max_time + m*90 = min_time
 * combined: m*90 = min_time - max_time
 * combined: m = (min_time - max_time) / 90
 * equation one: c = min_time - m*100
 * equation one: c = min_time - ((min_time - max_time) / 90)*100
 *
 * final equation: time = m*joy + c
 * final equation: time = ((min_time - max_time) / 90)*joy + min_time
 *                        - ((min_time - max_time) / 90)*100
 *
 *
 * Next, find how many steps to add per second. Let this be 'steps_per_s'.
 *
 * steps_per_s = goal / time
 *
 * steps_per_call = steps_per_s / freq
 *
 *
 * Next, calculate useful values setting compile-time constants.
 *
 * Compile time constants setting one:
 *     - freq = 10 Hz
 *     - min_time = 7 seconds
 *     - max_time = 70 seconds
 *
 *     - time = ((min_time - max_time) / 90)*joy + min_time - ((min_time - max_time) / 90)*100
 *     - time = ((7 - 70) / 90)*joy + 7 - ((7 - 70) / 90)*100
 *     - time = ((-63) / 90)*joy + 7 - ((-63) / 90)*100
 *     - time = ((-63) / 90)*joy + 7 - (-70)
 *     - time = ((-63) / 90)*joy + 77
 *     - time = (-63/90)*joy + 77
 *     - time = 77 - (63/90)*joy
 *     - time = 77 - (63*joy)/90
 *
 *     - steps_per_s = goal / time
 *     - steps_per_s = goal / (77 - (63*joy)/90)
 *
 *     - steps_per_call = steps_per_s / freq
 *     - steps_per_call = steps_per_s / 10
 *     - steps_per_call = (goal / (77 - (63*joy)/90)) / 10
 *     - steps_per_call = goal / (10*(77 - (63*joy)/90))
 *     - steps_per_call = goal / (10*77 - (10*63*joy)/90)
 *     - steps_per_call = goal / (770 - (10*63*joy)/90)
 *     - steps_per_call = goal / (770 - 7*joy)
 *
 * Example Usage one:
 *     - goal = 1024 steps
 *     - joy  = 100%
 *     - steps_per_call = goal / (770 - 7*joy)
 *     - steps_per_call = 15 (approx)
 * 
 * Example Usage two:
 *     - goal = 1024 steps
 *     - joy  = 10%
 *     - steps_per_call = goal / (770 - 7*joy)
 *     - steps_per_call = 1.5 (approx)
 *
 *
 * Compile time constants setting two:
 *     - freq = 10 Hz
 *     - min_time = 5 seconds
 *     - max_time = 50 seconds
 *
 *     - time = ((min_time - max_time) / 90)*joy + min_time - ((min_time - max_time) / 90)*100
 *     - time = ((-45) / 90)*joy + 5 - ((-45) / 90)*100
 *     - time = (-1/2)*joy + 5 + 50
 *     - time = (-1/2)*joy + 55
 *
 *     - steps_per_s = goal / time
 *     - steps_per_s = goal / ((-1/2)*joy + 55)
 *     - steps_per_s = goal / ((-1/2)*joy + 55)
 *
 *     - steps_per_call = steps_per_s / freq
 *     - steps_per_call = steps_per_s / 10
 *     - steps_per_call = (goal / ((-1/2)*joy + 55)) / 10
 *     - steps_per_call = goal / (10*(-1/2)*joy + 10*55)
 *     - steps_per_call = goal / ((-5)*joy + 550)
 *     - steps_per_call = goal / (550 - 5*joy)
 */
