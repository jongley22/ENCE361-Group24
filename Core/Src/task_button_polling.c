/*
 * task_button_polling.c
 *
 *  Created on: 10/03/2026
 *      Author: jon27
 */

#include "task_button_polling.h"

#include "task_display.h"
#include "buttons.h"
#include "rgb.h"
#include "tim.h"
#include "pwm.h"
#include "pedometer.h"
#include "task_joystick.h"

#include <stdlib.h>
#include "stm32c0xx_hal.h"

#include <stdbool.h>

#define CPU_TICK_FREQUENCY_HZ 1000
#define SW4_STEPS_INCREMENT 7U
#define PWM_MAX_DUTY_CYCLE 100
#define PWM_ADD_DUTY_CYCLE 10
#define DOUBLE_TAP_WINDOW_MS 400U    /* M2.3 — max ms between two taps */
#define JOYSTICK_MAX_STEPS_PER_CALL     40U /* Joystick displacment/step increment */
#define JOYSTICK_DEADZONE_PERCENT       10
#define JOYSTICK_STEP_THRESHOLD_PERCENT  50   /* below this, always 1 step per call */
#define TEST_MODE_JOYSTICK_PERIOD_TICKS     100U /* 10Hz = every 100ms */
#define JOYSTICK_LONG_HOLD_SEC 1
#define JOYSTICK_LONG_HOLD_TICKS (CPU_TICK_FREQUENCY_HZ * JOYSTICK_LONG_HOLD_SEC)

static uint32_t lastDownPressTime = 0;  /* M2.3 */
static void joystick_test_mode_update(void);
static void check_a_button(buttonName_t button_name, rgb_led_t rgb_name);
static void pwm_increase(void);

static uint32_t joystickLastClicked;
static bool joystick_clicked = false;

static uint32_t joystickNextRun = 0;
static bool joystickReturned = true;

void button_polling_init(void)
{
    buttons_init();
    rgb_colour_all_on();
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    joystickLastClicked = HAL_GetTick();
}


void button_polling_execute(void)
{
    buttons_update();
    check_a_button(DOWN, RGB_DOWN);
    check_a_button(UP, RGB_UP);
    check_a_button(LEFT, RGB_LEFT);
    check_a_button(RIGHT, RGB_RIGHT);
    if (display_is_test_mode()) {
        joystick_test_mode_update();
    }
    buttonState_t state = buttons_checkButton(JOYSTICK_CLICK);
    if(state == PUSHED) {
	    if(!joystick_clicked) {
		    joystick_clicked = true;
		    joystickLastClicked = HAL_GetTick();
	    }
    } else if(state == RELEASED) {
	    joystick_clicked = false;
	    if(HAL_GetTick() - joystickLastClicked > JOYSTICK_LONG_HOLD_TICKS) {
		    display_joystick_long_press();
	    } else {
		    display_joystick_short_press();
	    }
    }
}


static void pwm_increase(void)
{
    uint8_t dutyCycle = pwm_getDutyCycle(&htim2, TIM_CHANNEL_3);
    dutyCycle += PWM_ADD_DUTY_CYCLE;
    if (dutyCycle > PWM_MAX_DUTY_CYCLE) {
        dutyCycle = 0;
    }
    pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, dutyCycle);
}

static void joystick_test_mode_update(void)
{
    uint32_t now = HAL_GetTick();
    if (now < joystickNextRun) {
        return;
    }
    joystickNextRun = now + TEST_MODE_JOYSTICK_PERIOD_TICKS;

    int16_t percent = get_joystick_y_percent();

    /* joystick returned to rest — reset flag */
    if (abs(percent) <= JOYSTICK_DEADZONE_PERCENT) {
        joystickReturned = true;
        return;
    }

    uint32_t steps;
    if (abs(percent) < JOYSTICK_STEP_THRESHOLD_PERCENT)
    {
        if (!joystickReturned) {
            return;     /* must return to rest before next single step */
        }
        steps = 1;
        joystickReturned = false;
    }
    else
    {
        /* high displacement — continuous, no one shot restriction */
        steps = (uint32_t)(((abs(percent) - JOYSTICK_STEP_THRESHOLD_PERCENT) * JOYSTICK_MAX_STEPS_PER_CALL) / JOYSTICK_STEP_THRESHOLD_PERCENT);
        if (steps < 1) steps = 1;
    }

    if (percent < 0)    /* joystick up — increment */
    {
        uint32_t goal    = Pedometer_GetGoal();
        uint32_t current = Pedometer_GetSteps();
        uint32_t limit   = (goal >= 10) ? (goal - 10) : 0;

        if (current < limit) {
            if (current + steps > limit) {
                steps = limit - current;
            }
            Pedometer_AddSteps(steps);
        }
    }
    else                /* joystick down — decrement */
    {
        Pedometer_RemoveSteps(steps);
    }
}

static void check_a_button(buttonName_t button_name, rgb_led_t rgb_name)
{
    buttonState_t state = buttons_checkButton(button_name);

    if (button_name == UP)
    {
        if (state == PUSHED) {
            pwm_increase();
        }
    }
    else if (button_name == DOWN)           /*  double tap detection */
    {
        if (state == PUSHED)
        {
            uint32_t now = HAL_GetTick();
            if ((now - lastDownPressTime) <= DOUBLE_TAP_WINDOW_MS) {
                display_toggle_test_mode();
                lastDownPressTime = 0;
            } else {
                lastDownPressTime = now;
            }
            rgb_led_on(rgb_name);
        }
        else if (state == RELEASED) {
            rgb_led_off(rgb_name);
        }
    }
    else if (button_name == LEFT) {
        if (state == PUSHED) {
            rgb_led_on(rgb_name);
            uint32_t goal    = Pedometer_GetGoal();
            uint32_t current = Pedometer_GetSteps();
            uint32_t limit   = (goal >= 10) ? (goal - 10) : 0;
            if (current < limit) {
                Pedometer_AddSteps(SW4_STEPS_INCREMENT);
            }
        } else if (state == RELEASED) {
            rgb_led_off(rgb_name);
        }
    }
    else if (state == PUSHED) {             /* RIGHT */
        rgb_led_on(rgb_name);
    } else if (state == RELEASED) {
        rgb_led_off(rgb_name);
    }
}
