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
​ * ​ ​ @file​ ​ uart.c
​ * ​ ​ @brief​ ​ Implementation of uart0 on kl25z
​ *
​ * ​ ​ This file initializes and uses uart0 on kl25z.
​ *
​ * ​ ​ @author​ ​ Anshul Somani
​ * ​ ​ @date​ ​ November 3 2021
​ * ​ ​ @version​ ​ 1.0
​ *
​ */

#include "MKL25Z4.h"
#include "sysclock.h"

#include <string.h>
#include <stdio.h>

#include "cbfifo.h"
#include "uart.h"

#define UART_OVERSAMPLE_RATE 16
#define BAUD_VAL 39
#define FIFO_BUFFER_LEN 255
#define BACKSPACE_ASCII 0x08
#define DATA_BITS 0


void init_uart()
{
	SIM->SCGC4 |= (SIM_SCGC4_UART0(1)); // Enable Clock to UART0
	SIM->SCGC5 |= (SIM_SCGC5_PORTA(1)); // Enable Clock to PORTA

	UART0->C2 &= (UART_C2_TE(0))| // Disable Transmitter
						(UART_C2_RE(0)); // Disable Receiver

	SIM->SOPT2 |= (SIM_SOPT2_UART0SRC(1))| //Use MCGFLLCLK or MCGPLLCLK/2 as clock source for UART0 Receive and Transmit
					(SIM_SOPT2_PLLFLLSEL(0)); //Use MCGFLLCLK as clock source

	SIM->SOPT5 &= (SIM_SOPT5_UART0TXSRC(0)) | //Use UART0_Tx pin for transmission
					(SIM_SOPT5_UART0RXSRC(0)); // Use UART0_Rx pin for receiving data



	PORTA->PCR[1] |= PORT_PCR_MUX(2); // Configure GPIO pin (PTA1) as UART_Rx
	PORTA->PCR[2] |= PORT_PCR_MUX(2); // Configure GPIO pin (PTA2) as UART_Tx

	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx



	UART0->BDH |= UART_BDH_SBNS(1); //2 stop bits

	/* baud rate = (system clk)/(( OSR+1)*BR)
	 *
	 * BR = (system clk)/(( OSR+1)*baud rate)
	 *    =  24 MHz / 16*38400
	 *    = 39.0652
	 *    ~39
	 */

	UART0->BDH &= UART_BDH_SBR(0); // clear the register.
	UART0->BDL &= UART_BDL_SBR(0);
	UART0->BDL |= UART_BDL_SBR(BAUD_VAL); // setting baud rate to 38400

	UART0->C1 &= (UART_C1_LOOPS(0))| // Disable internal hardware looping of Tx/Rx
					(UART_C1_M(DATA_BITS))| // COnfigure UART0 for 8 bit data
					(UART_C1_PE(0)); // Disable Parity

	UART0->S1 |= (UART0_S1_OR(1))|  //Clear overrun flag
					(UART0_S1_NF(1))| //clear noise flag
					(UART0_S1_FE(1))| //clear frame error flag
					(UART0_S1_PF(1)); //clear parity error flag

	UART0->S2 &= (UART_S2_RXINV(0)); //Do not inverse the received data

	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);	// Set over-sampling ratio to 16

	UART0->C3 &= (UART0_C3_TXINV(0))| //Do not invert transmit data
					(UART0_C3_ORIE(0))| // Disable interrupt for Overrun flag
					(UART0_C3_NEIE(0))| // Disable interrupt for Noise flag
					(UART0_C3_FEIE(0))| // Disable interrupt for Frame Error flag
					(UART0_C3_PEIE(0)); // Disable interrupt for Parity Error flag



	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1); // Enable UART receiver and transmitter

	//Create Tx/Rx buffers.

	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	UART0->S1 &= ~UART0_S1_RDRF_MASK; // Clear the UART RDRF flag

	// Enable receive interrupts and transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);
}

/*
 * Interrupt Handler for UART0.
 * It enqueues/dequeues data to rx/tx buffers
 * when they are received or need to be transmitted via uart.
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   Nothing
 */

void UART0_IRQHandler(void)
{
	uint8_t ch;
	uint8_t ch2;

	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK | // Check if error flags triggered the interrupt
		UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
			// clear the error flags
			UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;
			// read the data register to clear RDRF
			ch = UART0->D;
			//cbfifo_enqueue(rx, &ch, 1);
	}

	if (UART0->S1 & UART0_S1_RDRF_MASK) // check if the RDRF flag triggered the interrupt
	{
			ch = UART0->D; // Read the received character from the register
			cbfifo_enqueue(rx_fifo, &ch, 1); // Enqueue the received character to the rx fifo
	}

	if ( (UART0->C2 & UART0_C2_TIE_MASK) &&  // Check if the Transmit Interrupt Enable flag is set
			(UART0->S1 & UART0_S1_TDRE_MASK)) // check if the TDRE flag triggered the interrupt
	{
		if(cbfifo_dequeue(tx_fifo, &ch2, 1)==1) // Dequeue 1 byte from tx fifo
		{
			UART0->D = ch2; //If  byte is succesfully dequeued, write it to the register.
		}
		else
		{
			UART0->C2 &= ~UART0_C2_TIE_MASK; /* If byte is not succesfully dequeued,
												disable the transmitter*/
		}
	}
}

/*
 * Enqueues data to tx buffer.
 * It enqueues data received from printf() and puts()
 * to tx buffer to then send to the serial terminal via uart.
 *
 * Parameters:
 *   int handle:
 *   char* buf: String to be sent to serial terminal
 *   int size: size of buf
 *
 * Returns:
 *   returns 0 is the buffer was enqueued without any errors.
 *   returns -1 if there was a problem in enqueueing data on the buffer.
 */

int __sys_write(int handle, char *buf, int size)
{
	for(uint32_t i =0; i<size; i++)
	{
		 while(cbfifo_length(tx_fifo) == FIFO_BUFFER_LEN) /* It tx fifo is full,
		 	 	 	 	 	 	 	 	 	 	 	 	 	 wait for space to clear up*/
		 {
			 ;
		 }
		 if(1 != cbfifo_enqueue(tx_fifo, buf+i, 1)) /* Enqueue 1 byte onto tx fifo*/
		 {
				 return -1;
		 }
	}
	UART0->C2 |= UART0_C2_TIE(1); /* Enable Transmit Interrupt Enable flag
									to trigger interrupt when TDRE flag is set */
	return 0;
}

/*
 * Dequeues data from rx buffer.
 * It dequeues data received from serial terminal and sends it
 * to getchar().
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   uint8_t value: The character receivec from the serial terminal.
 */

int __sys_readc(void)
{
	uint8_t value;
	while(cbfifo_length(rx_fifo) == 0); /* Check if rx fifo is empty,
											if true wait for it to fill*/

	cbfifo_dequeue(rx_fifo, &value, 1); // dequeue 1 byte from rx fifo
	return value;
}

char* copy_gets(char* input)
{
	char ch;
	int i = 0;
	do
	{
		ch = getchar(); // Read a character from serial terminal
		if(ch == BACKSPACE_ASCII) //Check if the character is backspace
		{
			putchar('\b'); // Display the backspace character on the terminal
			putchar(' '); /* Clear the previously entered character
								that was to be removed by backspace*/
			putchar(ch); //Add another backspace to go to the position before the erased character
			i--;
			continue;
		}
		putchar(ch);
		input[i] = ch; // Store the received character on the string.
		i++;

	}while(ch != '\r');
	printf("\n\r");
	return input;
}
