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
​ * ​ ​ @file​ ​ uart.h
​ * ​ ​ @brief​ ​ Implementation of uart0 on kl25z
​ *
​ * ​ ​ This file initializes and uses uart0 on kl25z.
​ *
​ * ​ ​ @author​ ​ Anshul Somani
​ * ​ ​ @date​ ​ November 3 2021
​ * ​ ​ @version​ ​ 1.0
​ *
​ */
#ifndef UART_H_
#define UART_H_

/*
 * Initializes UART0 peripheral on KL25Z to operate at,
 * 38400 baud rate, 2 stop bits, no parity.
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   Nothing
 */

void init_uart();

/*
 * Uses getchar() and putchar() to mimic gets().
 * It takes a string input similarly to gets().
 *
 * Parameters:
 *   char* input: String where the input from the user
 *   is stored.
 *
 * Returns:
 *   char* : Returns a String with the user input.
 */

char* copy_gets(char* input);

#endif /* UART_H_ */
