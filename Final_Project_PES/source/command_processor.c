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


#define STR1 "Author"
#define STR2 "Help"

#define STR3 "Display"
//#define STR4 "Display Rectangle"
//#define STR5 "Display Fish"
//#define STR6 "Display Oval"

#define STR4 "Print"
//#define STR8 "Print Rectangle"
//#define STR9 "Print Fish"
//#define STR10 "Print Oval"

#define STR5 "Draw"
//#define STR12 "Draw Rectangle"

#define STR6 "Show check_gif"



#define AUTHOR_NAME "Anshul Somani"


#define HELP_AUTHOR "1. Author <None> : \n\r Arguments: None\n\r Action: It prints the name of the user\n\r"
#define HELP_HELP "2. Help <None> : \n\r Arguments: None\n\r Action: It prints information regarding all the supported commands.\n\r"
#define HELP_DISPLAY "3. Display <Shape> : \n\r Arguments: String with the name of the object. Supported objects: square, rectangle, oval & fish.\n\r Action: It prints the object on the OLED display.\n\r"
#define HELP_PRINT "4. Print <Shape> : \n\r Arguments: String with the name of the object. Supported objects: square, rectangle, oval & fish.\n\r Action: It prints the object on the OLED display.\n\r"
#define HELP_DRAW "5. Draw <Shape> : \n\r Arguments: String with the name of the object. Supported objects: square & rectangle.\n\r Action: It draws the object on the OLED display pixel by pixel.\n\r"
#define HELP_SHOW "6. Show <Shape> : \n\r Arguments: String with the name of the object. Supported objects: check_gif.\n\r Action: It shows the appropriate animation.\n\r"

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
		{ STR1, &display_name, HELP_AUTHOR},
		{ STR2, &display_help, HELP_HELP},
		{ STR3, &display_shape, HELP_DISPLAY},
		{ STR4, &display_shape, HELP_DISPLAY},
		{ STR5, &display_draw, HELP_DRAW},
		{ STR6, &display_gif, HELP_SHOW},
};


static const int num_commands = // number of commands that are supported
  sizeof(commands) / sizeof(command_table_t);

void display_help()
{
	printf("These are all the supported commands:\n\r");
	printf("All the commands are case insensitive.\n\r");
	printf("\n\r");

	for(int i =0 ; i< num_commands; i++) // Print the help string for all the commands
	{
		printf("%s\n\r", commands[i].help_string);
	}
}

void display_name()
{
	printf("%s\n\r", AUTHOR_NAME);
}

void display_shape(int argc, char* argv[])
{
	if(strcasecmp(argv[1], "SQUARE"))
	{
		square_display();
	}
	else if(strcasecmp(argv[1], "RECTANGLE"))
	{
		rectangle_display();
	}
	else if(strcasecmp(argv[1], "OVAL"))
	{
		shape_display_image(&shape_oval[0]);
	}
}

void display_draw(int argc, char* argv[])
{
	if(strcasecmp(argv[1], "SQUARE"))
	{
		draw_square();
	}
	else if(strcasecmp(argv[1], "RECTANGLE"))
	{
		draw_rectangle();
	}
}

void display_gif(int argc, char* argv[])
{
	if(strcasecmp(argv[1], "CHECK_GIF"))
	{
		check_animation();
	}
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
