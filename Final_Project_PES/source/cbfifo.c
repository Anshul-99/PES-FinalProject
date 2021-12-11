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
​ * ​ ​ @file​ ​ cbfifo.c
​ * ​ ​ @brief​ ​ Implementation of circular buffer using static memory allocation
​ *
​ * ​ ​ This file implements an array based circular buffer and provides functions to enqueue, dequeue, check length and
 *   capacity of the queue
​ *
​ * ​ ​ @author​ ​ Anshul Somani
​ * ​ ​ @date​ ​ November 3 2021
​ * ​ ​ @version​ ​ 2.0
​ *
​ */

#include "MKL25Z4.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "cbfifo.h"
#include "uart.h"

typedef struct c_buffer {    /* Circular buffer struct*/
	uint8_t array[SIZE_CBUF]; /* Circular buffer*/
	size_t length; 				// Length of buffer
	uint8_t index_write; // index from where data needs to be enqueued
	uint8_t index_read; // index from where data needs to be dequeued
} c_buffer; // keep in .c file

c_buffer FIFOS[num_fifo] ={
		{ {0}, 0, 0, 0 }, // tx fifo statically initialized

		{ {0}, 0, 0, 0 } // rx fifio statically initialized
};

size_t cbfifo_enqueue(fifo_handle_t handle,  void *input_buf, size_t nbyte)
{
	c_buffer *circ_buff = &FIFOS[handle]; // Store pointer to tx_fifo ro rx_fifo

	if(nbyte<0) // Error condition to check if the bytes to be enqueued is non-zero
	{
		return -1; /* nybtes<0 */
	}
	if(nbyte > SIZE_CBUF) /* Error condition to check if the bytes to be enqueued
							is greater than the capacity of buffer */
	{
		return -1;
	}

	uint8_t *ipBufPtr = (uint8_t *)input_buf;
	uint32_t mask_state;
	mask_state = __get_PRIMASK(); // Store the primask value of the interrupts
	__disable_irq(); // disable interrupts

	uint8_t bytes =0;
	for(int i =0; i<nbyte; i++)
	{
		if(cbfifo_capacity() == cbfifo_length(handle)) // Check if buffer is full
		{
			break;
		}

		if(&(circ_buff->array[circ_buff->index_write]) > &(circ_buff->array[SIZE_CBUF-1]))
			//Check if index is at the end of the array. If yes, send it to the start.
		{
			circ_buff->index_write =0;
		}

		circ_buff->array[circ_buff->index_write] = ipBufPtr[i]; // write byte to the buffer array
		bytes++; // increment bytes enqueued
		circ_buff->index_write++;
		circ_buff->length++;
	}

	__set_PRIMASK(mask_state); // set the primask register with the earlier value to enable interrupts

	return bytes;

}

size_t cbfifo_dequeue(fifo_handle_t handle, void *input_buf, size_t nbyte)
{
	  c_buffer *circ_buff = &FIFOS[handle];

	if(nbyte > SIZE_CBUF) /* Error condition to check if the bytes to be enqueued
							is greater than the capacity of buffer */
	{
		return -1;
	}
	if(nbyte < 0) /* Error condition to check if the bytes to be dequeued is non-zero */
	{
		return -2;
	}
	if(nbyte == 0)
	{
		return 0;
	}

	uint8_t *ipBufPtr = (uint8_t *)input_buf;

	uint32_t mask_state;
	mask_state = __get_PRIMASK(); // Store the primask value of the interrupts
	__disable_irq(); // disable interrupts

	uint8_t bytes =0;

	for(uint8_t i=0; i<nbyte; i++)
	{
		if((cbfifo_length(handle)==0)) /* Error condition to check if the buffer is empty*/
		{
			break;
		}

		if(&(circ_buff->array[circ_buff->index_read]) > &(circ_buff->array[SIZE_CBUF-1]))
			//Check if index is at the end of the array. If yes, send it to the start.
		{
			circ_buff->index_read = 0;

		}

		ipBufPtr[i] = circ_buff->array[circ_buff->index_read];  // read byte from the buffer array
		bytes++; // increment bytes dequeued
		circ_buff->index_read++;
		circ_buff->length--;
	}
	__set_PRIMASK(mask_state); // set the primask register with the earlier value to enable interrupts

	return bytes;

}

size_t cbfifo_length(fifo_handle_t handle)
{
	  return FIFOS[handle].length;
}

size_t cbfifo_capacity()
{
	return SIZE_CBUF;
}

