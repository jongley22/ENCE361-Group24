#include "state_distance_travelled.h"

#include "pedometer.h"

#include <stdbool.h>
#include <stdio.h>


// floating point representation of distance in km
// or yards, without using actual floats!
typedef struct {
    uint32_t before_dot;
    uint32_t after_dot;
} Distance;

static bool unit_is_km = true;


static Distance calculate_current_distance(void);


/*
 * Toggle the units of distance between kilometers and
 * yards. Kilometers is displayed as a decimal and
 * yards are displayed to whole integers.
 */
void toggle_distance_units(void)
{
    unit_is_km = !unit_is_km;
}

/*
 * Render the distance travelled screen by
 * filling in the 'display' array.
 */
void render_distance_travelled(char* display, size_t max_chars_length)
{
    Distance dist = calculate_current_distance();
    if(unit_is_km) {
        snprintf(
            display,
            max_chars_length,
            "Distance\n"
            "%lu.%02lu km",
            dist.before_dot,
            dist.after_dot
        );
    } else {
        snprintf(
            display,
            max_chars_length,
            "Distance\n"
            "%lu yards",
            dist.before_dot
        );
    }
}

static Distance calculate_current_distance(void)
{
    uint32_t dist_meters = ((PEDOMETER_get_steps() * 80) / 100);

    Distance dist;

    if(unit_is_km) {
        dist.before_dot = dist_meters / 1000;
        dist.after_dot = (dist_meters / 10) % 100;
    } else {
        dist.before_dot = (dist_meters * 10936) / 10000;
    }

    return dist;
}
