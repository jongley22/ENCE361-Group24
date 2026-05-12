/*
 * state_goal_set.c
 *
 * The FSM state for the goal set screen.
 */

#include "state_goal_set.h"

#include "pedometer.h"

#include <stdbool.h>
#include <stdio.h>


#define POT_GOAL_MIN        500U
#define POT_GOAL_MAX        15000U
#define POT_GOAL_INCREMENT  100U
#define POT_ADC_MIN         200U
#define POT_ADC_MAX         3900U


static uint32_t new_goal = POT_GOAL_MIN;


static uint32_t calc_goal_from_pot(uint16_t current_pot);


/*
 * Set the actual goal in the 'pedometer' module to the 'new' goal.
 */
void save_new_goal(void)
{
    PEDOMETER_set_goal(new_goal);
}

/*
 * Set the 'new' goal from the current potentiometer
 * value. The 'new' goal is not set until
 * the 'save_new_goal' function is called.
 */
void set_goal_from_pot(uint16_t* old_pot, uint16_t* current_pot)
{
    new_goal = calc_goal_from_pot(*current_pot);
}

/*
 * Draw the goal set screen to the 'display' array.
 */
void render_goal_set(char* display, size_t max_chars_length)
{
    snprintf(
        display,
        max_chars_length,
        "Goal Set\n\n"
        "Current: %lu\n"
        "New: %lu",
        PEDOMETER_get_goal(),
        new_goal
    );
}

static uint32_t calc_goal_from_pot(uint16_t current_pot)
{
    if (current_pot < POT_ADC_MIN) {
        current_pot = POT_ADC_MIN;
    }
    if (current_pot > POT_ADC_MAX) {
        current_pot = POT_ADC_MAX;
    }

    uint32_t goal = (POT_GOAL_MIN + (((current_pot - POT_ADC_MIN) * (POT_GOAL_MAX - POT_GOAL_MIN))
                     / (POT_ADC_MAX - POT_ADC_MIN)));

    // round goal to nearest increment before returning
    return (goal / POT_GOAL_INCREMENT) * POT_GOAL_INCREMENT;
}
