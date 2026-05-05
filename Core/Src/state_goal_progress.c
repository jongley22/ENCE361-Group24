#include "state_goal_progress.h"

#include "pedometer.h"

#include <stdio.h>


void render_goal_progress(char* display, size_t max_chars_length)
{
    snprintf(
        display,
        max_chars_length,
        "Goal Progress\n"
        "Goal: %lu steps\n"
        "Prog: %lu %%",
        PEDOMETER_get_goal(),
        PEDOMETER_get_goal_percent()
    );
}
