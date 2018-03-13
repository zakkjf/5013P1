#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef SLL_H_INCLUDED
#define SLL_H_INCLUDED

#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/shm.h>

typedef struct node
{
    int val;
    struct node * next;
    struct node * prev;
} node;

int get_num_elements();

int init_sll(char* name);

void *  my_malloc(int size);

int my_free(void* ptr);

void insert_at_tail(int num);

void deattach_shared_mem();

void remove_shared_mem();

void print_it();

#endif
