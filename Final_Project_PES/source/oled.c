/*
 * oled.c
 *
 *  Created on: 10-Dec-2021
 *      Author: anshul
 */

#include "oled.h"
#include "i2c.h"

void init_display()
{
	//send_dev_add();

	write_data(DEV_ADD_WRITE, CMD, DISPLAY_OFF);

	write_data(DEV_ADD_WRITE, CMD, SET_DISPLAY_CLK);
	write_data(DEV_ADD_WRITE, DATA_, DISPLAY_CLK_FREQ);

	write_data(DEV_ADD_WRITE, CMD, SET_MUX_RATIO);
	write_data(DEV_ADD_WRITE, DATA_, MUX_RATIO);

	write_data(DEV_ADD_WRITE, CMD, SET_DISPLAY_OFFSET);
	write_data(DEV_ADD_WRITE, DATA_, DISPLAY_OFFSET);

	write_data(DEV_ADD_WRITE, CMD, SET_DISPLAY_STARTLINE);

	write_data(DEV_ADD_WRITE, CMD, ENABLE_CHARGE_PUMP_REG);
	write_data(DEV_ADD_WRITE, DATA_, CHARGE_PUMP_REG);

	write_data(DEV_ADD_WRITE, CMD, INT_IREF_SETTING);
	write_data(DEV_ADD_WRITE, DATA_, DISABLE_IREF);

	write_data(DEV_ADD_WRITE, CMD, SET_SEG_REMAP);

	write_data(DEV_ADD_WRITE, CMD, SET_COM_SCAN_DIR);

	write_data(DEV_ADD_WRITE, CMD, SET_COMPINS_HW_CONFIG);
	write_data(DEV_ADD_WRITE, DATA_, COMPINS_HW_CONFIG);

	write_data(DEV_ADD_WRITE, CMD, SET_CONTRAST_CNTRL);
	write_data(DEV_ADD_WRITE, DATA_, CONTRAST_CNTRL);

	write_data(DEV_ADD_WRITE, CMD, SET_PRECHARGE_VAL);
	write_data(DEV_ADD_WRITE, DATA_, PRECHARGE_VAL);

	write_data(DEV_ADD_WRITE, CMD, SET_DESELECT_VCOMH_LEVEL);
	write_data(DEV_ADD_WRITE, DATA_, DESELECT_VCOMH_LEVEL);

	write_data(DEV_ADD_WRITE, CMD, ENTIRE_DISPLAY_ON);

	write_data(DEV_ADD_WRITE, CMD, SET_NORMAL_DISPLAY);

	write_data(DEV_ADD_WRITE, CMD, DISPLAY_ON);

	//I2C_M_STOP;
}
