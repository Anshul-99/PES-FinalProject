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
#include "display.h"

#define DEBUG 1

int main(void)
{
	/* initialize the system clock*/
	sysclock_init();

	/* initialize UART for communication with serial termnial*/
	init_uart();

	/* initialize I2C for communication with OLED display*/
	init_i2c();

	/* initialize the OLED */
	init_oled();

	/* initialize a few buffers being used to display shapes
	 * on the OLED display*/
	init_buffers();

	/* Test the OLED display to ensure that all the pixels are working
	 * and it has been configured properly*/
	fill_oled();

	clear_oled();

	/* Begin command processor */
	char* input_str = NULL;
	char array_input[256];

	printf("Welcome to BreakfastSerial! \n\r");

	 while(1)
	 {
		printf("? ");
		input_str= copy_gets(&(array_input[0]));
		process_command(input_str);
	 }

	  return 0 ;
}
