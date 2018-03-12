/*****************************************************************************
​ ​*​ ​Copyright​ ​(C)​ ​2018 ​by​ Zach Farmer
​ ​*
​ ​*​ ​Redistribution,​ ​modification​ ​or​ ​use​ ​of​ ​this​ ​software​ ​in​ ​source​ ​or​ ​binary
​ ​*​ ​forms​ ​is​ ​permitted​ ​as​ ​long​ ​as​ ​the​ ​files​ ​maintain​ ​this​ ​copyright.​ ​Users​ ​are
​ ​*​ ​permitted​ ​to​ ​modify​ ​this​ ​and​ ​use​ ​it​ ​to​ ​learn​ ​about​ ​the​ ​field​ ​of​ ​embedded
​ ​*​ ​software.​ Zach Farmer, ​Alex​ ​Fosdick​, and​ ​the​ ​University​ ​of​ ​Colorado​ ​are​ ​not​
 * ​liable​ ​for ​any​ ​misuse​ ​of​ ​this​ ​material.
​ ​*
*****************************************************************************/
/**
​ ​*​ ​@file​ ​main.c
​ ​*​ ​@brief​ ​pThreads example source
​ ​*
​ ​*​ ​This is the source file for a three-thread PThreads example demonstrating
 * syncronous logging, printing, signal handling, file handling,
 * and CPU utilization stats
​ ​*
​ ​*​ ​@author​ ​Zach Farmer
​ ​*​ ​@date​ ​Feb 18 2018
​ ​*​ ​@version​ ​1.0
​ ​*
​ ​*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <sys/syscall.h>
#include <time.h>
#include <sys/mman.h>
#include "main.h"

#define _GNU_SOURCE
#define FILEPATH "log.txt"
#define FILEPATH2 "random.txt"
#define FILEPATH3 "/proc/stat"

#define ACTIVE 1
#define CLEAR  0

FILE *fp;

pthread_t thread1;
pthread_t thread2;

pthread_mutex_t printf_mutex;
pthread_mutex_t log_mutex;
pthread_mutex_t logger_mutex;
pthread_mutex_t sensor_mutex;
pthread_mutex_t th1_mutex;
pthread_mutex_t th2_mutex;

/**
​ ​*​ ​@brief​ ​thread-safe message copy
​ ​*
​ ​*​ ​Mutexes printf for asynchronous call protection
 * among multiple threads
​ ​*
​ ​*​ ​@param​ ​dest destination message
 ​*​ ​@param​ ​src source message
 ​*​ ​@param​ ​mutex the mutex used
 * @param ... variadic arguments for print (char *, char, etc)
​ *
​ ​*​ ​@return​ void
​ ​*/
void* msgcpy( void* dest, const void* src, pthread_mutex_t* mutex)
{
	pthread_mutex_lock(mutex);
	memcpy(dest, src, sizeof(struct msg));
	pthread_mutex_unlock(mutex);
	return dest;
}
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

/**
​ ​*​ ​@brief​ ​Signal handler for this program
​ ​*
​ ​*​ ​handles SIGUSR1 and SIGUSR2 signals,
 * which exit all child threads
​ ​*
​ ​*​ ​@param​ ​sig received signal
​ *
​ ​*​ ​@return​ void
​ ​*/
void sig_handler(int sig)
{

    if (sig == SIGUSR1)
    {
        //exit threads. Cannot distinguish between threads when force-sending signals
        //sync_logwrite(FILEPATH,"All Threads", "Received SIGUSR1");
        //thread-specific handling in thread_join component of main
        pthread_cancel(thread1);
        pthread_cancel(thread2);
    }

    if (sig == SIGUSR2)
    {
        //exit threads. Cannot distinguish between threads when force-sending signals
        //sync_logwrite(FILEPATH,"All Threads", "Received SIGUSR2");
        //thread-specific handling in thread_join component of main
        pthread_cancel(thread1);
        pthread_cancel(thread2);
    }


}

/**
​ ​*​ ​@brief​ ​Synchronous time-tagging function
​ ​*
​ ​*​ ​Synchronously prints a timestamp to the console
​ ​*
​ ​*​ ​@param​ msg message to accompany timestamp
​ ​*​ ​@return​ void
​ ​*/
int sync_timetag(char * filename, char * msg)
{
    time_t timer;
    char buffer[25];
    struct tm* time_inf;

    time(&timer);

    time_inf = localtime(&timer);
    strftime(buffer, 25, "%m/%d/%Y %H:%M:%S", time_inf);
    sync_printf(msg);
    //sync_logwrite(filename,msg,buffer);
    sync_printf(": ");
    sync_printf(buffer);
    sync_printf("\n");
    return 0;
}

