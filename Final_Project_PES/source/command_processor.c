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


#define STR1 "Author"
#define STR2 "Dump"
#define STR3 "Help"
#define STR4 "Info"
#define AUTHOR_NAME "Anshul Somani"
#define HELP_AUTHOR "1. Author <None> : \n\r Arguments: None\n\r Action: It prints the name of the user\n\r"
#define HELP_DUMP "2. Dump <Address> <Length> : \n\r Arguments: Starting address and length\n\r Action: It prints the data present in the given memory addresses.\n\r The Paramters can be in either hex(0x0a) or decimal(10).\n\r The command isn't case sensitive.\n\r"
#define HELP_HELP "3. Help <None> : \n\r Arguments: None\n\r Action: It prints information regarding all the supported commands.\n\r"
#define HELP_INFO "4. Info <None> : \n\r Arguments: None\n\r Action: It prints the build version, date, machine and commit ID.\n\r"

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
		{ STR2, &hexdump, HELP_DUMP},
		{ STR3, &display_help, HELP_HELP},
		{ STR4, &display_info, HELP_INFO}
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

void display_info()
{
	printf("display_info()\n\r");
	//printf(" Version %d built on %s at %s Commit %d\n\r", VERSION_BUILD_INFO, VERSION_BUILD_MACHINE, VERSION_BUILD_DATE, VERSION_TAG);
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
