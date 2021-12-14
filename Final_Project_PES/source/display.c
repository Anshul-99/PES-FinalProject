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
​ * ​ ​ @file​ ​  display.c
​ * ​ ​ @brief​ ​ This file implements functions that display shapes, images and animations
 * 			 on the OLED display.
 *     		 For this project, a 128x64 OLED display has been used. The OLED
 *     		 driver/controller is SSD1315 but the code seems to be compatible
 *     		 with OLED displays having the same resolution and SSD1306 as the
 *     		 OLED driver/ controller.
​ *
​ * ​ ​ @author​ ​ Anshul Somani
​ * ​ ​ @date​ ​ December 13 2021
​ * ​ ​ @version​ ​ 1.0
​ *
​ */


#include "oled.h"
#include <string.h>
#include "animation_lookup_table.h"
#include "image_lookup_table.h"
#include "i2c.h"

/* buffer that stores the square shape when it needs to be displayed
 * on the screen. This buffer isn't used for animation purposes, only static
 * image purposes.
 */
uint8_t square_buff[TOTAL_BYTES];

/* buffer that stores the rectangle shape when it needs to be displayed
 * on the screen. This buffer isn't used for animation purposes, only static
 * image purposes.
 */
uint8_t rect_buff[TOTAL_BYTES];

void generate_buffer(uint8_t x_coordinate, uint8_t y_coordinate, uint8_t* arr)
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

    /* sets 1 or 0 for the bit that maps to the pixel and
     * stores it in the buffer provided as argument */
	col = x_coordinate;

	uint8_t data = 0x01<<pixel;
	arr[page + (col*16)] |= data;
}

/*
 * Computes the buffer to display animage of square on the OLED display.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void compute_rect()
{
	memset(&rect_buff[0],0,TOTAL_BYTES); /* Clear the buffer of anything garbage values */

	for(uint8_t i =16 ; i<32; i++) /* generate the vertical lines */
	{
		generate_buffer(i,8, &rect_buff[0]);
		generate_buffer(i,23, &rect_buff[0]);
	}

	for(uint8_t j =8 ; j<24; j++) /* generate the horizontal lines */
	{
		generate_buffer(16, j, &rect_buff[0]);
		generate_buffer(31, j, &rect_buff[0]);
	}
}

/*
 * Computes the buffer to display an image of rectangle on the OLED display.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void compute_square()
{
	memset(&square_buff[0],0,TOTAL_BYTES); /* Clear the buffer of anything garbage values */

	for(uint8_t i =16 ; i<32; i++) /* generate the vertical lines */
	{
		generate_buffer(i,8, &square_buff[0]);
		generate_buffer(i,15, &square_buff[0]);
	}

	for(uint8_t j =8 ; j<16; j++) /* generate the horizontal lines */
	{
		generate_buffer(16, j, &square_buff[0]);
		generate_buffer(31, j, &square_buff[0]);
	}
}

void init_buffers()
{
	/* compute the buffers before the shapes have to be displayed */
	compute_square();
	compute_rect();
}



void check_animation()
{
	/* clear display */
	clear_oled();

	init_data_oled();

	/* Send the frames for the animation to the display. Initially the plan was to send the
	 * frames after a time interval calculated using a timer. The transmit and refresh speed is
	 * so slow that there is no need to use a timer.
	 */
	send_animation_buff(&check_gif_0[0]);
	send_animation_buff(&check_gif_1[0]);
	send_animation_buff(&check_gif_2[0]);
	send_animation_buff(&check_gif_3[0]);
	send_animation_buff(&check_gif_4[0]);
	send_animation_buff(&check_gif_5[0]);
	send_animation_buff(&check_gif_6[0]);
	send_animation_buff(&check_gif_7[0]);
	send_animation_buff(&check_gif_8[0]);
	send_animation_buff(&check_gif_9[0]);
	send_animation_buff(&check_gif_10[0]);
	send_animation_buff(&check_gif_11[0]);
	send_animation_buff(&check_gif_12[0]);
	send_animation_buff(&check_gif_13[0]);
	send_animation_buff(&check_gif_14[0]);
	send_animation_buff(&check_gif_15[0]);
	send_animation_buff(&check_gif_16[0]);
	send_animation_buff(&check_gif_17[0]);
	send_animation_buff(&check_gif_18[0]);
	send_animation_buff(&check_gif_19[0]);
	send_animation_buff(&check_gif_20[0]);
	send_animation_buff(&check_gif_21[0]);
	send_animation_buff(&check_gif_22[0]);
	send_animation_buff(&check_gif_23[0]);
	send_animation_buff(&check_gif_24[0]);
	send_animation_buff(&check_gif_25[0]);
	send_animation_buff(&check_gif_26[0]);
	send_animation_buff(&check_gif_27[0]);
	send_animation_buff(&check_gif_28[0]);
}

void square_display()
{
	/* clear display */
	clear_oled();

	init_data_oled();

	/* transmit the buffer containing the values for a square shape to the display*/
	for(uint16_t j =0; j<TOTAL_BYTES; j++)
	{
		write_data(DEV_ADD_WRITE, DATA_, square_buff[j]);
	}
}

void rectangle_display()
{
	/* clear display */
	clear_oled();

	init_data_oled();

	/* transmit the buffer containing the values for a rectangle shape to the display*/
	for(uint16_t j =0; j<TOTAL_BYTES; j++)
	{
		write_data(DEV_ADD_WRITE, DATA_, rect_buff[j]);
	}
}

void shape_display_image(uint8_t* arr)
{
	/* clear display */
	clear_oled();

	init_data_oled();

	/*for(uint16_t i =0; i<512; i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, shape_fish_64[i]);
	}

	for(uint16_t i =0; i<256; i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, shape_fish_32[i]);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
	}*/

	/* transmit the buffer containing the values for an image to the display*/
	for(uint16_t i =0; i<256; i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, arr[i]);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
	}
	/* Pad the rest of the screen with 0 to present any images from
	 * shifting on the screen. */
	for(uint16_t i =0; i<(512*3); i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
	}
}

void draw_square()
{
	/* clear display */
	clear_oled();

	/* draw the vertical lines, pixel by pixel. */
	for(uint8_t i = 16; i<32;i++)
	{
		toggle_pixel_oled(i,8,ON);
		toggle_pixel_oled(i,15,ON);
	}

	/* draw the horizontal lines, pixel by pixel.*/
	for(uint8_t j = 8; j<16;j++)
	{
		toggle_pixel_oled(16,j,ON);
		toggle_pixel_oled(31,j,ON);
	}
}

void draw_rectangle()
{
	/* clear display */
	clear_oled();

	/* draw the vertical lines, pixel by pixel. */
	for(uint8_t i = 16; i<32;i++)
	{
		toggle_pixel_oled(i,8,ON);
		toggle_pixel_oled(i,23,ON);
	}

	/* draw the horizontal lines, pixel by pixel.*/
	for(uint8_t j = 8; j<24;j++)
	{
		toggle_pixel_oled(16,j,ON);
		toggle_pixel_oled(31,j,ON);
	}
}
