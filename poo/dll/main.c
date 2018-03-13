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
#include <pthread.h>
#include <stdarg.h>
#include <stdint.h>
#include "shared_dll.h"
pthread_mutex_t printf_mutex;
pthread_mutex_t dll_mutex;
/**
​ ​*​ ​@brief​ ​Synchronous encapsulator for printf
​ ​*
​ ​*​ ​Mutexes printf for asynchronous call protection
 * among multiple threads
​ ​*
​ ​*​ ​@param​ ​format print formatting
 * @param ... variadic arguments for print (char *, char, etc)
​ *
​ ​*​ ​@return​ void
​ ​*/
void sync_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    pthread_mutex_lock(&printf_mutex);
    vprintf(format, args);
    pthread_mutex_unlock(&printf_mutex);

    va_end(args);
}

int main() {
    printf("Shared Memory IPC Example\n\n");

    pid_t   childpid;

    //struct msg* message1 = mmap(NULL, 128, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    //malloc(sizeof(struct msg));

    pthread_mutex_init(&printf_mutex, NULL);
    pthread_mutex_init(&dll_mutex, NULL);




    Node * n1 = init_dll(1);
    uint32_t i;
    for(i=2;i<10;i++)
    {
        n1 = insert_at_end(n1,i);
    }
    //printAll(n1);
    if((childpid = fork()) == -1)
    {
            perror("fork");
            exit(1);
    }
    if (childpid == 0)
    {
        sync_printf("Child");
       pthread_mutex_lock(&dll_mutex);
        n1=insert_at_position(n1,5,243);
        //printAll(n1);
       pthread_mutex_unlock(&dll_mutex);
    } 
    else 
    {   
        sync_printf("Parent");
        sleep(1);
       // pthread_mutex_lock(&dll_mutex);
        //n1=delete_at_position(n1,5);
        printAll(n1);
       /// pthread_mutex_unlock(&dll_mutex);
        sleep(5);
        pthread_mutex_lock(&dll_mutex);
        n1 = destroy(n1);
        pthread_mutex_unlock(&dll_mutex);
        
    }

    return 0;
}
