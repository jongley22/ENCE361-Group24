/*
 * task_joystick_potentiometer.c
 *
 * This module handles reading the analog joystick and potentiometer
 * values from the ADC. Having both of these values in the same
 * module means that there is only one module reading the ADC.
 */

#include "task_joystick_potentiometer.h"

#include "display_fsm.h"

#include <adc.h>
#include <stdlib.h>


#define JOYSTICK_X_MIN 470
#define JOYSTICK_Y_MIN 285
#define JOYSTICK_X_MAX 3940
#define JOYSTICK_Y_MAX 4075
#define JOYSTICK_AT_MAX 60

#define PERCENT_TREAT_AT_REST 10


JOYSTICK_State current_state;
uint16_t current_pot;

/*
 * Array with (potentiometer, joystick y, joystick x)
 */
static uint16_t raw_adc[3];


static void fill_in_state(JOYSTICK_State* state);
static bool states_are_equal(JOYSTICK_State* state1, JOYSTICK_State* state2);
static void call_needed_fsm_funcs(void);
static uint16_t get_potentiometer(void);

static int16_t calculate_joystick_percentage(
    uint16_t value,
    uint16_t min,
    uint16_t max
);


void JOYPOT_init(void)
{
    current_state.percent_x = 0;
    current_state.percent_y = 0;
    current_state.is_left = false;
    current_state.is_up = false;
    current_state.x_at_rest = true;
    current_state.y_at_rest = true;
    current_state.at_rest = true;
    current_state.at_max = false;
    current_pot = 0;
}

/*
 * Read the joystick and potentiometer values. Then
 * decide what functions need to be called
 * in 'display_fsm'.
 */
void JOYPOT_execute(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)raw_adc, 3);
    call_needed_fsm_funcs();
}

/*
 * Figure out if the joystick or potentiometer values
 * have changed. If they have, notify 'display_fsm'
 */
static void call_needed_fsm_funcs(void)
{
    JOYSTICK_State new_state = {0};

    // get the current values
    fill_in_state(&new_state);

    if (!states_are_equal(&new_state, &current_state)) {
        DISP_FSM_trig_joystick_change_state(&current_state, &new_state);
        current_state = new_state;
    }

    uint16_t new_pot = get_potentiometer();
    if (current_pot != new_pot) {
        DISP_FSM_trig_potentiometer_change(&current_pot, &new_pot);
        current_pot = new_pot;
    }
}

/*
 * Callback for when DMA is complete. This is
 * not used, but must be present.
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
}

static uint16_t get_potentiometer(void)
{
    return raw_adc[0];
}

static uint16_t get_joystick_x(void)
{
    if (raw_adc[2] > JOYSTICK_X_MAX) {
        return JOYSTICK_X_MAX;
    }
    return raw_adc[2];
}

static uint16_t get_joystick_y(void)
{
    if (raw_adc[1] > JOYSTICK_Y_MAX) {
        return JOYSTICK_Y_MAX;
    }
    return raw_adc[1];
}

static int16_t get_joystick_x_percent(void)
{
    return calculate_joystick_percentage(
        get_joystick_x(),
        JOYSTICK_X_MIN,
        JOYSTICK_X_MAX
    );
}

static int16_t get_joystick_y_percent(void)
{
    return calculate_joystick_percentage(
        get_joystick_y(),
        JOYSTICK_Y_MIN,
        JOYSTICK_Y_MAX
    );
}

/*
 * Given a joystick value, calculate the percentage
 * displacement from the center.
 *
 * For example, moving the joystick half way to the
 * right would be 50%. The percentage is returned
 * as a signed integer to the nearest whole
 * percent (e.g. returning 50).
 */
static int16_t calculate_joystick_percentage(
    uint16_t value,
    uint16_t min,
    uint16_t max)
{
    int16_t diff = (int16_t)((max - min) / 2);
    int16_t percent = ((value - min - diff) * 100) / diff;

    return percent;
}

/*
 * Fill in the passed in joystick structure with
 * the current joystick values.
 */
static void fill_in_state(JOYSTICK_State* state)
{
    int32_t x = get_joystick_x_percent();
    int32_t y = get_joystick_y_percent();

    state->percent_x = abs(x);
    state->percent_y = abs(y);

    state->x_at_rest = (state->percent_x < PERCENT_TREAT_AT_REST);
    state->y_at_rest = (state->percent_y < PERCENT_TREAT_AT_REST);

    state->x_at_max = (state->percent_x > JOYSTICK_AT_MAX);
    state->y_at_max = (state->percent_y > JOYSTICK_AT_MAX);

    state->at_max = (state->x_at_max || state->y_at_max);
    state->at_rest = (state->x_at_rest && state->y_at_rest);

    state->is_left = false;
    state->is_up = false;

    if (x > 0) {
        state->is_left = true;
    }
    if (y < 0) {
        state->is_up = true;
    }
}

/*
 * Check if two joystick states have the same values.
 *
 * This is used to find if the joystick has been moved.
 */
static bool states_are_equal(JOYSTICK_State* state1, JOYSTICK_State* state2)
{
    return (state1->percent_x == state2->percent_x
            && state1->percent_y == state2->percent_y
            && state1->is_up == state2->is_up
            && state1->is_left == state2->is_left);
}
