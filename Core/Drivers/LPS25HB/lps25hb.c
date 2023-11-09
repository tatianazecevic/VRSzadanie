/*
 * lp25hb.c
 *
 *  Created on: Nov 4, 2023
 *      Author: laptop
 */
#include "lps25hb.h"

static uint8_t slaveAddress = LPS25HB_I2C_ADDRESS_0;
void lps25hb_write_bytes(uint8_t *data, uint8_t reg_address, uint8_t len){
    masterWriteMultiByte(data, len, slaveAddress, reg_address);
}
void lps25hb_read_bytes(uint8_t *data, uint8_t reg_address, uint8_t len){
	if(len > 1)
		reg_address |= (1<<7);
    masterReadMultiByte(data, len, slaveAddress, reg_address);
}

uint8_t lps25hb_init(){
    uint8_t ID;
    lps25hb_read_bytes(&ID,LPS25HB_REG_WHO_AM_I_ADDR,1);
    if(ID != LPS25HB_WHO_AM_I_VALUE){
        slaveAddress = LPS25HB_I2C_ADDRESS_1;
        lps25hb_read_bytes(&ID,LPS25HB_REG_WHO_AM_I_ADDR,1);
        if(ID != LPS25HB_WHO_AM_I_VALUE)
            return 0;
    }
    uint8_t ctrl1 = 0b11000000; //dokumwntacia
    lps25hb_write_bytes(&ctrl1,LPS25HB_REG_CTRL_REG1,1);
    return 1;
}   

float lps25hb_get_pressure(){
    uint8_t data[3];
    lps25hb_read_bytes(&data,LPS25HB_REG_PRESS_OUT_XL,3);
    return ((data[0] | data[1] << 8 | data[2] << 16)/4096.0); 
} 
float lps25hb_calculate_altitude(float pressure){
    return  44330 * (1 - pow(pressure/1013.25, 1/5.255));
}


