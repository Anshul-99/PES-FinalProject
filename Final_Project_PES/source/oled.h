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
​ * ​ ​ @file​ ​ oled.h
​ * ​ ​ @brief​ ​ This file implements functions needed to initialize, configure
 *     		 and then control the OLED display.
 *     		 For this project, a 128x64 OLED display has been used. The OLED
 *     		 driver/controller is SSD1315 but the code seems to be compatible
 *     		 with OLED displays having the same resolution and SSD1306 as the
 *     		 OLED driver/ controller.
​ *
​ * ​ ​ @author​ ​ Anshul Somani
​ * ​ ​ @date​ ​ November 8 2021
​ * ​ ​ @version​ ​ 1.0
​ *
​ */

#ifndef OLED_H_
#define OLED_H_
#include <stdint.h>

/* commands used to initialize the OLED display.
 * The commands have been referred from the application notes for
 * SSD1315 and SSD1306.
 */
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

#define TOTAL_BYTES 2048 /* Total number of bytes needed to control the entire OLED screen */

enum pixel_state{OFF, ON};
//enum state pixel_state;

/*
 * Initializes the OLED display before anything can be displayed on it.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void init_oled();

/*
 * Initializes the OLED display. It tells the display where exactly to
 * begin displaying the buffer from, the addressing mode, etc.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void init_data_oled();

/*
 * Clears the OLED display.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void clear_oled();

/*
 * Switches ON all the pixels on the OLED display.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void fill_oled();

/*
 * Toggles a particular pixel ON or OFF.
 *
 * The bottom left corner is 0,0. The entire screen is a part of the
 * 1st quadrant.
 *
 * Parameters:
 *   uint8_t x_coordinate: The x co-ordinate of the pixel to be controlled.
 *   uint8_t y_coordinate: The y co-ordinate of the pixel to be controlled.
 *   uint8_t state: The state of the pixel. (ON/OFF).
 *
 * Returns:
 *   void
 */
void toggle_pixel_oled(uint8_t x_coordinate, uint8_t y_coordinate, uint8_t state);

/*
 * Transmits the animation buffer to the display.
 * It ensures that the frames don't shift while transmitting.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void send_animation_buff(const uint8_t* buffer);

#endif /* OLED_H_ */

