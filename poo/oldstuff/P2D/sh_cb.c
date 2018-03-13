/*****************************************************************************
​ ​*​ ​Copyright​ ​(C)​ ​2018 ​by​ Zach Farmer
​ ​*
​ ​*​ ​Redistribution,​ ​modification​ ​or​ ​use​ ​of​ ​this​ ​software​ ​in​ ​source​ ​or​ ​binary
​ ​*​ ​forms​ is permitted under the Zach Literally Could Not Care Less If You 
 * Paid Him To License and GNU GPL.
 *
 * ​Zach Farmer ​is not liable for any misuse of this material.
​ ​*
*****************************************************************************/
/**
​ ​*​ ​@file​ ​main.c
​ ​*​ ​@brief​ ​Shared Memory IPC Example
​ ​*
​ ​*​ ​This​ ​is an example of how shared memory (mmap) can be used for IPC
 * (Inter-process communication)
​ ​*
​ ​*​ ​@author​ ​Zach Farmer
​ ​*​ ​@date​ ​Mar 3 2018
​ ​*​ ​@version​ ​69
​ ​*
​ ​*/

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "msgstruct.h"
#include <pthread.h>
#include <stdarg.h>

struct cb_item
{

}
int cb_init()
{

	void* shmem = mmap(NULL, 128, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);

}

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
	uint8_t * buffer;
	size_t head;
	size_t tail;
	size_t size; //of the buffer
} circular_buf_t;

/**
* Important Usage Note: This library reserves one spare byte for queue-full detection
* Otherwise, corner cases and detecting difference between full/empty is hard.
* You are not seeing an accidental off-by-one.
*/

int circular_buf_reset(circular_buf_t * cbuf);
int circular_buf_put(circular_buf_t * cbuf, uint8_t data);
//TODO: int circular_buf_put_range(circular_buf_t cbuf, uint8_t * data, size_t len);
int circular_buf_get(circular_buf_t * cbuf, uint8_t * data);
//TODO: int circular_buf_get_range(circular_buf_t cbuf, uint8_t *data, size_t len);
bool circular_buf_empty(circular_buf_t cbuf);
bool circular_buf_full(circular_buf_t cbuf);

int circular_buf_reset(circular_buf_t * cbuf)
{
    int r = -1;

    if(cbuf)
    {
        cbuf->head = 0;
        cbuf->tail = 0;
        r = 0;
    }

    return r;
}

int circular_buf_put(circular_buf_t * cbuf, uint8_t data)
{
    int r = -1;

    if(cbuf)
    {
        cbuf->buffer[cbuf->head] = data;
        cbuf->head = (cbuf->head + 1) % cbuf->size;

        if(cbuf->head == cbuf->tail)
        {
            cbuf->tail = (cbuf->tail + 1) % cbuf->size;
        }

        r = 0;
    }

    return r;
}

int circular_buf_get(circular_buf_t * cbuf, uint8_t * data)
{
    int r = -1;

    if(cbuf && data && !circular_buf_empty(*cbuf))
    {
        *data = cbuf->buffer[cbuf->tail];
        cbuf->tail = (cbuf->tail + 1) % cbuf->size;

        r = 0;
    }

    return r;
}

bool circular_buf_empty(circular_buf_t cbuf)
{
	// We define empty as head == tail
    return (cbuf.head == cbuf.tail);
}

bool circular_buf_full(circular_buf_t cbuf)
{
	// We determine "full" case by head being one position behind the tail
	// Note that this means we are wasting one space in the buffer!
	// Instead, you could have an "empty" flag and determine buffer full that way
    return ((cbuf.head + 1) % cbuf.size) == cbuf.tail;
}

int main(void)
{
	circular_buf_t cbuf;
	cbuf.size = 10;

	printf("\n=== C Circular Buffer Check ===\n");

	circular_buf_reset(&cbuf); //set head/tail to 0

	cbuf.buffer = malloc(cbuf.size);

	printf("Full: %d, empty: %d\n", circular_buf_full(cbuf), circular_buf_empty(cbuf));

	printf("Adding 9 values\n");
	for(uint8_t i = 0; i < cbuf.size - 1; i++)
	{
		circular_buf_put(&cbuf, i);
	}

	printf("Full: %d, empty: %d\n", circular_buf_full(cbuf), circular_buf_empty(cbuf));

	printf("Reading back values: ");
	while(!circular_buf_empty(cbuf))
	{
		uint8_t data;
		circular_buf_get(&cbuf, &data);
		printf("%u ", data);
	}
	printf("\n");

	printf("Adding 15 values\n");
	for(uint8_t i = 0; i < cbuf.size + 5; i++)
	{
		circular_buf_put(&cbuf, i);
	}

	printf("Full: %d, empty: %d\n", circular_buf_full(cbuf), circular_buf_empty(cbuf));

	printf("Reading back values: ");
	while(!circular_buf_empty(cbuf))
	{
		uint8_t data;
		circular_buf_get(&cbuf, &data);
		printf("%u ", data);
	}
	printf("\n");

	free(cbuf.buffer);

	return 0;
}