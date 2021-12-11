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
​ * ​ ​ @file​ ​ hexdump.c
​ * ​ ​ @brief​ ​ Implementation to read data from memory
 *	 	 	 and display it on the screen in chunks of 16 bytes
​ *
​ * ​ ​ @author​ ​ Anshul Somani
​ * ​ ​ @date​ ​ November 8 2021
​ * ​ ​ @version​ ​ 1.0
​ *
​ */
#include "MKL25Z4.h"
#include "hexdump.h"
#include <stdio.h>
#include <string.h>

#define ZERO_ASCII 0x30
#define NINE_ASCII 0x39

/*
 * Converts numerical values in a string to integer format
 *
 * Parameters:
 *   char* input: contains the numerical value in char format
 *
 * Returns:
 *   uint16_t : numerical value in integer format.
 */

uint16_t char_to_int(char* input)
{
	uint8_t cntr = 0;
	uint16_t temp = 0;
	while(input[cntr] != '\0') // exit the loop when the terminatin char is reached
	{
		temp = temp*10;
		if (((uint8_t)(input[cntr]) >= ZERO_ASCII ) && ((uint8_t)(input[cntr]) <= NINE_ASCII))
		{
			temp += (((uint8_t)input[cntr])-ZERO_ASCII);
		}
		cntr++;
	}
	return temp;
}

/*
 * Converts numerical values in a string to hex format
 *
 * Parameters:
 *   char* input: contains the numerical value in char format
 *
 * Returns:
 *   uint16_t : numerical value in hex format.
 */

uint16_t char_to_hex(char* input)
{
	uint8_t cntr =0;
	uint16_t temp =0;
	while(input[cntr] != '\0')
	{
		temp = temp*16;
		if(((uint8_t)(input[cntr])>0x40) && ((uint8_t)(input[cntr])<0x47))
		{
			temp += (((uint8_t)input[cntr])-0x37);
		}
		else if (((uint8_t)(input[cntr])>0x60) && ((uint8_t)(input[cntr])<0x67))
		{
			temp += (((uint8_t)input[cntr])-0x57);
		}
		else if (((uint8_t)(input[cntr])>0x2F) && ((uint8_t)(input[cntr])<0x3A))
		{
			temp += (((uint8_t)input[cntr])-0x30);
		}
		cntr++;
	}
	return temp;
}

void hexdump(int argc, char* argv[])
{

	uint32_t add_value =0;
	uint32_t length =0;

	add_value = char_to_hex(argv[1]);

	if(((argv[2])[0] == '0') && ((argv[2])[1] == 'x'))
	{
		length = char_to_hex(argv[2]);
	}
	else
	{
		length = char_to_int(argv[2]);
	}

	char* ptr = add_value;

	int cntr =0;
	for(int i =0; i<=(length/16); i++)
	{
		printf("%p:", ptr);
		for (int j =0; j< 16; j++)
		{
			printf(" %x ", *ptr);
			ptr++;
			cntr++;
			if(cntr == length)
				break;
		}
		printf("\n\r");
	}
}
