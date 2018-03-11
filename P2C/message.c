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


//server and client do this
static int close_queue(mqd_t qd)
{
    if(mq_close(qd) == -1) {
        //printf("ERROR: mq_close\n");
        return 1;
    }

    return 0;
}

int exit_my_queue(mqd_t my_qd, char* queue_name)
{
	if(close_queue(my_qd)==1)
	{
		return 1;
	}

    if(mq_unlink(queue_name) == -1) {
        //printf("ERROR: mq_unlink\n");
        return 1;
    }
    return 0;
}


static mqd_t open_queue(char *srv_queue_name)
{
	mqd_t qd_dest;
	//open server message queue from client
    if((qd_dest = mq_open(srv_queue_name, O_WRONLY)) == -1) {
        //printf("ERROR: open queue\n");
        return 1;
    }

    return qd_dest;
}

mqd_t init_my_queue(char *queue_name)
{
	mqd_t my_qd;
	//open server message queue from server
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = QUEUE_SIZE_MAX;
    attr.mq_msgsize = sizeof(struct msg);
    attr.mq_curmsgs = 0;

    if((my_qd = mq_open(queue_name, O_RDONLY | O_CREAT, PERMISSION, &attr)) == -1) {
        //printf("ERROR: init my queue\n");
        ////printf("Oh dear, something went wrong with read()! %s\n", strerror(errno));
        return 1;
    }

    return my_qd;
}

//send message
int send_to_queue(char* destination, const char* stuff)//struct msg* out_message)
{
	mqd_t qd_dest;

	//open destination queue as writable
	if((qd_dest=open_queue(destination))==1){
		return 1;
	}

    //send message to destiniation queue
    //if(mq_send(*qd_dest, ((const char*)out_message), sizeof(struct msg), 0) == -1) {
	if(mq_send(qd_dest, stuff, 128, 0) == -1) {

        //printf("ERROR: Not able to send message\n");
        return 1;
    }

    //close destination queue
    if(close_queue(qd_dest)==1)
    {
    	return 1;
    }

    return 0;
}

//read a message in from the given readable queue descriptor, typically your own,
//passing it through struct msg* message
int read_my_queue(mqd_t my_qd, char* stuff)//struct msg* in_message)
{
    // get the string from client
    //if(mq_receive(*my_qd, (char*) in_message, sizeof(struct msg), NULL) == -1) {
    if(mq_receive(my_qd, stuff, 128, NULL) == -1) {

        //printf("ERROR: read_my queue\n");
        return 1;
    }
    ////printf("Server: Message string received: %s\n", buffer);

	return 0;	
}