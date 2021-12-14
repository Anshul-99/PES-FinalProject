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
​ * ​ ​ @file​ ​ hexdump.h
​ * ​ ​ @brief​ ​ Implementation to read data from memory
 *	 	 	 and display it on the screen in chunks of 16 bytes
​ *
​ * ​ ​ @author​ ​ Anshul Somani
​ * ​ ​ @date​ ​ November 8 2021
​ * ​ ​ @version​ ​ 1.0
​ *
​ */

#ifndef HEXDUMP_H_
#define HEXDUMP_H_

/*
 * Prints the data present in memory address
 * It takes the starting address and the length from the user.
 *
 * Parameters:
 *   char* argv[]: command tokens received from the user. Contains the argument
 *   int argc: number of tokens in argv[]
 *
 * Returns:
 *   void
 */

void hexdump(int argc, char* argv[]);

/*
 * Converts numerical values in a string to integer format
 *
 * Parameters:
 *   char* input: contains the numerical value in char format
 *
 * Returns:
 *   uint16_t : numerical value in integer format.
 */

uint16_t char_to_int(char* input);

#endif /* HEXDUMP_H_ */