uint8_t test_cbfifo()
{
	char *str ="To be, or not to be: that is the question:\n"
	    "Whether 'tis nobler in the mind to suffer\n"
	    "The slings and arrows of outrageous fortune,\n"
	    "Or to take arms against a sea of troubles,\n"
	    "And by opposing end them? To die, to sleep--\n"
	    "No more--and by a sleep to say we end\n"
	    "The heart-ache and the thousand natural shocks\n"
	    "That flesh is heir to, 'tis a consummation\n"
	    "Devoutly to be wish'd. To die, to sleep;\n"
	    "To sleep: perchance to dream: ay, there's the rub;\n"
	    "For in that sleep of death what dreams may come\n"
	    "When we have shuffled off this mortal coil,\n"
	    "Must give us pause.";

	  char buf[1024];
	  const int cap = cbfifo_capacity();

	  // asserts in following 2 lines -- this is not testing the student,
	  // it's validating that the test is correct
	  assert(strlen(str) >= cap*2);
	  assert(sizeof(buf) > cap);
	  assert(cap == 256 || cap == 255);

	  assert(cbfifo_length(rx_fifo) == 0);
	  assert(cbfifo_dequeue(rx_fifo, buf, cap) == 0);
	  assert(cbfifo_dequeue(rx_fifo, buf, 1) == 0);

	  assert(cbfifo_length(tx_fifo) == 0);
	  assert(cbfifo_dequeue(tx_fifo, buf, cap) == 0);
	  assert(cbfifo_dequeue(tx_fifo, buf, 1) == 0);


	  // enqueue 10 bytes, then dequeue same amt
	  assert(cbfifo_enqueue(rx_fifo, str, 10) == 10);
	  assert(cbfifo_length(rx_fifo) == 10);
	  assert(cbfifo_dequeue(rx_fifo, buf, 10) == 10);
	  assert(strncmp(buf, str, 10) == 0);
	  assert(cbfifo_length(rx_fifo) == 0);

	  // enqueue 10 bytes, then dequeue same amt
	  assert(cbfifo_enqueue(tx_fifo, str, 10) == 10);
	  assert(cbfifo_length(tx_fifo) == 10);
	  assert(cbfifo_dequeue(tx_fifo, buf, 10) == 10);
	  assert(strncmp(buf, str, 10) == 0);
	  assert(cbfifo_length(tx_fifo) == 0);


	  // enqueue 20 bytes;  dequeue 5, then another 20
	  assert(cbfifo_enqueue(rx_fifo, str, 20) == 20);
	  assert(cbfifo_length(rx_fifo) == 20);
	  assert(cbfifo_dequeue(rx_fifo, buf, 5) == 5);
	  assert(cbfifo_length(rx_fifo) == 15);
	  assert(cbfifo_dequeue(rx_fifo, buf+5, 20) == 15);
	  assert(cbfifo_length(rx_fifo) == 0);
	  assert(strncmp(buf, str, 20) == 0);

	  // enqueue 20 bytes;  dequeue 5, then another 20
	  assert(cbfifo_enqueue(tx_fifo, str, 20) == 20);
	  assert(cbfifo_length(tx_fifo) == 20);
	  assert(cbfifo_dequeue(tx_fifo, buf, 5) == 5);
	  assert(cbfifo_length(tx_fifo) == 15);
	  assert(cbfifo_dequeue(tx_fifo, buf+5, 20) == 15);
	  assert(cbfifo_length(tx_fifo) == 0);
	  assert(strncmp(buf, str, 20) == 0);


	  // fill buffer and then read it back out
	  assert(cbfifo_enqueue(rx_fifo, str, cap) == cap);
	  assert(cbfifo_length(rx_fifo) == cap);
	  assert(cbfifo_enqueue(rx_fifo, str, 1) == 0);
	  assert(cbfifo_dequeue(rx_fifo, buf, cap) == cap);
	  assert(cbfifo_length(rx_fifo) == 0);
	  assert(strncmp(buf, str, cap) == 0);

	  // fill buffer and then read it back out
	  assert(cbfifo_enqueue(tx_fifo, str, cap) == cap);
	  assert(cbfifo_length(tx_fifo) == cap);
	  assert(cbfifo_enqueue(tx_fifo, str, 1) == 0);
	  assert(cbfifo_dequeue(tx_fifo, buf, cap) == cap);
	  assert(cbfifo_length(tx_fifo) == 0);
	  assert(strncmp(buf, str, cap) == 0);


	  // Add 20 bytes and pull out 18
	  assert(cbfifo_enqueue(rx_fifo, str, 20) == 20);
	  assert(cbfifo_length(rx_fifo) == 20);
	  assert(cbfifo_dequeue(rx_fifo, buf, 18) == 18);
	  assert(cbfifo_length(rx_fifo) == 2);
	  assert(strncmp(buf, str, 18) == 0);

	  // Add 20 bytes and pull out 18
	  assert(cbfifo_enqueue(tx_fifo, str, 20) == 20);
	  assert(cbfifo_length(tx_fifo) == 20);
	  assert(cbfifo_dequeue(tx_fifo, buf, 18) == 18);
	  assert(cbfifo_length(tx_fifo) == 2);
	  assert(strncmp(buf, str, 18) == 0);


	  // Now add a bunch of data in 4 chunks
	  int chunk_size = (cap-2) / 4;
	  for (int i=0; i<4; i++) {
	    assert(cbfifo_enqueue(rx_fifo, str+i*chunk_size, chunk_size) == chunk_size);
	    assert(cbfifo_length(rx_fifo) == (i+1)*chunk_size + 2);
	  }
	  assert(cbfifo_length(rx_fifo) == 4*chunk_size + 2);

	  // Take out the 2 remaining bytes from above
	  assert(cbfifo_dequeue(rx_fifo, buf, 2) == 2);
	  assert(strncmp(buf, str+18, 2) == 0);

	  // now read those chunks out a byte at a time
	  for (int i=0; i<chunk_size*4; i++) {
	    assert(cbfifo_dequeue(rx_fifo, buf+i, 1) == 1);
	    assert(cbfifo_length(rx_fifo) == chunk_size*4 - i - 1);
	  }
	  assert(strncmp(buf, str, chunk_size*4) == 0);

	  chunk_size = (cap-2) / 4;
	  for (int i=0; i<4; i++) {
		assert(cbfifo_enqueue(tx_fifo, str+i*chunk_size, chunk_size) == chunk_size);
		assert(cbfifo_length(tx_fifo) == (i+1)*chunk_size + 2);
	  }
	  assert(cbfifo_length(tx_fifo) == 4*chunk_size + 2);

	  // Take out the 2 remaining bytes from above
	  assert(cbfifo_dequeue(tx_fifo, buf, 2) == 2);
	  assert(strncmp(buf, str+18, 2) == 0);

	  // now read those chunks out a byte at a time
	  for (int i=0; i<chunk_size*4; i++) {
		assert(cbfifo_dequeue(tx_fifo, buf+i, 1) == 1);
		assert(cbfifo_length(tx_fifo) == chunk_size*4 - i - 1);
	  }
	  assert(strncmp(buf, str, chunk_size*4) == 0);


	  // write more than capacity
	  assert(cbfifo_enqueue(rx_fifo, str, 65) == 65);
	  assert(cbfifo_enqueue(rx_fifo, str+65, cap) == cap-65);
	  assert(cbfifo_length(rx_fifo) == cap);
	  assert(cbfifo_dequeue(rx_fifo, buf, cap) == cap);
	  assert(cbfifo_length(rx_fifo) == 0);
	  assert(strncmp(buf, str, cap) == 0);

	  // write more than capacity
	  assert(cbfifo_enqueue(tx_fifo, str, 65) == 65);
	  assert(cbfifo_enqueue(tx_fifo, str+65, cap) == cap-65);
	  assert(cbfifo_length(tx_fifo) == cap);
	  assert(cbfifo_dequeue(tx_fifo, buf, cap) == cap);
	  assert(cbfifo_length(tx_fifo) == 0);
	  assert(strncmp(buf, str, cap) == 0);


	  // write zero bytes
	  assert(cbfifo_enqueue(rx_fifo, str, 0) == 0);
	  assert(cbfifo_length(rx_fifo) == 0);

	  // write zero bytes
	  assert(cbfifo_enqueue(tx_fifo, str, 0) == 0);
	  assert(cbfifo_length(tx_fifo) == 0);


	  // Exercise the following conditions:
	  //    enqueue when read < write:
	  //        bytes < CAP-write  (1)
	  //        bytes exactly CAP-write  (2)
	  //        bytes > CAP-write but < space available (3)
	  //        bytes exactly the space available (4)
	  //        bytes > space available (5)
	  assert(cbfifo_enqueue(rx_fifo, str, 32) == 32);  // advance so that read < write
	  assert(cbfifo_length(rx_fifo) == 32);
	  assert(cbfifo_dequeue(rx_fifo, buf, 16) == 16);
	  assert(cbfifo_length(rx_fifo) == 16);
	  assert(strncmp(buf, str, 16) == 0);

	  assert(cbfifo_enqueue(tx_fifo, str, 32) == 32);  // advance so that read < write
	  assert(cbfifo_length(tx_fifo) == 32);
	  assert(cbfifo_dequeue(tx_fifo, buf, 16) == 16);
	  assert(cbfifo_length(tx_fifo) == 16);
	  assert(strncmp(buf, str, 16) == 0);


	  assert(cbfifo_enqueue(rx_fifo, str+32, 32) == 32);  // (1)
	  assert(cbfifo_length(rx_fifo) == 48);
	  assert(cbfifo_enqueue(rx_fifo, str+64, cap-64) == cap-64);  // (2)
	  assert(cbfifo_length(rx_fifo) == cap-16);
	  assert(cbfifo_dequeue(rx_fifo, buf+16, cap-16) == cap-16);
	  assert(strncmp(buf, str, cap) == 0);

	  assert(cbfifo_enqueue(tx_fifo, str+32, 32) == 32);  // (1)
	  assert(cbfifo_length(tx_fifo) == 48);
	  assert(cbfifo_enqueue(tx_fifo, str+64, cap-64) == cap-64);  // (2)
	  assert(cbfifo_length(tx_fifo) == cap-16);
	  assert(cbfifo_dequeue(tx_fifo, buf+16, cap-16) == cap-16);
	  assert(strncmp(buf, str, cap) == 0);


	  assert(cbfifo_enqueue(rx_fifo, str, 32) == 32);  // advance so that read < write
	  assert(cbfifo_length(rx_fifo) == 32);
	  assert(cbfifo_dequeue(rx_fifo, buf, 16) == 16);
	  assert(cbfifo_length(rx_fifo) == 16);
	  assert(strncmp(buf, str, 16) == 0);

	  assert(cbfifo_enqueue(tx_fifo, str, 32) == 32);  // advance so that read < write
	  assert(cbfifo_length(tx_fifo) == 32);
	  assert(cbfifo_dequeue(tx_fifo, buf, 16) == 16);
	  assert(cbfifo_length(tx_fifo) == 16);
	  assert(strncmp(buf, str, 16) == 0);


	  assert(cbfifo_enqueue(rx_fifo, str+32, cap-20) == cap-20);  // (3)
	  assert(cbfifo_length(rx_fifo) == cap-4);
	  assert(cbfifo_dequeue(rx_fifo, buf, cap-8) == cap-8);
	  assert(strncmp(buf, str+16, cap-8) == 0);
	  assert(cbfifo_length(rx_fifo) == 4);
	  assert(cbfifo_dequeue(rx_fifo, buf, 8) == 4);
	  assert(strncmp(buf, str+16+cap-8, 4) == 0);
	  assert(cbfifo_length(rx_fifo) == 0);

	  assert(cbfifo_enqueue(tx_fifo, str+32, cap-20) == cap-20);  // (3)
	  assert(cbfifo_length(tx_fifo) == cap-4);
	  assert(cbfifo_dequeue(tx_fifo, buf, cap-8) == cap-8);
	  assert(strncmp(buf, str+16, cap-8) == 0);
	  assert(cbfifo_length(tx_fifo) == 4);
	  assert(cbfifo_dequeue(tx_fifo, buf, 8) == 4);
	  assert(strncmp(buf, str+16+cap-8, 4) == 0);
	  assert(cbfifo_length(tx_fifo) == 0);


	  assert(cbfifo_enqueue(rx_fifo, str, 49) == 49);  // advance so that read < write
	  assert(cbfifo_length(rx_fifo) == 49);
	  assert(cbfifo_dequeue(rx_fifo, buf, 16) == 16);
	  assert(cbfifo_length(rx_fifo) == 33);
	  assert(strncmp(buf, str, 16) == 0);

	  assert(cbfifo_enqueue(tx_fifo, str, 49) == 49);  // advance so that read < write
	  assert(cbfifo_length(tx_fifo) == 49);
	  assert(cbfifo_dequeue(tx_fifo, buf, 16) == 16);
	  assert(cbfifo_length(tx_fifo) == 33);
	  assert(strncmp(buf, str, 16) == 0);


	  assert(cbfifo_enqueue(rx_fifo, str+49, cap-33) == cap-33);  // (4)
	  assert(cbfifo_length(rx_fifo) == cap);
	  assert(cbfifo_dequeue(rx_fifo, buf, cap) == cap);
	  assert(cbfifo_length(rx_fifo) == 0);
	  assert(strncmp(buf, str+16, cap) == 0);

	  assert(cbfifo_enqueue(tx_fifo, str+49, cap-33) == cap-33);  // (4)
	  assert(cbfifo_length(tx_fifo) == cap);
	  assert(cbfifo_dequeue(tx_fifo, buf, cap) == cap);
	  assert(cbfifo_length(tx_fifo) == 0);
	  assert(strncmp(buf, str+16, cap) == 0);


	  assert(cbfifo_enqueue(rx_fifo, str, 32) == 32);  // advance so that read < write
	  assert(cbfifo_length(rx_fifo) == 32);
	  assert(cbfifo_dequeue(rx_fifo, buf, 16) == 16);
	  assert(cbfifo_length(rx_fifo) == 16);
	  assert(strncmp(buf, str, 16) == 0);

	  assert(cbfifo_enqueue(tx_fifo, str, 32) == 32);  // advance so that read < write
	  assert(cbfifo_length(tx_fifo) == 32);
	  assert(cbfifo_dequeue(tx_fifo, buf, 16) == 16);
	  assert(cbfifo_length(tx_fifo) == 16);
	  assert(strncmp(buf, str, 16) == 0);


	  assert(cbfifo_enqueue(rx_fifo, str+32, cap) == cap-16);  // (5)
	  assert(cbfifo_dequeue(rx_fifo, buf, 1) == 1);
	  assert(cbfifo_length(rx_fifo) == cap-1);
	  assert(cbfifo_dequeue(rx_fifo, buf+1, cap-1) == cap-1);
	  assert(cbfifo_length(rx_fifo) == 0);
	  assert(strncmp(buf, str+16, cap) == 0);

	  assert(cbfifo_enqueue(tx_fifo, str+32, cap) == cap-16);  // (5)
	  assert(cbfifo_dequeue(tx_fifo, buf, 1) == 1);
	  assert(cbfifo_length(tx_fifo) == cap-1);
	  assert(cbfifo_dequeue(tx_fifo, buf+1, cap-1) == cap-1);
	  assert(cbfifo_length(tx_fifo) == 0);
	  assert(strncmp(buf, str+16, cap) == 0);


	  //    enqueue when write < read:
	  //        bytes < read-write (6)
	  //        bytes exactly read-write (= the space available) (7)
	  //        bytes > space available (8)

	  int wpos=0, rpos=0;
	  assert(cbfifo_enqueue(rx_fifo, str, cap-4) == cap-4);
	  wpos += cap-4;
	  assert(cbfifo_length(rx_fifo) == cap-4);
	  assert(cbfifo_dequeue(rx_fifo, buf, 32) == 32);
	  rpos += 32;
	  assert(cbfifo_length(rx_fifo) == cap-36);
	  assert(strncmp(buf, str, 32) == 0);
	  assert(cbfifo_enqueue(rx_fifo, str+wpos, 12) == 12);
	  wpos += 12;
	  assert(cbfifo_length(rx_fifo) == cap-24);

	  assert(cbfifo_enqueue(rx_fifo, str+wpos, 16) == 16);  // (6)
	  assert(cbfifo_length(rx_fifo) == cap-8);
	  assert(cbfifo_dequeue(rx_fifo, buf, cap) == cap-8);
	  assert(cbfifo_length(rx_fifo) == 0);
	  assert(strncmp(buf, str+rpos, cap-8) == 0);

	  wpos =0, rpos =0;
	  assert(cbfifo_enqueue(tx_fifo, str, cap-4) == cap-4);
	  wpos += cap-4;
	  assert(cbfifo_length(tx_fifo) == cap-4);
	  assert(cbfifo_dequeue(tx_fifo, buf, 32) == 32);
	  rpos += 32;
	  assert(cbfifo_length(tx_fifo) == cap-36);
	  assert(strncmp(buf, str, 32) == 0);
	  assert(cbfifo_enqueue(tx_fifo, str+wpos, 12) == 12);
	  wpos += 12;
	  assert(cbfifo_length(tx_fifo) == cap-24);

	  assert(cbfifo_enqueue(tx_fifo, str+wpos, 16) == 16);  // (6)
	  assert(cbfifo_length(tx_fifo) == cap-8);
	  assert(cbfifo_dequeue(tx_fifo, buf, cap) == cap-8);
	  assert(cbfifo_length(tx_fifo) == 0);
	  assert(strncmp(buf, str+rpos, cap-8) == 0);


	  // reset
	  wpos=0;
	  rpos=0;
	  assert(cbfifo_enqueue(rx_fifo, str, cap-4) == cap-4);
	  wpos += cap-4;
	  assert(cbfifo_length(rx_fifo) == cap-4);
	  assert(cbfifo_dequeue(rx_fifo, buf, 32) == 32);
	  rpos += 32;
	  assert(cbfifo_length(rx_fifo) == cap-36);
	  assert(strncmp(buf, str, 32) == 0);
	  assert(cbfifo_enqueue(rx_fifo, str+wpos, 12) == 12);
	  wpos += 12;
	  assert(cbfifo_length(rx_fifo) == cap-24);

	  assert(cbfifo_enqueue(rx_fifo, str+wpos, 24) == 24);  // (7)
	  assert(cbfifo_length(rx_fifo) == cap);
	  assert(cbfifo_dequeue(rx_fifo, buf, cap) == cap);
	  assert(cbfifo_length(rx_fifo) == 0);
	  assert(strncmp(buf, str+rpos, cap) == 0);

	  // reset
	  wpos=0;
	  rpos=0;
	  assert(cbfifo_enqueue(tx_fifo, str, cap-4) == cap-4);
	  wpos += cap-4;
	  assert(cbfifo_length(tx_fifo) == cap-4);
	  assert(cbfifo_dequeue(tx_fifo, buf, 32) == 32);
	  rpos += 32;
	  assert(cbfifo_length(tx_fifo) == cap-36);
	  assert(strncmp(buf, str, 32) == 0);
	  assert(cbfifo_enqueue(tx_fifo, str+wpos, 12) == 12);
	  wpos += 12;
	  assert(cbfifo_length(tx_fifo) == cap-24);

	  assert(cbfifo_enqueue(tx_fifo, str+wpos, 24) == 24);  // (7)
	  assert(cbfifo_length(tx_fifo) == cap);
	  assert(cbfifo_dequeue(tx_fifo, buf, cap) == cap);
	  assert(cbfifo_length(tx_fifo) == 0);
	  assert(strncmp(buf, str+rpos, cap) == 0);


	  // reset
	  wpos=0;
	  rpos=0;
	  assert(cbfifo_enqueue(rx_fifo, str, cap-4) == cap-4);
	  wpos += cap-4;
	  assert(cbfifo_length(rx_fifo) == cap-4);
	  assert(cbfifo_dequeue(rx_fifo, buf, 32) == 32);
	  rpos += 32;
	  assert(cbfifo_length(rx_fifo) == cap-36);
	  assert(strncmp(buf, str, 32) == 0);
	  assert(cbfifo_enqueue(rx_fifo, str+wpos, 12) == 12);
	  wpos += 12;
	  assert(cbfifo_length(rx_fifo) == cap-24);

	  assert(cbfifo_enqueue(rx_fifo, str+wpos, 64) == 24);  // (8)
	  assert(cbfifo_length(rx_fifo) == cap);
	  assert(cbfifo_dequeue(rx_fifo, buf, cap) == cap);
	  assert(cbfifo_length(rx_fifo) == 0);
	  assert(strncmp(buf, str+rpos, cap) == 0);

	  // reset
	  wpos=0;
	  rpos=0;
	  assert(cbfifo_enqueue(tx_fifo, str, cap-4) == cap-4);
	  wpos += cap-4;
	  assert(cbfifo_length(tx_fifo) == cap-4);
	  assert(cbfifo_dequeue(tx_fifo, buf, 32) == 32);
	  rpos += 32;
	  assert(cbfifo_length(tx_fifo) == cap-36);
	  assert(strncmp(buf, str, 32) == 0);
	  assert(cbfifo_enqueue(tx_fifo, str+wpos, 12) == 12);
	  wpos += 12;
	  assert(cbfifo_length(tx_fifo) == cap-24);

	  assert(cbfifo_enqueue(tx_fifo, str+wpos, 64) == 24);  // (8)
	  assert(cbfifo_length(tx_fifo) == cap);
	  assert(cbfifo_dequeue(tx_fifo, buf, cap) == cap);
	  assert(cbfifo_length(tx_fifo) == 0);
	  assert(strncmp(buf, str+rpos, cap) == 0);


	  printf("%s: passed all test cases\n\r", __FUNCTION__);

	  return 1;
}




