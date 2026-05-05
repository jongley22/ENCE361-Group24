/*
 * imu_lsm6ds_spi.c
 *
 *  Created on: Nov 28, 2024
 *      Author: fsy13
 */

#include "imu_lsm6ds.h"

#include "pedometer.h"

#include "spi.h"

// Hardware configuration
#define spi_hal_handler hspi2

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin & IMU_INT1_Pin)
    {
        PEDOMETER_add_steps(1);
    }
}

void imu_lsm6ds_write_byte(imu_register_t register_address, uint8_t value)
{
	uint8_t write_buff[2] = {0};
	write_buff[0] = value;
	write_buff[1] = register_address;

	// Send one word = 16 bits, MSB first
	HAL_SPI_Transmit(&spi_hal_handler, write_buff, 1, HAL_MAX_DELAY);
}

uint8_t imu_lsm6ds_read_byte(imu_register_t register_address)
{
	// 16 bit transmission:
	// First byte is the register address on MOSI, with the read bit enabled.
	// Second byte is the data from slave on MISO.
	// Indexing in reverse order due to MSB first.

	uint8_t tx[2] = {0};
	uint8_t rx[2] = {0};

	tx[1] = register_address |= (1 << 7); // Set "Read" bit

	HAL_SPI_TransmitReceive(&spi_hal_handler, tx, rx, 1, HAL_MAX_DELAY);

	return rx[0];
}

void imu_init(void)
{
    imu_lsm6ds_write_byte(CTRL1_XL, CTRL1_XL_HIGH_PERFORMANCE);
    imu_lsm6ds_write_byte(INT1_CTRL, INT1_STEP_DETECTOR_EN);
    imu_lsm6ds_write_byte(CTRL10_C, CTRL10_C_FUNC_EN);
    imu_lsm6ds_write_byte(CTRL10_C, CTRL10_C_PEDO_EN);
}
