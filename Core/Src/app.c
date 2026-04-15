/*
 * app.c
 *
 *  Created on: 24/02/2026
 *      Author: jon27
 */
#include "app.h"

#include "task_scheduler.h"

#include "task_button_polling.h"
#include "task_led_blinking.h"
#include "task_joystick.h"
#include "task_display.h"

#define BUTTON_POLLING_FREQUENCY_HZ 50
#define LED_BLINKING_FREQUENCY_HZ 2
#define JOYSTICK_FREQUENCY_HZ 50
#define DISPLAY_FREQUENCY_HZ 4


static void add_all_tasks(void);


void app_main(void)
{
    scheduler_add_task(
        &button_polling_init,
        &button_polling_execute,
        BUTTON_POLLING_FREQUENCY_HZ
    );
    scheduler_add_task(
        &led_blinking_init,
        &led_blinking_execute,
        LED_BLINKING_FREQUENCY_HZ
    );
    scheduler_add_task(
        &joystick_init,
        &joystick_execute,
        JOYSTICK_FREQUENCY_HZ
    );
    scheduler_add_task(
        &display_init,
        &display_execute,
        DISPLAY_FREQUENCY_HZ
    );
    scheduler_run_tasks();
}
