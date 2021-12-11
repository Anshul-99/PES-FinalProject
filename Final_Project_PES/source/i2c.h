/*
 * i2c.h
 *
 *  Created on: 07-Dec-2021
 *      Author: anshul
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include "MKL25Z4.h"

#define I2C_M_START 	I2C1->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP  	I2C1->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART 	I2C1->C1 |= I2C_C1_RSTA_MASK

#define I2C_TRAN			I2C1->C1 |= I2C_C1_TX_MASK
#define I2C_REC				I2C1->C1 &= ~I2C_C1_TX_MASK

#define BUSY_ACK 	    while(I2C1->S & 0x01)
#define TRANS_COMP		while(!(I2C1->S & 0x80))
#define I2C_WAIT 			i2c_wait();

#define NACK 	        I2C1->C1 |= I2C_C1_TXAK_MASK
#define ACK           I2C1->C1 &= ~I2C_C1_TXAK_MASK

#define DEV_ADD_WRITE (0x78)
#define CMD (0x00)
#define DATA_ (0x40)

void init_i2c();

void send_dev_add();

int8_t write_data(uint8_t dev, uint8_t address, uint8_t data);

#endif /* I2C_H_ */
