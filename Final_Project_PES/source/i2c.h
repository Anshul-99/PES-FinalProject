/*****************************************************************************
​ * ​ ​ Copyright​ ​ (C)​ ​ 2021​ ​ by​ ​ Anshul Somani
​ *
​ * ​ ​ Redistribution,​ ​ modification​ ​ or​ ​ use​ ​ of​ ​ this​ ​ software​ ​ in​ ​ source​ ​ or​ ​ binary
​ * ​ ​ forms​ ​ is​ ​ permitted​ ​ as​ ​ long​ ​ as​ ​ the​ ​ files​ ​ maintain​ ​ this​ ​ copyright.​ ​ Users​ ​ are
​ * ​ ​ permitted​ ​ to​ ​ modify​ ​ this​ ​ and​ ​ use​ ​ it​ ​ to​ ​ learn​ ​ about​ ​ the​ ​ field​ ​ of​ ​ embedded
​ * ​ ​ software.​ ​ Anshul Somani ​ and​ ​ the​ ​ University​ ​ of​ ​ Colorado​ ​ are​ ​ not​ ​ liable​ ​ for
​ * ​ ​ any​ ​ misuse​ ​ of​ ​ this​ ​ material.
​ *
*****************************************************************************/
/**
​ * ​ ​ @file​ ​ i2c.h
​ * ​ ​ @brief​ ​ This file implements all the functions necessary to use I2C protocol.
​ *
​ * ​ ​ @author​ ​ Anshul Somani
​ * ​ ​ @date​ ​ December 7 2021
​ * ​ ​ @version​ ​ 1.0
​ *
​ */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include "MKL25Z4.h"

/* Enabling Master bit generates the start condition on I2C bus */
#define I2C_M_START 	I2C1->C1 |= I2C_C1_MST_MASK
/* Disabling Master bit generates the stop condition on I2C bus */
#define I2C_M_STOP  	I2C1->C1 &= ~I2C_C1_MST_MASK
/* Restart condition */
#define I2C_M_RSTART 	I2C1->C1 |= I2C_C1_RSTA_MASK

/* Enable transmit mode */
#define I2C_TRAN			I2C1->C1 |= I2C_C1_TX_MASK
/* Disable transmit mode */
#define I2C_REC				I2C1->C1 &= ~I2C_C1_TX_MASK

/* Check busy flag */
#define BUSY_ACK 	    while(I2C1->S & 0x01)
/* Check transfer complete flag */
#define TRANS_COMP		while(!(I2C1->S & 0x80))
#define I2C_WAIT 			i2c_wait();

#define NACK 	        I2C1->C1 |= I2C_C1_TXAK_MASK
#define ACK           I2C1->C1 &= ~I2C_C1_TXAK_MASK

#define DEV_ADD_WRITE (0x78) /* device address of OLED for writing*/
#define CMD (0x00) /* byte that needs to be sent after device address to then send a command byte */
#define DATA_ (0x40) /* byte that needs to be sent after device address to then send a data byte */

/*
 * Initializes I2C module
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void init_i2c();

/*
 * Writes data or command on the I2C bus.
 * It first sends the device address. Then it sends a control byte,
 * that indicates whether the next byte is data or command.
 * It ends by sending the data or command byte.
 *
 * Parameters:
 *   uint8_t dev: device address with R/W bit reset/
 *   uint8_t address: Control byte
 *   uint8_t data: command or data byte
 *
 * Returns:
 *   void
 */
void write_data(uint8_t dev, uint8_t address, uint8_t data);

#endif /* I2C_H_ */
