/*
 * hts221.c
 *
 *  Created on: Nov 4, 2023
 *      Author: laptop
 */

#include "hts221.h"

float tx0, tx1, ty0, ty1;
float hx0, hx1, hy0, hy1;

uint8_t slaveAddress = HTS221_I2C_ADDRESS;

void hts221_write_bytes(uint8_t reg_address, uint8_t *data,uint8_t len){
	masterWriteMultiByte(data,len, slaveAddress, reg_address);
}
void hts221_read_bytes(uint8_t reg_address, uint8_t *data,uint8_t len){
	//if(len > 1)
	//	reg_address |= (1<<7);
	masterReadMultiByte(data,len, slaveAddress, reg_address);
}

uint8_t hts221_init(){
	uint8_t ID;
	hts221_read_bytes(HTS221_REG_WHO_AM_I, &ID, 1);
	if(ID != HTS221_WHO_AM_I_VALUE){
		return 0;
	}

	uint8_t t1_t0_msb;
	hts221_read_bytes(HTS221_REG_T1_T0_MSB, &t1_t0_msb, 1);

	uint8_t data[2];
	hts221_read_bytes(HTS221_REG_H0_rH_x2, &data, 1);
	hy0 = (float)data[0]/2.0;
	hts221_read_bytes(HTS221_REG_H1_rH_x2, &data, 1);
	hy1 = (float)data[0]/2.0;

	hts221_read_bytes(HTS221_REG_H0_T0_OUT_L, &data, 2);
	hx0 = (float)(data[0] | data[1] << 8);
	hts221_read_bytes(HTS221_REG_H1_T0_OUT_L, &data, 2);
	hx1 = (float)(data[0] | data[1] << 8);

	hts221_read_bytes(HTS221_REG_T0_degC_x8, &data, 1);
	ty0 = (float)(data[0] | ((t1_t0_msb & 0b11) << 8))/8.0;
	hts221_read_bytes(HTS221_REG_T1_degC_x8, &data, 1);
	ty1 = (float)(data[0] | ((t1_t0_msb & 0b1100) << (8 - 2)))/8.0;

	hts221_read_bytes(HTS221_REG_T0_OUT_L, &data, 2);
	tx0 = (float)(data[0] | data[1] << 8);
	hts221_read_bytes(HTS221_REG_T1_OUT_L, &data, 2);
	tx1 = (float)(data[0] | data[1] << 8);

	uint8_t ctrl_reg1 = 0b10000011;
	hts221_write_bytes(HTS221_REG_CTRL_REG1, &ctrl_reg1,1);
	return 1;
}
float hts221_get_temperature(){
	uint8_t data[2];
	uint16_t x;
	hts221_read_bytes(HTS221_REG_TEMP_OUT_L, &data, 2);
	x = data[0] | data[1]<<8;
	return linear_interpolation((float)x, tx0, tx1, ty0, ty1);
}

float hts221_get_humidity(){
	uint8_t data[2];
	uint16_t x;
	hts221_read_bytes(HTS221_REG_HUMIDITY_OUT_L, &data, 2);
	x = data[0] | data[1]<<8;
	return linear_interpolation((float)x, hx0, hx1, hy0, hy1);
}
float linear_interpolation(float x,float x0,float x1,float y0,float y1){
	//vzorec na linearnu interpolaciu
	return (y0 + ((x-x0)*(y1-y0))/(x1-x0));
}



