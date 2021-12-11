/*
 * oled.c
 *
 *  Created on: 10-Dec-2021
 *      Author: anshul
 */

#include "oled.h"
#include "i2c.h"
#include <stdio.h>

#define TOTAL_BYTES 2048

static uint8_t shape1[] = {
		 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x01,
		 0x00,0x00,0xf0,0x0f,0x00,0x00,0xfe,0x7f,0x00,0x00,0x03,0xe0,0x00,0x80,0x01,
		 0x80,0x01,0x80,0x00,0x00,0x03,0x60,0x00,0x00,0x03,0x60,0x00,0x00,0x06,0x20,
		 0x00,0x00,0x0c,0x30,0x00,0x00,0x0c,0x30,0x00,0x00,0x0c,0x10,0x00,0x00,0x08,
		 0x10,0x00,0x00,0x0c,0x10,0x00,0x00,0x08,0x30,0x00,0x00,0x0c,0x10,0x00,0x00,
		 0x08,0x30,0x00,0x00,0x04,0x20,0x00,0x00,0x04,0x60,0x00,0x00,0x06,0x60,0x00,
		 0x00,0x02,0x80,0x01,0x00,0x01,0x80,0x01,0x80,0x01,0x80,0x07,0xd0,0x00,0x00,
		 0xbc,0x3d,0x00,0x00,0xf4,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

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
	uint8_t page = 0;
	uint8_t col =0;
	uint8_t pixel =0;

	col = x_coordinate;
	page = 7- (y_coordinate/8);
	pixel = 7 - (y_coordinate%8);
	uint8_t data = 0x01<<pixel;

	printf("page: %d, col: %d, pixel: %d, data: %d\n\r", page,col,pixel, data);

	write_data(DEV_ADD_WRITE, CMD, SET_ADDR_MODES);
	write_data(DEV_ADD_WRITE, CMD, HORIZONTAL_ADDR_MODE);

	write_data(DEV_ADD_WRITE, CMD, SET_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, col);
	write_data(DEV_ADD_WRITE, CMD, col);

	write_data(DEV_ADD_WRITE, CMD, SET_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, page);
	write_data(DEV_ADD_WRITE, CMD, page);

	write_data(DEV_ADD_WRITE, DATA_, data);
}

void shape_display()
{
	write_data(DEV_ADD_WRITE, CMD, SET_ADDR_MODES);
	write_data(DEV_ADD_WRITE, CMD, HORIZONTAL_ADDR_MODE);

	write_data(DEV_ADD_WRITE, CMD, SET_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_COL_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_COL_ADDR);

	write_data(DEV_ADD_WRITE, CMD, SET_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, START_PAGE_ADDR);
	write_data(DEV_ADD_WRITE, CMD, END_PAGE_ADDR);

	for(uint16_t i =0; i<128; i++)
	{
		write_data(DEV_ADD_WRITE, DATA_, shape1[i]);
	}
}
