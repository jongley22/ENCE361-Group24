#include "state_goal_progress.h"

#include "pedometer.h"

#include <stdio.h>


void render_goal_progress(char* display, size_t max_chars_length)
{
    snprintf(
        display,
        max_chars_length,
        "-- Goal Progress Screen --\n"
        "Goal Prog: %lu %%",
        PEDOMETER_get_goal_percent()
    );
}
