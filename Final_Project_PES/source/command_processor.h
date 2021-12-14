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
​ * ​ ​ @file​ ​ command_processor.h
​ * ​ ​ @brief​ ​ This file implements a scheduler for a command processor using function pointers.
​ *
​ * ​ ​ @author​ ​ Anshul Somani
​ * ​ ​ @date​ ​ December 12 2021
​ * ​ ​ @version​ ​ 2.0
​ *
​ */

#ifndef COMMAND_PROCESSOR_H_
#define COMMAND_PROCESSOR_H_

/*
 * Converts string input from user to tokens
 * which are then used to perform the appropriate tasks
 *
 * Parameters:
 *   char* input: contains the input from the user
 *
 * Returns:
 *   void
 */

void process_command(char *input);

/*
 * Displays the help strings for all the supported commands
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */

void help_func();

/*
 * Displays the author name
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */

void name_func();

/*
 * Displays a shape. The shape depends on the argument.
 *
 * Parameters:
 *   int argc: number of tokens in the command.
 *   char* argv[]: array of all the tokens in the command.
 *
 * Returns:
 *   void
 */
void shape_func(int argc, char* argv[]);

/*
 * Draws a shape as an animation. The shape depends on the argument.
 *
 * Parameters:
 *   int argc: number of tokens in the command.
 *   char* argv[]: array of all the tokens in the command.
 *
 * Returns:
 *   void
 */
void draw_func(int argc, char* argv[]);

/*
 * Shows an animation based on the argument
 *
 * Parameters:
 *   int argc: number of tokens in the command.
 *   char* argv[]: array of all the tokens in the command.
 *
 * Returns:
 *   void
 */
void gif_func(int argc, char* argv[]);

/*
 * Fills the OLED display
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void fill_func();

/*
 * Clears the OLED display
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */
void clear_func();

/*
 * toggles a pixel ON/OFF
 *
 * Parameters:
 *   int argc: number of tokens in the command.
 *   char* argv[]: array of all the tokens in the command.
 *
 * Returns:
 *   void
 */
void toggle_func(int argc, char* argv[]);

#endif /* COMMAND_PROCESSOR_H_ */
