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
​ * ​ ​ @date​ ​ December 12 2021
​ * ​ ​ @version​ ​ 2.0
​ *
​ */

#include "MKL25Z4.h"
#include "sysclock.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "command_processor.h"
#include "cbfifo.h"
#include "uart.h"
#include "hexdump.h"
#include "image_lookup_table.h"
#include "oled.h"
#include "display.h"


#define STR1 "Author"
#define STR2 "Help"

#define STR3 "Display"
#define STR4 "Print"
#define STR5 "Draw"
#define STR6 "Show"

#define STR7 "Clear"
#define STR8 "Fill"
#define STR9 "Toggle_pixel"

#define AUTHOR_NAME "Anshul Somani"

#define HELP_AUTHOR "1. Author <None> : \n\r Arguments: None\n\r Action: It prints the name of the user\n\r"
#define HELP_HELP "2. Help <None> : \n\r Arguments: None\n\r Action: It prints information regarding all the supported commands.\n\r"
#define HELP_DISPLAY "3. Display <Shape> : \n\r Arguments: String with the name of the object. Supported objects: square, rectangle, oval & fish.\n\r Action: It prints the object on the OLED display.\n\r"
#define HELP_PRINT "4. Print <Shape> : \n\r Arguments: String with the name of the object. Supported objects: square, rectangle, oval & fish.\n\r Action: It prints the object on the OLED display.\n\r"
#define HELP_DRAW "5. Draw <Shape> : \n\r Arguments: String with the name of the object. Supported objects: square & rectangle.\n\r Action: It draws the object on the OLED display pixel by pixel.\n\r"
#define HELP_SHOW "6. Show <Shape> : \n\r Arguments: String with the name of the object. Supported objects: check_gif.\n\r Action: It shows the appropriate animation.\n\r"
#define HELP_CLEAR "7. Clear <None> : \n\r Arguments: None\n\r Action: Clears the OLED display.\n\r"
#define HELP_FILL "8. Fill <None> : \n\r Arguments: None\n\r Action: Switches on all the pixels on the OLED display\n\r"
#define HELP_TOGGLE "9. Toggle_pixel <X co-ordinate> <y co-ordinate> <pixel state> : \n\r Arguments: It takes the x co-ordinate, y co-ordinate and pixel state(ON/OFF).\n\r Action: It switches ON/OFF the relevent pixel on the OLED. (0,0) is at the bottom left corner. So all the pixels are in the 1st quadrant. Y goes from 0-31 and X goes from 0-127.\n\r"

#define ZERO_ASCII 0x30
#define NINE_ASCII 0x39

typedef void (*command_handler_t)(int, char *argv[]); // dcreate a funciton pointer definition

typedef struct // Contains variables relevant to commands
{
	  const char *name; // name of command
	  command_handler_t handler; /* Function pointer for the command
	  	  	  	  	  	  	  	  	  containing the relevant instructions */
	  const char *help_string; //String containing help info for the command.
} command_table_t;

static const command_table_t commands[] =  // command table containing all the relevant commands
{
		{ STR1, &name_func, HELP_AUTHOR},
		{ STR2, &help_func, HELP_HELP},
		{ STR3, &shape_func, HELP_DISPLAY},
		{ STR4, &shape_func, HELP_PRINT},
		{ STR5, &draw_func, HELP_DRAW},
		{ STR6, &gif_func, HELP_SHOW},
		{ STR7, &clear_func, HELP_CLEAR},
		{ STR8, &fill_func, HELP_FILL},
		{ STR9, &toggle_func, HELP_TOGGLE}
};


static const int num_commands = // number of commands that are supported
  sizeof(commands) / sizeof(command_table_t);

void help_func()
{
	printf("These are all the supported commands:\n\r");
	printf("All the commands are case insensitive.\n\r");
	printf("\n\r");

	for(int i =0 ; i< num_commands; i++) // Print the help string for all the commands
	{
		printf("%s\n\r", commands[i].help_string);
	}
}

void name_func()
{
	printf("%s\n\r", AUTHOR_NAME);
}

