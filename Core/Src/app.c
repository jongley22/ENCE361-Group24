/*
 * app.c
 *
 *  Created on: 24/02/2026
 *      Author: jon27
 */

#include "app.h"

#include "fsm_states.h"

#include "task_scheduler.h"
#include "task_button_polling.h"
#include "task_led_blinking.h"
#include "task_joystick_potentiometer.h"
#include "task_display.h"
#include "task_buzzer.h"
#include "state_test_mode.h"

#include <stm32c0xx_hal.h>
#include <stdint.h>


#define BUTTON_POLLING_FREQUENCY_HZ 50
#define LED_BLINKING_FREQUENCY_HZ 2
#define JOYSTICK_FREQUENCY_HZ 50
#define DISPLAY_FREQUENCY_HZ 4
#define BUZZER_FREQUENCY_HZ 2


// IMPORTANT:
//
// If this value is changed, you NEED to re-work through all
// math at end of state_test_mode.c
#define TEST_MODE_FREQUENCY_HZ 10


void app_main(void)
{
    FSM_STATES_to_initial_state();

    SCHEDULER_add_task(
        &BUTTON_POLLING_init,
        &BUTTON_POLLING_execute,
        BUTTON_POLLING_FREQUENCY_HZ
    );
    SCHEDULER_add_task(
        &LED_BLINKING_init,
        &LED_BLINKING_execute,
        LED_BLINKING_FREQUENCY_HZ
    );
    SCHEDULER_add_task(
        &JOYPOT_init,
        &JOYPOT_execute,
        JOYSTICK_FREQUENCY_HZ
    );
    SCHEDULER_add_task(
        &DISPLAY_init,
        &DISPLAY_execute,
        DISPLAY_FREQUENCY_HZ
    );
    SCHEDULER_add_task(
        &BUZZER_init,
        &BUZZER_execute,
        BUZZER_FREQUENCY_HZ
    );
    SCHEDULER_add_task(
        &TEST_MODE_called_at_frequency,
        &TEST_MODE_called_at_frequency,
        TEST_MODE_FREQUENCY_HZ
    );
    SCHEDULER_run_tasks();
}
