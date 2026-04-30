#include "state_current_steps.h"

#include "pedometer.h"

#include <stdbool.h>
#include <stdio.h>


static bool unit_is_percent = false;


void toggle_step_units(void)
{
    unit_is_percent = !unit_is_percent;
}

void render_current_steps(char* display, size_t max_chars_length)
{
    if(unit_is_percent) {
        snprintf(
            display,
            max_chars_length,
            "Current Steps\n"
            "steps: %lu %% of goal",
            PEDOMETER_get_goal_percent()
        );
    } else {
        snprintf(
            display,
            max_chars_length,
            "Current Steps\n"
            "steps: %lu",
            PEDOMETER_get_steps()
        );
    }
}
