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
​ * ​ ​ @file​ ​ i2c.c
​ * ​ ​ @brief​ ​ This file implements all the functions necessary to use I2C protocol.
​ *
​ * ​ ​ @author​ ​ Anshul Somani
​ * ​ ​ @date​ ​ December 7 2021
​ * ​ ​ @version​ ​ 1.0
​ *
​ */

#include "i2c.h"
#include "MKL25Z4.h"
#include "sysclock.h"

int lock_detect=0;
int i2c_lock=0;

void init_i2c()
{
	SIM->SCGC4 |= SIM_SCGC4_I2C1(1); // Enable clock to I2C1
	SIM->SCGC5 |= SIM_SCGC5_PORTE(1); // Enable clock to PORTE

	PORTE->PCR[0] |= PORT_PCR_MUX(6) /*| PORT_PCR_PS(1)| PORT_PCR_PE(1) */; // Enable SDA on pin PTE0
	PORTE->PCR[1] |= PORT_PCR_MUX(6) /*| PORT_PCR_PS(1)| PORT_PCR_PE(1)*/; // Enable SCL on pin PTE1

	I2C1->F = (I2C_F_ICR(0x12) | I2C_F_MULT(0)); //ICR can be 0x12 as well

	I2C1->C1 |= I2C_C1_IICEN(1); // Enable I2C peripheral
}


void i2c_busy(void){

	lock_detect=0;
	I2C1->C1 &= ~I2C_C1_IICEN_MASK; /* Disable interrupts */
	I2C_TRAN;
	I2C_M_START; // Start Signal
	I2C1->C1 |=  I2C_C1_IICEN_MASK; /* Enable interrupts */
	// Write to clear line
	I2C1->C1 |= I2C_C1_MST_MASK; /* set MASTER mode */
	I2C1->C1 |= I2C_C1_TX_MASK; /* Set transmit (TX) mode */
	I2C1->D = 0xFF;
	while ((I2C1->S & I2C_S_IICIF_MASK) == 0U) {
	} /* wait for interrupt bit to be set */
	I2C1->S |= I2C_S_IICIF_MASK; /* clear interrupt bit */

	/* Clear arbitration error flag*/
	I2C1->S |= I2C_S_ARBL_MASK;

	/* Send start */
	I2C1->C1 &= ~I2C_C1_IICEN_MASK; /* Disable interrupts */
	I2C1->C1 |= I2C_C1_TX_MASK; /* Set transmit (TX) mode */
	I2C1->C1 |= I2C_C1_MST_MASK; /* START signal generated */

	I2C1->C1 |= I2C_C1_IICEN_MASK;

	I2C1->C1 &= ~I2C_C1_IICEN_MASK;
	I2C1->C1 |= I2C_C1_MST_MASK;
	I2C1->C1 &= ~I2C_C1_MST_MASK; /* set SLAVE mode */
	I2C1->C1 &= ~I2C_C1_TX_MASK; /* Set Rx */
	I2C1->C1 |= I2C_C1_IICEN_MASK;

	/* Clear arbitration error & interrupt flag*/
	I2C1->S |= I2C_S_IICIF_MASK;
	I2C1->S |= I2C_S_ARBL_MASK;
	lock_detect=0;
	i2c_lock=1;
}

void i2c_wait(void) {
	lock_detect = 0;
	while(((I2C1->S & I2C_S_IICIF_MASK)==0) & (lock_detect < 200)) {
		lock_detect++;
	}
	if (lock_detect >= 200)
		i2c_busy();
	I2C1->S |= I2C_S_IICIF_MASK;
}

void write_data(uint8_t dev, uint8_t address, uint8_t data)
{
	I2C_TRAN;
	I2C_M_START;
	I2C_M_START;
	I2C1->D = dev; // send device address
	I2C_WAIT

	I2C1->D = address; // send information regarding the next byte. Whether it is data or command
	I2C_WAIT

	I2C1->D = data; // Data or command
	I2C_WAIT

	I2C_M_STOP;
}
