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
​ ​*​ ​@brief​ ​mQueues IPC Example
​ ​*
​ ​*​ ​This​ ​is an example of how mQueues can be used for IPC
 * (Inter-process communication)
​ ​*
​ ​*​ ​@author​ ​Zach Farmer
​ ​*​ ​@date​ ​Mar 3 2018
​ ​*​ ​@version​ ​69
​ ​*
​ ​*/

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "message.h"
#include <errno.h>


#define SRV_QUEUE_NAME "/testqueue"
#define CLIENT_QUEUE_NAME "/ballsac"

pthread_mutex_t printf_mutex;
pthread_mutex_t log_mutex;

/**
​ ​*​ ​@brief​ ​Synchronous encapsulator for printf
​ ​*
​ ​*​ ​Mutexes printf for asynchronous call protection
 * among multiple threads
​ ​*
​ ​*​ ​@param​ ​format print formatting
 * @param ... variadic arguments for print(char *, char, etc)
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

/**
​ ​*​ ​@brief​ clientside/childside mQueue message function
​ ​*
​ ​*​ ​client side of Mqueue IPC
​ ​*
​ ​*​ ​@param​ message message to send to server/parent
 *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int client(struct msg* message)
{
	//struct msg received;
    char received[64];
    char* client_queue_name;
    mqd_t qd_client;

    sync_printf("Client Thread: Active\n");

    // create the client queue for receiving messages from server
    //sprintf(client_queue_name, "/sp-example-client-%d", getpid());
    //client_queue_name = CLIENT_QUEUE_NAME;

    //sync_printf("Source of client message %s\n", client_queue_name);

    client_queue_name = CLIENT_QUEUE_NAME;

    qd_client=init_my_queue(client_queue_name);

    send_to_queue(SRV_QUEUE_NAME, client_queue_name);

    read_my_queue(qd_client,received);
	
	sync_printf("Source of client message %s", received);

	exit_my_queue(qd_client,client_queue_name);

    sync_printf("Client: Exiting\n");

    return 0;
}


/**
​ ​*​ ​@brief​ serverside/parentside mQueue message function
​ ​*
​ ​*​ ​server side of Mqueue IPC
​ ​*
​ ​*​ ​@param​ message message response to client/child
 *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int server(struct msg* message)
{
    mqd_t qd_server;
    //struct msg received;
    char  received[64];

    message->source = SRV_QUEUE_NAME;

	sync_printf("Server Thread: Active\n");

    qd_server=init_my_queue(SRV_QUEUE_NAME);

    read_my_queue(qd_server,received);

    // display LED settings received from server
	sync_printf("Source of server message %s", received);

	//send message back to where it came from
    send_to_queue(CLIENT_QUEUE_NAME, SRV_QUEUE_NAME);

	exit_my_queue(qd_server,SRV_QUEUE_NAME);

	//sync_printf("HERE2");

    sync_printf("Server: Exiting\n");
    return 0;
}

int main(void)
{
    pid_t   childpid;

	pthread_mutex_init(&printf_mutex, NULL);

	//sync_printf("mQueues IPC Example\n\n");
	struct msg* message1 = (struct msg*) malloc(sizeof(struct msg));

	struct msg* message2 = (struct msg*) malloc(sizeof(struct msg));
      

	//sync_printf("size of msgstruct %d", (int)sizeof(struct msg));

	sync_printf("message1 source is %s\n",message2->source);

    if((childpid = fork()) == -1)
    {
            perror("fork\n");
            exit(1);
    }

    if(childpid == 0)
    {
		client(message1);
        exit(0);
    }
    else
    {
		server(message2);
    }
    return(0);
}
