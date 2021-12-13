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

	fill_display();

	clear_display();

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
