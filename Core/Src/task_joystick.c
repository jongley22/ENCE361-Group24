#include  "task_joystick.h"

#include "adc.h"
#include <stdint.h>


static uint16_t raw_adc[2];


void joystick_execute(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)raw_adc, 2);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{

}
