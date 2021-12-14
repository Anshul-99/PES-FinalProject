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
​ * ​ ​ @file​ ​ oled.c
​ * ​ ​ @brief​ ​ This file implements functions needed to initialize, configure
 *     		 and then control the OLED display.
 *     		 For this project, a 128x64 OLED display has been used. The OLED
 *     		 driver/controller is SSD1315 but the code seems to be compatible
 *     		 with OLED displays having the same resolution and SSD1306 as the
 *     		 OLED driver/ controller.
​ *
​ * ​ ​ @author​ ​ Anshul Somani
​ * ​ ​ @date​ ​ December 10 2021
​ * ​ ​ @version​ ​ 1.0
​ *
​ */

#include "oled.h"
#include "i2c.h"
#include <string.h>
#include <stdio.h>

/* The buffer sent to OLED to control pixels. It keeps information regarding
 * which pixel is supposed to be ON and which are supposed to be OFF*/

uint8_t table_pixel[TOTAL_BYTES];

#define CLEAR_SCREEN 0x00 /* Switches OFF all the pixels mapped to the display RAM that stores this value */
#define FILL_SCREEN 0xFF /* Switches ON all the pixels mapped to the display RAM that stores this value */

void init_oled()
{

	write_data(DEV_ADD_WRITE, CMD, DISPLAY_OFF);

	write_data(DEV_ADD_WRITE, CMD, SET_DISPLAY_CLK);
	write_data(DEV_ADD_WRITE, CMD, DISPLAY_CLK_FREQ);

	write_data(DEV_ADD_WRITE, CMD, SET_MUX_RATIO);
	write_data(DEV_ADD_WRITE, CMD, MUX_RATIO);

	write_data(DEV_ADD_WRITE, CMD, SET_DISPLAY_OFFSET);
	write_data(DEV_ADD_WRITE, CMD, DISPLAY_OFFSET);

	write_data(DEV_ADD_WRITE, CMD, SET_DISPLAY_STARTLINE);

	write_data(DEV_ADD_WRITE, CMD, ENABLE_CHARGE_PUMP_REG);
	write_data(DEV_ADD_WRITE, CMD, CHARGE_PUMP_REG);

	write_data(DEV_ADD_WRITE, CMD, INT_IREF_SETTING);
	write_data(DEV_ADD_WRITE, CMD, DISABLE_IREF);

	write_data(DEV_ADD_WRITE, CMD, SET_SEG_REMAP);

	write_data(DEV_ADD_WRITE, CMD, SET_COM_SCAN_DIR);

	write_data(DEV_ADD_WRITE, CMD, SET_COMPINS_HW_CONFIG);
	write_data(DEV_ADD_WRITE, CMD, COMPINS_HW_CONFIG);

	write_data(DEV_ADD_WRITE, CMD, SET_CONTRAST_CNTRL);
	write_data(DEV_ADD_WRITE, CMD, CONTRAST_CNTRL);

	write_data(DEV_ADD_WRITE, CMD, SET_PRECHARGE_VAL);
	write_data(DEV_ADD_WRITE, CMD, PRECHARGE_VAL);

	write_data(DEV_ADD_WRITE, CMD, SET_DESELECT_VCOMH_LEVEL);
	write_data(DEV_ADD_WRITE, CMD, DESELECT_VCOMH_LEVEL);

	write_data(DEV_ADD_WRITE, CMD, ENTIRE_DISPLAY_ON);

	write_data(DEV_ADD_WRITE, CMD, SET_NORMAL_DISPLAY);

	write_data(DEV_ADD_WRITE, CMD, DISPLAY_ON);

	clear_oled(); /* Clear the screen to begin from a clean slate */

}

void init_data_oled()
{
	write_data(DEV_ADD_WRITE, CMD, SET_ADDR_MODES);
	write_data(DEV_ADD_WRITE, CMD, HORIZONTAL_ADDR_MODE);

	write_data(DEV_ADD_WRITE, CMD, SET_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_COL_ADDR);

	write_data(DEV_ADD_WRITE, CMD, SET_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_PAGE_ADDR);
}