void shape_func(int argc, char* argv[])
{
	/* compare if the argument is square. If it is, display a square */
	if(!(strcasecmp(argv[1], "SQUARE")))
	{
		square_display();
	}
	/* compare if the argument is rectangle. If it is, display a rectangle */
	else if(!(strcasecmp(argv[1], "RECTANGLE")))
	{
		rectangle_display();
	}
	/* compare if the argument is oval. If it is, display an oval */
	else if(!(strcasecmp(argv[1], "OVAL")))
	{
		shape_display_image(&shape_oval[0]);
	}
	else if(!(strcasecmp(argv[1], "FISH")))
	{
		shape_display_image(&shape_fish_32[0]);
	}
	/* If the argument doesn't match anything. Send the error message. */
	else
	{
		printf("Unknown object\n\r");
		printf("Enter help for a list of supported commands and objects.\n\r");
	}
}

void draw_func(int argc, char* argv[])
{
	/* compare if the argument is square. If it is, display a square */
	if(!(strcasecmp(argv[1], "SQUARE")))
	{
		draw_square();
	}
	/* compare if the argument is rectangle. If it is, display a rectangle */
	else if(!(strcasecmp(argv[1], "RECTANGLE")))
	{
		draw_rectangle();
	}
	/* If the argument doesn't match anything. Send the error message. */
	else
	{
		printf("Unknown object\n\r");
		printf("Enter help for a list of supported commands and objects.\n\r");
	}
}

void gif_func(int argc, char* argv[])
{
	/* compare if the argument is check_gif. If it is, display the relevant animation */
	if(!(strcasecmp(argv[1], "CHECK_GIF")))
	{
		check_animation();
	}
	/* If the argument doesn't match anything. Send the error message. */
	else
	{
		printf("Unknown object\n\r");
		printf("Enter help for a list of supported commands and objects.\n\r");
	}
}

void clear_func()
{
	/* Clear the screen */
	clear_oled();
}

void fill_func()
{
	/* Fill the screen */
	fill_oled();
}

void toggle_func(int argc, char* argv[])
{
	uint8_t x = char_to_int(argv[1]);
	uint8_t y = char_to_int(argv[2]);
	uint8_t state;
	if(strcasecmp(argv[3], "ON") == 0)
	{
		state = ON;
	}
	else if(strcasecmp(argv[3], "OFF") == 0)
	{
		state = OFF;
	}
	else
	{
		state = OFF;
		printf("Invalid state\n\r");
		return;
	}

	toggle_pixel_oled(x,y,state);
}

void process_command(char *input)
{
	  char *p = input;
	  char *end;
	  int flag =0; // used to identify if a token has been completed.
	  int flag_UnknownCmd =0; // Is set if there is an unknown command in the tokens

	  for (end = input; *end != '\0'; end++) // find end of string
		;

	  char *argv[10]; // stores pointers to all the tokens
	  int argc = 0;// keeps track of the number of tokens
	  memset(argv, 0, sizeof(argv));

	  for (p = input; p < end; p++)
	  {
		  if((*p != ' ') && (*p != 0x09) && (*p != '\r'))
		  {                          // Store pointer to characters if they aren't space
			  if(flag == 0) /*Add a new pointer to argv[] only if
			  	  	  	  	  	  a character follows space or tab*/
			  {
				  argv[argc] = p;
				  argc++;
				  flag =1;
			  }

		  }
		  else if ((*p == ' ') || (*p == 0x09)|| (*p == '\r'))
		  {
			  if(flag == 1) /* If a space follows any other character,
			  	  	  	  	  	  replace space with a terminating character*/
			  {
				  *p = '\0';
			  }
			 flag = 0;
		  }
	  }

	  argv[argc] = NULL;
	  if (argc == 0)   // no command
		return;

	  for (int i=0; i < num_commands; i++) /* Check if the token matches
	  	  	  	  	  	  	  	  	  	  any of the strings for the commands */
	  {
	      if (strcasecmp(argv[0], commands[i].name) == 0)
	      {
	        commands[i].handler(argc, argv);
	        flag_UnknownCmd =1;
	        break;
	      }
	  }
	  if(flag_UnknownCmd == 0) //If token matches none of the strings
	  {
		  printf("Unknown Command: %s\n\r", argv[0]);
	  }
}
