/*
 * imu_lsm6ds.h
 *
 *  Created on: Nov 27, 2024
 *      Author: fsy13
 */

#ifndef INC_IMU_LSM6DS_H_
#define INC_IMU_LSM6DS_H_

#include <stdint.h>

typedef enum {
	CTRL1_XL = 0x10,
	OUTX_L_XL = 0x28,
	OUTX_H_XL = 0x29,
	OUTY_L_XL = 0x2A,
	OUTY_H_XL = 0x2B,
	OUTZ_L_XL = 0x2C,
	OUTZ_H_XL = 0x2D,
	INT1_CTRL = 0x0D,
	CTRL10_C = 0x19,
	STEP_COUNTER_L = 0x4B,
	STEP_COUNTER_H = 0x4C,
} imu_register_t;

// Standard options
#define CTRL1_XL_HIGH_PERFORMANCE 0xA0U
#define INT1_STEP_DETECTOR_EN  0x80
#define CTRL10_C_PEDO_EN  0x10  // bit 4
#define CTRL10_C_FUNC_EN  0x04  // bit 2

void imu_lsm6ds_write_byte(imu_register_t register_address, uint8_t value);

uint8_t imu_lsm6ds_read_byte(imu_register_t register_address);

void imu_init(void);

#endif /* INC_IMU_LSM6DS_H_ */
