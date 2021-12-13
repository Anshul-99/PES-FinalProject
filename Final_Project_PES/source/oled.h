/*
 * oled.h
 *
 *  Created on: 10-Dec-2021
 *      Author: anshul
 */

#ifndef OLED_H_
#define OLED_H_
#include <stdint.h>

#define DISPLAY_OFF 0xAE
#define SET_DISPLAY_CLK 0XD5
#define DISPLAY_CLK_FREQ 0XF0
#define SET_MUX_RATIO 0xA8
#define MUX_RATIO 0x3F
#define SET_DISPLAY_OFFSET 0XD3
#define DISPLAY_OFFSET 0X00
#define SET_DISPLAY_STARTLINE 0X40
#define ENABLE_CHARGE_PUMP_REG 0X8D
#define CHARGE_PUMP_REG 0X10
#define INT_IREF_SETTING 0XAD
#define DISABLE_IREF 0X20
#define SET_SEG_REMAP 0XA1 //A0
#define SET_COM_SCAN_DIR 0XC8
#define SET_COMPINS_HW_CONFIG 0XDA
#define COMPINS_HW_CONFIG 0X12
#define SET_CONTRAST_CNTRL 0X81
#define CONTRAST_CNTRL 0XFF
#define SET_PRECHARGE_VAL 0XD9
#define PRECHARGE_VAL 0X22
#define SET_DESELECT_VCOMH_LEVEL 0XDB
#define DESELECT_VCOMH_LEVEL 0X30
#define ENTIRE_DISPLAY_ON 0XA4
#define SET_NORMAL_DISPLAY 0XA6
#define DISPLAY_ON 0XAF

#define SET_ADDR_MODES 0X20
#define HORIZONTAL_ADDR_MODE 0X00
#define SET_COL_ADDR 0X21
#define START_COL_ADDR 0X00
#define END_COL_ADDR 0X7F
#define SET_PAGE_ADDR 0X22
#define START_PAGE_ADDR 0X00
#define END_PAGE_ADDR 0X07

void init_display();

void clear_display();

void fill_display();

void toggle_pixel(uint8_t x_coordinate, uint8_t y_coordinate, uint8_t state);

void draw_square();

void draw_rectangle();

void generate_buffer(uint8_t x_coordinate, uint8_t y_coordinate, uint8_t* arr);

void compute_rect();

void compute_square();

void send_buff_animation(uint8_t* buffer);

void init_buffers();

void check_animation();

void square_display();

void rectangle_display();

void shape_display_image(uint8_t* arr);

#endif /* OLED_H_ */

