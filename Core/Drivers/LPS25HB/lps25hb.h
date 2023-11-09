/*
 * lp25hb.h
 *
 *  Created on: Nov 4, 2023
 *      Author: laptop
 */

#ifndef DRIVERS_LP25HB_LP25HB_H_
#define DRIVERS_LP25HB_LP25HB_H_

#include "stdint.h"
#include "i2c.h"
#include <math.h>

#define LPS25HB_I2C_ADDRESS_0       0xBA
#define LPS25HB_I2C_ADDRESS_1       0xB8

#define LPS25HB_REG_WHO_AM_I_ADDR   0x0F
#define LPS25HB_WHO_AM_I_VALUE      0b10111101

#define LPS25HB_REG_CTRL_REG1       0x20
#define LPS25HB_REG_PRESS_OUT_XL    0x28


void lps25hb_write_bytes(uint8_t *data, uint8_t reg_address, uint8_t len);
void lps25hb_read_bytes(uint8_t *data, uint8_t reg_address, uint8_t len);

uint8_t lps25hb_init();

float lps25hb_get_pressure();
float lps25hb_calculate_altitude(float pressure);


#endif /* DRIVERS_LP25HB_LP25HB_H_ */
