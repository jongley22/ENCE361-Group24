/*
 * task_joystick.c
 *
 *  Created on: 10/03/2026
 *      Author: jon27
 */
#include "task_joystick.h"
#include <stdlib.h>

#include "adc.h"
#include "task_display.h"

#define JOYSTICK_X_MIN 470
#define JOYSTICK_Y_MIN 285
#define JOYSTICK_X_MAX 3940
#define JOYSTICK_Y_MAX 4075


/*
 * Array with joystick y, x
 */
static uint16_t raw_adc[2];

static int16_t calculate_joystick_percentage(
    uint16_t value,
    uint16_t min,
    uint16_t max
);

static void joystick_normal_mode_toggle_run(void);

void joystick_init(void)
{
}


void joystick_execute(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)raw_adc, 2);
    joystick_normal_mode_toggle_run();
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
}


static void joystick_normal_mode_toggle_run(void)
{
	static bool normal_joystick_clicked = true;
	int16_t x = get_joystick_x_percent();
	int16_t y = get_joystick_y_percent();
	if(x < 10 && x > -10 && y < 10 && y > -10) {
		normal_joystick_clicked = false;
	} else if(!normal_joystick_clicked) {
		if(x < -80) {
			// display left
			normal_joystick_clicked = true;
			next_display_screen(true);
		} else if(x > 80) {
			// display right
			normal_joystick_clicked = true;
			next_display_screen(false);
		}
		if(y < -80) {
			// up
			normal_joystick_clicked = true;
			display_unit_switch();
		} else if(y > 80) {
			// down
			normal_joystick_clicked = true;
		}
	}
}

uint16_t get_joystick_x(void)
{
	if (raw_adc[1] > JOYSTICK_X_MAX) {
		return JOYSTICK_X_MAX;
	} else if (raw_adc[1] < JOYSTICK_X_MIN) {
		return JOYSTICK_X_MIN;
	} else {
		return raw_adc[1];
	}
}

uint16_t get_joystick_y(void)
{
	if (raw_adc[0] > JOYSTICK_Y_MAX) {
		return JOYSTICK_Y_MAX;
	} else if (raw_adc[0] < JOYSTICK_Y_MIN) {
		return JOYSTICK_Y_MIN;
	} else {
		return raw_adc[0];
	}
}


int16_t get_joystick_x_percent(void)
{
    return calculate_joystick_percentage(
        get_joystick_x(),
        JOYSTICK_X_MIN,
        JOYSTICK_X_MAX
    );
}


int16_t get_joystick_y_percent(void)
{
    return calculate_joystick_percentage(
        get_joystick_y(),
        JOYSTICK_Y_MIN,
        JOYSTICK_Y_MAX
    );
}


static int16_t calculate_joystick_percentage(
    uint16_t value,
    uint16_t min,
    uint16_t max)
{
    int16_t diff = (int16_t)((max - min) / 2);
    int16_t percent = ((value - min - diff) * 100) / diff;
    if (abs(percent) < 10){
    	return 0;
    }
    return percent;
}
