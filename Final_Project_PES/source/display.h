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
​ * ​ ​ @file​ ​  display.h
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

#ifndef DISPLAY_H_
#define DISPLAY_H_

/*
 * Draws a square pixel by pixel as an animation.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void draw_square();

/*
 * Draws a rectangle pixel by pixel as an animation.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void draw_rectangle();

/*
 * Initializes the buffers needed to generate static shapes.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void init_buffers();

/*
 * Displays an animation of a tick mark that reappears.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void check_animation();

/*
 * Displays a square without any animation
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void square_display();

/*
 * Displays a rectangle without any animation
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void rectangle_display();

/*
 * Displays an image without any animation.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void shape_display_image(const uint8_t* arr);

#endif /* DISPLAY_H_ */
