/*
 * task_button_polling.c
 *
 * Handles polling the user buttons. When a button is
 * clicked, the 'display_fsm' state machine is notified
 * so it can pass the event to the current state.
 */

#include "task_button_polling.h"

#include "display_fsm.h"
#include "pedometer.h"

#include "buttons.h"
#include "tim.h"


#include <stm32c0xx_hal.h>
#include <stdlib.h>
#include <stdbool.h>


#define SW4_STEPS_INCREMENT 7U
#define PWM_MAX_DUTY_CYCLE 100
#define PWM_ADD_DUTY_CYCLE 10

#define JOYSTICK_LONG_HOLD_SEC 1
#define CPU_TICK_FREQUENCY_HZ 1000
#define DOUBLE_TAP_WINDOW_TICKS 1000U
#define JOYSTICK_LONG_HOLD_TICKS (CPU_TICK_FREQUENCY_HZ * JOYSTICK_LONG_HOLD_SEC)


static void down_double_tap_logic(void);
static void check_a_button(buttonName_t button_name);
static void manage_joystick_push_logic(void);


void BUTTON_POLLING_init(void)
{
    buttons_init();
}

/*
 * Poll all of the buttons on the system. For
 * all clicked buttons, call the functions
 * in 'display_fsm.c'.
 */
void BUTTON_POLLING_execute(void)
{
    buttons_update();
    check_a_button(DOWN);
    check_a_button(LEFT);
    check_a_button(RIGHT);

    manage_joystick_push_logic();
}

/*
 * Board buttons, going to test mode and incrementing 7 steps.
 */
static void check_a_button(buttonName_t button_name)
{
    buttonState_t state = buttons_checkButton(button_name);


    if (button_name == DOWN) {
        if (state == PUSHED)
        {
            down_double_tap_logic();
        }
    } else if (button_name == LEFT) {
        if (state == PUSHED) {
            PEDOMETER_add_steps(SW4_STEPS_INCREMENT);
        }
    }
}

/*
 * Enter test mode
 */
static void down_double_tap_logic(void)
{
    static uint32_t down_last_press_time = 0;

    uint32_t now = HAL_GetTick();
    if ((now - down_last_press_time) <= DOUBLE_TAP_WINDOW_TICKS) {
        DISP_FSM_trig_button_test_tap();
        down_last_press_time = 0;
    } else {
        down_last_press_time = now;
    }
}

/*
 * Entering goal change state and exiting with/without saving goal change
 */
static void manage_joystick_push_logic(void)
{
    static bool joystick_clicked = false;
    static uint32_t joystick_last_press_time = 0;

    buttonState_t joystick = buttons_checkButton(JOYSTICK_CLICK);

    if (joystick == PUSHED) {
        if (!joystick_clicked) {
            joystick_clicked = true;
            joystick_last_press_time = HAL_GetTick();
        }
    } else if (joystick == RELEASED) {
        joystick_clicked = false;
        if (HAL_GetTick() - joystick_last_press_time > JOYSTICK_LONG_HOLD_TICKS) {
            DISP_FSM_trig_joystick_long_press();
        } else {
            DISP_FSM_trig_joystick_short_press();
        }
    }
}
