/*
 * oled.c
 *
 *  Created on: 10-Dec-2021
 *      Author: anshul
 */

#include "oled.h"
#include "i2c.h"
#include <stdio.h>
#include <string.h>
#include "animation_lookup_table.h"
#include "image_lookup_table.h"

#define TOTAL_BYTES 2048

uint8_t table_pixel[TOTAL_BYTES];

uint8_t square_buff[TOTAL_BYTES];

uint8_t rect_buff[TOTAL_BYTES];

void init_display()
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

	init_buffers();
	clear_display();

}

void compute_rect()
{
	memset(&rect_buff[0],0,TOTAL_BYTES);

	for(uint8_t i =16 ; i<32; i++)
	{
		generate_buffer(i,8, &rect_buff[0]);
		generate_buffer(i,23, &rect_buff[0]);
	}

	for(uint8_t j =8 ; j<24; j++)
	{
		generate_buffer(16, j, &rect_buff[0]);
		generate_buffer(31, j, &rect_buff[0]);
	}

//	shape_display(&rect_buff[0]);
}

void compute_square()
{
	memset(&square_buff[0],0,TOTAL_BYTES);

	for(uint8_t i =16 ; i<32; i++)
	{
		generate_buffer(i,8, &square_buff[0]);
		generate_buffer(i,15, &square_buff[0]);
	}

	for(uint8_t j =8 ; j<16; j++)
	{
		generate_buffer(16, j, &square_buff[0]);
		generate_buffer(31, j, &square_buff[0]);
	}

//	shape_display(&square_buff[0]);
}

void init_buffers()
{
	memset(&table_pixel[0], 0, TOTAL_BYTES);

	compute_square();
	compute_rect();

}

void clear_display()
{
	write_data(DEV_ADD_WRITE, CMD, SET_ADDR_MODES);
	write_data(DEV_ADD_WRITE, CMD, HORIZONTAL_ADDR_MODE);

	write_data(DEV_ADD_WRITE, CMD, SET_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_COL_ADDR);

	write_data(DEV_ADD_WRITE, CMD, SET_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_PAGE_ADDR);

	for(uint16_t i =0; i<TOTAL_BYTES;i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
	}
}

void fill_display()
{
	write_data(DEV_ADD_WRITE, CMD, SET_ADDR_MODES);
	write_data(DEV_ADD_WRITE, CMD, HORIZONTAL_ADDR_MODE);

	write_data(DEV_ADD_WRITE, CMD, SET_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_COL_ADDR);

	write_data(DEV_ADD_WRITE, CMD, SET_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_PAGE_ADDR);

	for(uint16_t i =0; i<TOTAL_BYTES;i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, 0xFF);
	}
}

void toggle_pixel(uint8_t x_coordinate, uint8_t y_coordinate, uint8_t state)
{
	volatile uint8_t page = 0;
	volatile uint8_t col =0;
	volatile uint8_t pixel =0;

	if((y_coordinate>=8)&&(y_coordinate< 16))
	{
		page =  ((y_coordinate+8)/8);
		pixel = (y_coordinate%8);
	}
	else if((y_coordinate>=16)&&(y_coordinate< 24))
	{
		page =  ((y_coordinate+16)/8);
		pixel = (y_coordinate%8);
	}
	else if((y_coordinate>=24)&&(y_coordinate< 32))
	{
		page =  ((y_coordinate+24)/8);
		pixel = (y_coordinate%8);
	}
	else
	{
		page =  (y_coordinate/8);
		pixel = (y_coordinate%8);
	}

	col = x_coordinate;

	uint8_t data = 0x01<<pixel;

	printf("page: %d, col: %d, pixel: %d, data: %d\n\r", page,col,pixel, data);

	write_data(DEV_ADD_WRITE, CMD, SET_ADDR_MODES);
	write_data(DEV_ADD_WRITE, CMD, HORIZONTAL_ADDR_MODE);

	write_data(DEV_ADD_WRITE, CMD, SET_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_COL_ADDR);

	write_data(DEV_ADD_WRITE, CMD, SET_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_PAGE_ADDR);


	table_pixel[page + (col*16)] |= data;

	for(uint16_t i = 0; i<TOTAL_BYTES; i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, table_pixel[i]);
	}

}

void generate_buffer(uint8_t x_coordinate, uint8_t y_coordinate, uint8_t* arr)
{
	volatile uint8_t page = 0;
	volatile uint8_t col =0;
	volatile uint8_t pixel =0;

	if((y_coordinate>=8)&&(y_coordinate< 16))
	{
		page =  ((y_coordinate+8)/8);
		pixel = (y_coordinate%8);
	}
	else if((y_coordinate>=16)&&(y_coordinate< 24))
	{
		page =  ((y_coordinate+16)/8);
		pixel = (y_coordinate%8);
	}
	else if((y_coordinate>=24)&&(y_coordinate< 32))
	{
		page =  ((y_coordinate+24)/8);
		pixel = (y_coordinate%8);
	}
	else
	{
		page =  (y_coordinate/8);
		pixel = (y_coordinate%8);
	}

	col = x_coordinate;

	uint8_t data = 0x01<<pixel;
	arr[page + (col*16)] |= data;
}