void clear_oled()
{
	/* To ensure that any pixels that were toggled are not toggled later on.
	 * Helps ensure that the toggled pixels don't display in the middle of any animation, shape, image.*/
	memset(&table_pixel[0], 0, TOTAL_BYTES);

	init_data_oled();

	/* Send 2048 bytes to clear the entire screen */
	for(uint16_t i =0; i<TOTAL_BYTES;i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, CLEAR_SCREEN);
	}
}

void fill_oled()
{
	init_data_oled();

	/* Send 2048 bytes to fill the entire screen */
	for(uint16_t i =0; i<TOTAL_BYTES;i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, FILL_SCREEN);
	}
}

void toggle_pixel_oled(uint8_t x_coordinate, uint8_t y_coordinate, uint8_t state)
{
	/* The display is divided into 8 horizontal rows called 'pages' and
	 * 128 vertical columns called 'cols'. Each col has 64 pixels and each
	 * page has 128*8 pages. Each byte maps to 1 col and 1 page.
	 *
	 * 			  col_0 			col_1 col_2 .....
	 * page3&7    pixel1 of page 3
	 * 			  pixel1 of page 7
	 * 			  pixel2 of page 3
	 * 			  pixel2 of page 7
	 * 			  pixel3 of page 3
	 * 			  pixel3 of page 7
	 * 			  pixel4 of page 3
	 * 			  pixel4 of page 7
	 * 			  pixel5 of page 3
	 * 			  pixel5 of page 7
	 * 			  pixel6 of page 3
	 * 			  pixel6 of page 7
	 * 			  pixel7 of page 3
	 * 			  pixel7 of page 7
	 * 			  pixel8 of page 3
	 * 			  pixel8 of page 7
	 * page2&6
	 * page1&5
	 * page0&4
	 *
	 * The co-ordinates entered by the user is converted to the col, page
	 * and the particular pixel on the byte mapped to that page.
	 */

	if(!((x_coordinate>=0)&&(x_coordinate<128)))
	{
		printf("Invalid X co-ordinate\n\r");
		return;
	}
	else if(!((y_coordinate>=0)&&(y_coordinate<32)))
	{
		printf("Invalid Y co-ordinate\n\r");
		return;
	}
	else if ((state !=0)&&(state !=1))
	{
		printf("Invalid state\n\r");
		return;
	}

	volatile uint8_t page = 0;
	volatile uint8_t col =0;
	volatile uint8_t pixel =0;

	/* This logic is used to alternate between the pages. Only pages, 0,1,2,3 are accessed in this functionality.
	 * This is because if the user enters (0,32), it maps page 4 that is present with page0. not with page3*/

	if((y_coordinate>=8)&&(y_coordinate< 16)) /* page 1*/
	{
		page =  ((y_coordinate+8)/8);
		pixel = (y_coordinate%8);
	}
	else if((y_coordinate>=16)&&(y_coordinate< 24)) /* page 2*/
	{
		page =  ((y_coordinate+16)/8);
		pixel = (y_coordinate%8);
	}
	else if((y_coordinate>=24)&&(y_coordinate< 32)) /* page 3 */
	{
		page =  ((y_coordinate+24)/8);
		pixel = (y_coordinate%8);
	}
	else /* page 0 */
	{
		page =  (y_coordinate/8);
		pixel = (y_coordinate%8);
	}

	col = x_coordinate;

	uint8_t data = 0;
	switch(state)
	{
	case ON:
		 data = 0x01<<pixel;

		/* sets 1 for the bit that maps to the pixel and
		 * sends the entire byte to the display */
		table_pixel[page + (col*16)] |= data;
		break;
	case OFF:
		data = ~(0x01<<pixel);

		/* sets  0 for the bit that maps to the pixel and
		 * sends the entire byte to the display */
		table_pixel[page + (col*16)] &= data;
		break;
	}

	init_data_oled();



	for(uint16_t i = 0; i<TOTAL_BYTES; i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, table_pixel[i]);
	}

}

void send_animation_buff(const uint8_t* buffer)
{
	/* sends the entire buffer to the OLED display and then
	 * pads the remaining values with 0 to ensure that the frames
	 * don't start shifiting across the display.
	 */
	for(uint16_t i =0; i<256; i++) /* 512 bytes */
	{
		write_data(DEV_ADD_WRITE, DATA_, buffer[i]);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
	}
	for(uint16_t i =0; i<256; i++) /* 1536 bytes. 512 +1536 = 2048 */
	{
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
	}
}









