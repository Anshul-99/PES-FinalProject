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

	//I2C1->C1 &= ~I2C_C1_IICEN(1);

	clear_display();

	fill_display();

	clear_display();

	/*char* input_str = NULL;
	char array_input[256];*/

	printf("Welcome to BreakfastSerial! \n\r");

	/*toggle_pixel(0,0,1);
	toggle_pixel(127,63,0);
	toggle_pixel(53,24,0);*/

	shape_display();

	uint8_t i =0;

	 while(1)
	 {
		 i++;
	 }

	  return 0 ;
}
