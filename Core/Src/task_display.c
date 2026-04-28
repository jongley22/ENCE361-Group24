/*
 * task_display.c
 *
 *  Created on: 10/03/2026
 *      Author: jon27
 */
#include "task_display.h"
#include "task_joystick.h"
#include "pedometer.h"
#include "main.h"
#include "pwm.h"

#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
#include "ssd1306.h"
#include "stm32c0xx_hal_conf.h"
#include "usart.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define POT_GOAL_MIN        500U
#define POT_GOAL_MAX        15000U
#define POT_GOAL_INCREMENT  100U
#define POT_ADC_MIN         200U
#define POT_ADC_MAX         3900U
#define BUZZER_DURATION_TICKS 10

static bool buzzer_started = false;

static uint32_t POT_get_position(void)
{
    uint32_t raw = get_potentiometer();
    if (raw < POT_ADC_MIN) raw = POT_ADC_MIN;
    if (raw > POT_ADC_MAX) raw = POT_ADC_MAX;
    uint32_t goal = POT_GOAL_MIN + (((raw - POT_ADC_MIN) * (POT_GOAL_MAX - POT_GOAL_MIN)) / (POT_ADC_MAX - POT_ADC_MIN));
    return (goal / POT_GOAL_INCREMENT) * POT_GOAL_INCREMENT;
}

static bool serialIsDebugging = false;
static bool testModeActive    = false;

static bool distanceUnitIsM = true;
static bool stepsUnitIsGoal = false;

static bool goalSetModeActive = false;
static uint32_t newGoal;


typedef enum {
    CURRENT_STEPS=0,
    DISTANCE_TRAVELLED,
    GOAL_PROGRESS
} DisplayScreen;

static DisplayScreen currentDisplayScreen = CURRENT_STEPS;


static void render_test_mode(void);


void display_joystick_long_press(void)
{
    if(goalSetModeActive) {
        Pedometer_SetGoal(newGoal);
        goalSetModeActive = false;
    } else {
        if(currentDisplayScreen == GOAL_PROGRESS) {
            goalSetModeActive = true;
        }
    }
}

void display_joystick_short_press(void)
{
    if(goalSetModeActive) {
        goalSetModeActive = false;
    }
}

void display_unit_switch(void)
{
    if(!testModeActive && !goalSetModeActive) {
        if(currentDisplayScreen == CURRENT_STEPS) {
            stepsUnitIsGoal = !stepsUnitIsGoal;
        } else if(currentDisplayScreen == DISTANCE_TRAVELLED) {
            distanceUnitIsM = !distanceUnitIsM;
        }
    }
}

void next_display_screen(bool isLeft)
{
    if(!testModeActive && !goalSetModeActive) {
        if(isLeft) {
            currentDisplayScreen++;
            if(currentDisplayScreen > GOAL_PROGRESS) {
                currentDisplayScreen = CURRENT_STEPS;
            }
        } else {
            if(currentDisplayScreen == CURRENT_STEPS) {
                currentDisplayScreen = GOAL_PROGRESS;
            } else {
                currentDisplayScreen--;
            }
        }
    }
}


static void render_test_mode(void)
{
    char steps_str[24];
    char goal_str[24];

    snprintf(steps_str, sizeof(steps_str), "Steps: %lu", Pedometer_GetSteps());
    snprintf(goal_str,  sizeof(goal_str),  "Goal:  %lu", Pedometer_GetGoal());

    ssd1306_Fill(Black);

    ssd1306_SetCursor(0, 10);
    ssd1306_WriteString("-- TEST MODE --", Font_7x10, White);

    ssd1306_SetCursor(0, 25);
    ssd1306_WriteString(steps_str, Font_7x10, White);

    ssd1306_SetCursor(0, 40);
    ssd1306_WriteString(goal_str, Font_7x10, White);

    ssd1306_UpdateScreen();
}

static void buzzer_start(void)
{
    //HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 1);
    //HAL_Delay(3000);
    //HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, 0);
    static int32_t counter;
    if(buzzer_started) {
        counter ++;
        if(counter > BUZZER_DURATION_TICKS) {
            HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
        }
    } else {
        HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
        counter = 0;
        buzzer_started = true;
    }
}

void display_execute(void)
{
    uint32_t steps = Pedometer_GetSteps();
    uint32_t goal  = Pedometer_GetGoal();
    if (steps >= goal) {
        buzzer_start();
    } else {
        buzzer_started = false;
    }
    if (testModeActive)
    {
        render_test_mode();
        return;
    }

    uint16_t lengths = 40;

    char line1[lengths];
    char line2[lengths];

    if(currentDisplayScreen == CURRENT_STEPS) {
        if(stepsUnitIsGoal) {
            uint32_t steps = Pedometer_GetSteps();
            uint32_t goal = Pedometer_GetGoal();
            uint32_t prog_perc = (steps * 100) / goal;
            snprintf(line1, sizeof(line1), "Steps: %lu%% of goal", prog_perc);
        } else {
            snprintf(line1, sizeof(line1), "Steps: %lu", Pedometer_GetSteps());
        }
        snprintf(line2, sizeof(line2), "");
    } else if(currentDisplayScreen == DISTANCE_TRAVELLED) {
        uint32_t dist = ((Pedometer_GetSteps() * 80) / 100);
        if(distanceUnitIsM) {
            dist /= 1000;
            snprintf(line1, sizeof(line1), "Dist: %lu km", dist);
        } else {
            dist = (dist * 10936) / 10000;
            snprintf(line1, sizeof(line1), "Dist: %lu yards", dist);
        }
        snprintf(line2, sizeof(line2), "");
    } else if(currentDisplayScreen == GOAL_PROGRESS) {
        if(goalSetModeActive) {
            uint32_t pos = POT_get_position();
            newGoal = pos;  // TODO: possible calculation if values don't line up
            snprintf(line1, sizeof(line1),  "-- GOAL SET --");
            snprintf(line1, sizeof(line2),  "New: %lu steps", newGoal);
        } else {
            uint32_t steps = Pedometer_GetSteps();
            uint32_t goal = Pedometer_GetGoal();
            uint32_t prog_perc = (steps * 100) / goal;
            snprintf(line1, sizeof(line1),  "Goal:  %lu", goal);
            snprintf(line2, sizeof(line2), "Prog: %lu%%", prog_perc);
        }
    }
    ssd1306_Fill(Black);

    ssd1306_SetCursor(0, 10);
    ssd1306_WriteString(line1, Font_7x10, White);

    ssd1306_SetCursor(0, 25);
    ssd1306_WriteString(line2, Font_7x10, White);

    ssd1306_UpdateScreen();


}


void display_init(void)
{
    ssd1306_Init();
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Hello world!", Font_7x10, White);
    ssd1306_UpdateScreen();
}


void toggle_serial_debug(void)
{
    serialIsDebugging = !serialIsDebugging;
}

void display_toggle_test_mode(void)
{
    if(!goalSetModeActive) {
        testModeActive = !testModeActive;
    }
}
bool display_is_test_mode(void)             /* M2.3 */
{
    return testModeActive;
}
