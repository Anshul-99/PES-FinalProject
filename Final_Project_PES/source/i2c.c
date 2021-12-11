/*
 * i2c.c
 *
 *  Created on: 07-Dec-2021
 *      Author: anshul
 */

#include "i2c.h"
#include "MKL25Z4.h"
#include "sysclock.h"


/*
 *
 * address = 0x78 = 0b0111 1000
 * after adding write bit at lsb,
 * address = 0xF0 = 0b1111 0000
 */

int lock_detect=0;
int i2c_lock=0;

void init_i2c()
{
	SIM->SCGC4 |= SIM_SCGC4_I2C1(1); // Enable clock to I2C1
	SIM->SCGC5 |= SIM_SCGC5_PORTE(1); // Enable clock to PORTE

	PORTE->PCR[0] |= PORT_PCR_MUX(6); // Enable SDA on pin PTE0
	PORTE->PCR[1] |= PORT_PCR_MUX(6); // Enable SCL on pin PTE1

	I2C1->F = (I2C_F_ICR(0x12) | I2C_F_MULT(0)); //ICR can be 0x12 as well

	I2C1->C1 |= I2C_C1_IICEN(1); // Enable I2C peripheral

	// Select high drive mode
	I2C1->C2 |= (I2C_C2_HDRS_MASK);
}

void start_signal()
{
	I2C1->C1 |= I2C_C1_TX(1)| // Transmit mode
				I2C_C1_MST(1); // Master mode
}


void stop_signal()
{
	I2C1->C1 &= ~(I2C_C1_MST(1)|I2C_C1_TX(1)); // slave mode and receive mode.
}

/* From Dean's code */
void i2c_busy(void){
	// Start Signal
	lock_detect=0;
	I2C1->C1 &= ~I2C_C1_IICEN_MASK;
	I2C_TRAN;
	I2C_M_START;
	I2C1->C1 |=  I2C_C1_IICEN_MASK;
	// Write to clear line
	I2C1->C1 |= I2C_C1_MST_MASK; /* set MASTER mode */
	I2C1->C1 |= I2C_C1_TX_MASK; /* Set transmit (TX) mode */
	I2C1->D = 0xFF;
	while ((I2C1->S & I2C_S_IICIF_MASK) == 0U) {
	} /* wait interrupt */
	I2C1->S |= I2C_S_IICIF_MASK; /* clear interrupt bit */


							/* Clear arbitration error flag*/
	I2C1->S |= I2C_S_ARBL_MASK;


							/* Send start */
	I2C1->C1 &= ~I2C_C1_IICEN_MASK;
	I2C1->C1 |= I2C_C1_TX_MASK; /* Set transmit (TX) mode */
	I2C1->C1 |= I2C_C1_MST_MASK; /* START signal generated */

	I2C1->C1 |= I2C_C1_IICEN_MASK;
							/*Wait until start is send*/

							/* Send stop */
	I2C1->C1 &= ~I2C_C1_IICEN_MASK;
	I2C1->C1 |= I2C_C1_MST_MASK;
	I2C1->C1 &= ~I2C_C1_MST_MASK; /* set SLAVE mode */
	I2C1->C1 &= ~I2C_C1_TX_MASK; /* Set Rx */
	I2C1->C1 |= I2C_C1_IICEN_MASK;


								/* wait */
							/* Clear arbitration error & interrupt flag*/
	I2C1->S |= I2C_S_IICIF_MASK;
	I2C1->S |= I2C_S_ARBL_MASK;
	lock_detect=0;
	i2c_lock=1;
}

/* From Dean's code*/
void i2c_wait(void) {
	lock_detect = 0;
	while(((I2C1->S & I2C_S_IICIF_MASK)==0) & (lock_detect < 200)) {
		lock_detect++;
	}
	if (lock_detect >= 200)
		i2c_busy();
	I2C1->S |= I2C_S_IICIF_MASK;
}

void send_dev_add()
{
	I2C_TRAN;
	I2C_M_START;
	I2C1->D = DEV_ADD_WRITE; // send device address
	I2C_WAIT
}

int8_t write_data(uint8_t dev, uint8_t address, uint8_t data)
{
	/* Code written by me */
	/*start_signal();
	I2C1->D = DEV_ADD_WRITE;
	while(I2C1->S & I2C_S_BUSY_MASK); //I2C1 is busy transmitting data
	if(!(I2C1->S & I2C_S_TCF_MASK)) // If TCF flag is not set error in transmission
		return -1;

	I2C1->D = CMD;
	while(I2C1->S & I2C_S_BUSY_MASK); //I2C1 is busy transmitting data
	if(!(I2C1->S & I2C_S_TCF_MASK)) // If TCF flag is not set error in transmission
		return -1;

	I2C1->D = 0xAF;
	while(I2C1->S & I2C_S_BUSY_MASK); //I2C1 is busy transmitting data
	if(!(I2C1->S & I2C_S_TCF_MASK)) // If TCF flag is not set error in transmission
		return -1;
	stop_signal();*/

	/* From Dean's code */
	I2C_TRAN;
	I2C_M_START;
	I2C1->D = dev; // send device address
	I2C_WAIT

	I2C1->D = address; // send information regarding the next byte. Whether it is data or command
	I2C_WAIT

	I2C1->D = data; // Data or command
	I2C_WAIT
	I2C_M_STOP;
	//I2C_M_RSTART;

	//int i =10;
	//while(i--);

	return 0;
}

int8_t write_data_byte(uint8_t dev, uint8_t address, uint8_t data)
{
	/* From Dean's code */
	I2C_TRAN;
	I2C_M_START;
	I2C1->D = dev; // send device address
	I2C_WAIT

	I2C1->D = address; // send information regarding the next byte. Whether it is data or command
	I2C_WAIT

	I2C1->D = data; // Data or command
	I2C_WAIT
	I2C_M_STOP;



	return 0;
}
