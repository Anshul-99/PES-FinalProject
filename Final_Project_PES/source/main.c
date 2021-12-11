/*
 * main.c - application entry point
 * 
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 */
#include <command_processor.h>
#include <uart.h>
#include "sysclock.h"
#include "MKL25Z4.h"
#include <stdio.h>
#include <assert.h>
#include "cbfifo.h"
#include "i2c.h"
#include "oled.h"

#define DEBUG 1

int main(void)
{
	sysclock_init();

	init_uart();

	init_i2c();
	init_display();

	I2C1->C1 &= ~I2C_C1_IICEN(1);

	//int8_t result = write_data(DEV_ADD_WRITE, CMD, 0xAF);

	uint8_t i =0;

	 while(1)
	 {
		 i++;
	 }

	  return 0 ;
}