/**
​ ​*​ ​@brief​ Tracks alphanumeric characters into counting bins
​ ​*
​ ​*​ ​Mutexes printf for asynchronous call protection
 * among multiple threads
​ ​*
​ ​*​ ​@param​ ch char to track
 * @param arr storage structure for counted bins
​ *
​ ​*​ ​@return​ void
​ ​*/
int chartrack(char ch, int * arr)
{
    if(ch>96 && ch<123)
    {
        ch=ch-32;
    }

    if(ch>64 && ch<91)
    {
        //incrememnt the correct bin
        (*(arr+(ch-65)))++;
        ch=ch-65;
        //sync_printf("%d\n", ch);
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
​ ​*​ ​@brief​ ​Synchronous logging call for thread and POSIX Ids
​ ​*
​ ​*​ ​@param​ ​filename filename of log
 * @param thread name of present thread (as char *)
​ *
​ ​*​ ​@return​ void
​ ​*/
int sync_log_id(char* filename, char* thread)
{
    pthread_mutex_lock(&log_mutex);
    int tid, pid;
    tid = syscall(SYS_gettid);
    pid = getpid();

	fp = fopen (filename, "a");

	fprintf(fp,"%s", thread);
	fprintf(fp,"%s"," : ");
	fprintf(fp,"Linux Thread ID: %d POSIX Thread ID: %d",tid,pid);
	fprintf(fp,"%s","\n");
	//Flush file output
	fflush(fp);

	//Close the file
	fclose(fp);
    pthread_mutex_unlock(&log_mutex);

    return 0;
}

/**
​ ​*​ ​@brief​ ​Synchronous logging call
​ ​*
​ ​*​ ​logs synchonously to specified file
​ ​*
​ ​*​ ​@param​ ​filename of log
 * @param name of thread thread currently logging
 * @param log text to log
​ *
​ ​*​ ​@return​ void
​ ​*/
int sync_logwrite(char* filename, char* thread, char* log)
{
    pthread_mutex_lock(&log_mutex);

	fp = fopen (filename, "a");

	fprintf(fp,"%s", thread);
	fprintf(fp,"%s", " : ");
	fprintf(fp,"%s", log);
	fprintf(fp,"%s", "\n");
	//Flush file output
	fflush(fp);

	//Close the file
	fclose(fp);
    pthread_mutex_unlock(&log_mutex);

    return 0;
}
/**
​ ​*​ ​@brief​ ​child thread 1
​ ​*
​ ​*​ ​This child thread sorts an input random text file
 * from a doubly linked list and prints any characters
 * occurring more than 3 times to the console
​ ​*
​ ​*​ ​@param​ ​nfo info struct containing filenames for reading and logging
​ *
​ ​*​ ​@return​ void
​ ​*/
void *thread1_fnt(void* ptr)
{
    ////sync_logwrite(nfo->logfile,"Thread 1","Thread 1 Enter");
	struct msg *received = malloc(sizeof(struct msg));
    while(1)
    {
    	//pthread_mutex_lock(&th1_mutex);
		msgcpy(received, ptr, &th1_mutex);	
	    if(received->request == ACTIVE)
	    {
			received->response = ACTIVE;
			received->request = CLEAR;
			received->data = "BALLS!";
			msgcpy(ptr,received,&th1_mutex);
	    }
		//pthread_mutex_unlock(&th1_mutex);
		//sleep(1);
    }
   	return NULL;
}


/**
​ ​*​ ​@brief​ ​child thread 2
​ ​*
​ ​*​ ​This child thread reports CPU utilization to the console on 100ms intervals
​ ​*
​ ​*​ ​@param​ ​nfo info struct containing filenames for reading usage stats
​ *
​ ​*​ ​@return​ void
​ ​*/
void *thread2_fnt(void* ptr)
{
    ////sync_logwrite(nfo->logfile,"Thread 2","Thread 2 Enter");
   	struct msg *received = malloc(sizeof(struct msg));
    while(1)
    {
    	//pthread_mutex_lock(&th2_mutex);
		msgcpy(received, ptr,&th2_mutex);
	    if(received->request == ACTIVE)
	    {
			received->response = ACTIVE;
			received->request = CLEAR;
			received->data = "ASS!";
			msgcpy(ptr,received,&th2_mutex);
	    }
		//pthread_mutex_unlock(&th2_mutex);
		//sleep(1);
    }
   	return NULL;
}

/**
​ ​*​ ​@brief​ main
​ ​*
​ ​*​ Begins logging, calls child threads, synchonously
 * waits until child thread completion to continue,
 * initializes signal handlers, mutexes, and info object
​ ​*
​ ​*​ ​
​ ​*​ ​@return​ void
​ ​*/
int main()
{
    //attach signal handlers
    if (signal(SIGUSR1, sig_handler) == SIG_ERR)
    sync_printf("Error: Can't catch SIGUSR1\n");

    if (signal(SIGUSR2, sig_handler) == SIG_ERR)
    sync_printf("Error: Can't catch SIGUSR2\n");

    //initialize mutexes for logging and printing
    pthread_mutex_init(&printf_mutex, NULL);
    pthread_mutex_init(&log_mutex, NULL);
    pthread_mutex_init(&logger_mutex, NULL);
    pthread_mutex_init(&sensor_mutex, NULL);
    pthread_mutex_init(&th1_mutex, NULL);
    pthread_mutex_init(&th2_mutex, NULL);

    //create and assign info struct to pass into threads
	struct info *nfo = (struct info*) malloc(sizeof(struct info));

	nfo->infile = FILEPATH2;
	nfo->logfile = FILEPATH;
	nfo->procstat= FILEPATH3;

    //sync_logwrite(nfo->logfile,"Thread Main","Thread Main");
    //sync_log_id(nfo->logfile,"Thread Main");
    //sync_timetag(nfo->logfile,"Thread Main Start");
    
    void* shmem_th1 = mmap(NULL, sizeof(struct msg), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    void* shmem_th2 = mmap(NULL, sizeof(struct msg), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);

    struct msg *msg_th1= malloc(sizeof(struct msg));
 	struct msg *msg_th2= malloc(sizeof(struct msg));
 
   // struct msg *msg_th1= malloc(sizeof(struct msg));
 	//struct msg *msg_th2= malloc(sizeof(struct msg));
    msg_th1->response = CLEAR;
    msg_th1->response = CLEAR;
    msg_th1->request = ACTIVE;
    msg_th2->request = ACTIVE;

	memcpy(shmem_th1, msg_th1, sizeof(struct msg));
	memcpy(shmem_th2, msg_th2, sizeof(struct msg));

    //struct msg *rx = malloc(sizeof(struct msg));
    /* create a first thread which executes thread1_fnt(&x) */
    //sync_timetag(nfo->logfile,"Thread 1 Start");
    sync_printf("Got here\n");
    if(pthread_create(&thread1, NULL, (void *)thread1_fnt, shmem_th1)) {

        fprintf(stderr, "Error creating Thread 1\n");
        return 1;

    }
        /* create a second thread which executes thread2_fnt(&x) */
    //sync_timetag(nfo->logfile,"Thread 2 Start");
   	if(pthread_create(&thread2, NULL, (void *)thread2_fnt, shmem_th2)) {

        fprintf(stderr, "Error creating Thread 2\n");
        return 1;

    }
    char req1=0, req2=0;
    while(1)
    {
    	msgcpy(msg_th1, shmem_th1, &th1_mutex);
		msgcpy(msg_th2, shmem_th2, &th2_mutex);

	    if(msg_th1->response==ACTIVE)
	    {
	    	sync_printf("Sensor 1: %s\n", msg_th1->data);
	    	msg_th1->response = CLEAR;
	    	msg_th1->request = req1;
	    	req1 = CLEAR;
	    	msgcpy(shmem_th1, msg_th1, &th1_mutex);
	    }
	   
	    if(msg_th2->response==ACTIVE)
	    {
	    	sync_printf("Sensor 2: %s\n", msg_th2->data);
	    	msg_th2->response = CLEAR;
	    	msg_th2->request = req2;
	    	msgcpy(shmem_th2, msg_th2, &th2_mutex);
	    }

	    //sleep(1);
	    //heartbeat
	   	req1 = ACTIVE;
	   	req2 = ACTIVE;
	}
    /* wait for the second thread to finish */
    if(pthread_join(thread1, NULL)) {

        //sync_logwrite(nfo->logfile,"Thread 1","Joining error");
        return 2;

    }
    else
    {
        //sync_logwrite(nfo->logfile,"Thread 1","Exiting");
        //sync_timetag(nfo->logfile,"Thread 1 End");
    }

    if(pthread_join(thread2, NULL)) {

        //sync_logwrite(nfo->logfile,"Thread 2","Joining error");
        return 2;

    }
    else
    {
        //sync_logwrite(nfo->logfile,"Thread 2","Exiting");
        //sync_timetag(nfo->logfile,"Thread 2 End");
    }
    munmap(shmem_th1, sizeof(struct msg));
    munmap(shmem_th2, sizeof(struct msg));
    //sync_logwrite(nfo->logfile,"Thread Main","Exiting");
    //sync_timetag(nfo->logfile,"Thread Main End");
    free(nfo); //free nfo struct memory
    return 0;
}
