/*
 * state_distance_travelled.h
 *
 * The FSM state for the distance travelled screen.
 */

#ifndef STATE_DISTANCE_TRAVELLED_H
#define STATE_DISTANCE_TRAVELLED_H


#include <stddef.h>


/*
 * Toggle the units of distance between kilometers and
 * yards. Kilometers is displayed as a decimal and
 * yards are displayed to whole integers.
 */
void toggle_distance_units(void);

/*
 * Render the distance travelled screen by
 * filling in the 'display' array.
 */
void render_distance_travelled(char* display, size_t max_chars_length);


#endif /* STATE_DISTANCE_TRAVELLED_H */