void check_animation()
{
	clear_display();
	write_data(DEV_ADD_WRITE, CMD, SET_ADDR_MODES);
	write_data(DEV_ADD_WRITE, CMD, HORIZONTAL_ADDR_MODE);

	write_data(DEV_ADD_WRITE, CMD, SET_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_COL_ADDR);

	write_data(DEV_ADD_WRITE, CMD, SET_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_PAGE_ADDR);

	send_buff_animation(&check_gif_0[0]);
	send_buff_animation(&check_gif_1[0]);
	send_buff_animation(&check_gif_2[0]);
	send_buff_animation(&check_gif_3[0]);
	send_buff_animation(&check_gif_4[0]);
	send_buff_animation(&check_gif_5[0]);
	send_buff_animation(&check_gif_6[0]);
	send_buff_animation(&check_gif_7[0]);
	send_buff_animation(&check_gif_8[0]);
	send_buff_animation(&check_gif_9[0]);
	send_buff_animation(&check_gif_10[0]);
	send_buff_animation(&check_gif_11[0]);
	send_buff_animation(&check_gif_12[0]);
	send_buff_animation(&check_gif_13[0]);
	send_buff_animation(&check_gif_14[0]);
	send_buff_animation(&check_gif_15[0]);
	send_buff_animation(&check_gif_16[0]);
	send_buff_animation(&check_gif_17[0]);
	send_buff_animation(&check_gif_18[0]);
	send_buff_animation(&check_gif_19[0]);
	send_buff_animation(&check_gif_20[0]);
	send_buff_animation(&check_gif_21[0]);
	send_buff_animation(&check_gif_22[0]);
	send_buff_animation(&check_gif_23[0]);
	send_buff_animation(&check_gif_24[0]);
	send_buff_animation(&check_gif_25[0]);
	send_buff_animation(&check_gif_26[0]);
	send_buff_animation(&check_gif_27[0]);
	send_buff_animation(&check_gif_28[0]);
}

void square_display()
{
	clear_display();

	write_data(DEV_ADD_WRITE, CMD, SET_ADDR_MODES);
	write_data(DEV_ADD_WRITE, CMD, HORIZONTAL_ADDR_MODE);

	write_data(DEV_ADD_WRITE, CMD, SET_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_COL_ADDR);

	write_data(DEV_ADD_WRITE, CMD, SET_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_PAGE_ADDR);

	for(uint16_t j =0; j<TOTAL_BYTES; j++)
	{
		write_data(DEV_ADD_WRITE, DATA_, square_buff[j]);
	}
}

void rectangle_display()
{
	clear_display();

	write_data(DEV_ADD_WRITE, CMD, SET_ADDR_MODES);
	write_data(DEV_ADD_WRITE, CMD, HORIZONTAL_ADDR_MODE);

	write_data(DEV_ADD_WRITE, CMD, SET_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_COL_ADDR);

	write_data(DEV_ADD_WRITE, CMD, SET_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_PAGE_ADDR);

	for(uint16_t j =0; j<TOTAL_BYTES; j++)
	{
		write_data(DEV_ADD_WRITE, DATA_, rect_buff[j]);
	}
}

void shape_display_image(uint8_t* arr)
{
	clear_display();
	write_data(DEV_ADD_WRITE, CMD, SET_ADDR_MODES);
	write_data(DEV_ADD_WRITE, CMD, HORIZONTAL_ADDR_MODE);

	write_data(DEV_ADD_WRITE, CMD, SET_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_COL_ADDR);

	write_data(DEV_ADD_WRITE, CMD, SET_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_PAGE_ADDR);

	/*for(uint16_t i =0; i<512; i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, shape_fish_64[i]);
	}

	for(uint16_t i =0; i<256; i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, shape_fish_32[i]);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
	}*/

	for(uint16_t i =0; i<256; i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, arr[i]);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
	}
}

void send_buff_animation(uint8_t* buffer)
{
	for(uint16_t i =0; i<256; i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, buffer[i]);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
	}
	for(uint16_t i =0; i<256; i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
		write_data(DEV_ADD_WRITE, DATA_, 0x00);
	}
}

void draw_square()
{
	clear_display();
	for(uint8_t i = 16; i<32;i++)
	{
		toggle_pixel(i,8,1);
		toggle_pixel(i,15,1); //23 for rect
	}

	for(uint8_t j = 8; j<16;j++) //24 for rectangle
	{
		toggle_pixel(16,j,1);
		toggle_pixel(31,j,1);
	}
}

void draw_rectangle()
{
	clear_display();
	for(uint8_t i = 16; i<32;i++)
	{
		toggle_pixel(i,8,1);
		toggle_pixel(i,23,1); //23 for rect
	}

	for(uint8_t j = 8; j<24;j++) //24 for rectangle
	{
		toggle_pixel(16,j,1);
		toggle_pixel(31,j,1);
	}
}





