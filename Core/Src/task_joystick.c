#include  "task_joystick.h"

#include "adc.h"


/*
 * Array with joystick y,x
 */
static uint16_t raw_adc[2];


void joystick_execute(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)raw_adc, 2);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
}

uint16_t get_joystick_x(void)
{
    return raw_adc[1];
}

uint16_t get_joystick_y(void)
{
    return raw_adc[0];
}
