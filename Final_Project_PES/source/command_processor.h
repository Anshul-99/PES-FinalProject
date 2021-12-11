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
​ * ​ ​ @file​ ​ command_processor.c
​ * ​ ​ @brief​ ​ This file implements a scheduler for a command processor using function pointers.
​ *
​ * ​ ​ @author​ ​ Anshul Somani
​ * ​ ​ @date​ ​ November 8 2021
​ * ​ ​ @version​ ​ 1.0
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

void display_help();

/*
 * Displays the author name
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */

void display_name();

/*
 * Displays the build version, date, machine and commit ID
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   void
 */

void display_info();

#endif /* COMMAND_PROCESSOR_H_ */
