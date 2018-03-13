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
#include "ADPS9301.h"
#include "TMP102.h"
#include "main.h"

#define DEBUG OFF
#define SENSOR_OUTPUT ON
#define LOGPATH "log.txt"
#define MSG_SIZE_MAX 128
#define LOG_SIZE_MAX 256
#define TEMP_LOGGING_INTERVAL 1000 //logging interval in ms DO NOT SET BELOW 50 OR YOU ARE GONNA BREAK SHIT
#define LIGHT_LOGGING_INTERVAL 1000 //logging interval in ms DO NOT SET BELOW 50 OR YOU ARE GONNA BREAK SHIT

/*DON'T MESS WITH ANYTHING BELOW THIS LINE*/
/*--------------------------------------------------------------*/
#define ON 1
#define OFF 0
#define ACTIVE ON
#define CLEAR OFF

#define FAHRENHEIT  'F'
#define CELSIUS     'C'
#define KELVIN      'K'

typedef enum {TEMP_THR,LIGHT_THR,SOCKET_THR,MASTER_THR} source_t;

FILE *fp;

pthread_t thread1;
pthread_t thread2;
pthread_t thread3;

pthread_mutex_t printf_mutex;
pthread_mutex_t log_mutex;
pthread_mutex_t sensor_mutex;
pthread_mutex_t th1_mutex;
pthread_mutex_t th2_mutex;
pthread_mutex_t th3_mutex;
pthread_mutex_t cpy_mutex;

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
void* msgcpy( void* dest, const void* src)
{
	pthread_mutex_lock(&cpy_mutex);
	memcpy(dest, src, sizeof(struct msg));
	pthread_mutex_unlock(&cpy_mutex);
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
        
        //thread-specific handling in thread_join component of main
        pthread_cancel(thread1);
        pthread_cancel(thread2);
    }

    if (sig == SIGUSR2)
    {
        //exit threads. Cannot distinguish between threads when force-sending signals
        
        //thread-specific handling in thread_join component of main
        pthread_cancel(thread1);
        pthread_cancel(thread2);
    }


}

char* log_str(source_t source, int level, char* msg)
{
	char* buffer = malloc(sizeof(char)*MSG_SIZE_MAX);
    time_t timer;
    char timebuf[25];
    struct tm* time_inf;

    time(&timer);

    time_inf = localtime(&timer);
    strftime(timebuf, 25, "%m/%d/%Y %H:%M:%S", time_inf);

    char* name;
	switch(source)
	{
	   case LIGHT_THR  :
	      name = "Light Sensor Thread";
	      break; 
	   case TEMP_THR  :
	      name = "Temp Sensor Thread";
	      break; 
	   case SOCKET_THR :
	      name = "Socket Thread";
	      break; 
	   case MASTER_THR  :
	      name = "Master Thread";
	      break; 
   	   default :
	      name = "Other";
	}

    sprintf(buffer, "%s, Level %d, Time: %s, Message:%s\n",
    		name,
    		level,
    		timebuf,
    		msg);

    return buffer;
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
int sync_logwrite(char* filename,char* log)
{
    pthread_mutex_lock(&log_mutex);

	fp = fopen (filename, "a");

	fprintf(fp,"%s", log);

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
    //
	struct msg *received = malloc(sizeof(struct msg));
	struct timespec tim;
	tim.tv_sec = 0;
	tim.tv_nsec = 1000000L; //1ms sampling loop
	//char poo;
	//poo = 0;
	 //char doo[5];
	int logcount=0;
    int err = 0;
    err+=APDS9301_power_on(0x39,"/dev/i2c-1");
    double lux;
    while(1)
    {
    	logcount++;
    	//sprintf(doo, "%d", poo);
        
        pthread_mutex_lock(&sensor_mutex);
        err+=APDS9301_get_lux(0x39,"/dev/i2c-1", &lux);
        pthread_mutex_unlock(&sensor_mutex);
        //sync_printf("Light sensor value %d: %.5lf lux\n", logcount, lux);
    	pthread_mutex_lock(&th1_mutex);
		msgcpy(received, ptr);	
	    if(received->request == ACTIVE && received->response==CLEAR)
	    {
            received->sensorvalue = lux;
			received->response = ACTIVE;
			received->request = CLEAR;
			if(logcount%LIGHT_LOGGING_INTERVAL==0)
			{
				strcpy(received->data,log_str(LIGHT_THR, 2, "THIS IS ONLY A TEST, DAWG"));
			}
			msgcpy(ptr,received);
	    }
		pthread_mutex_unlock(&th1_mutex);
		nanosleep(&tim, NULL);
    }

    free(received);
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
	struct msg *received = malloc(sizeof(struct msg));
	struct timespec tim;
	tim.tv_sec = 0;
	tim.tv_nsec = 1000000L; //1ms sampling loop
	//char poo;
	//poo = 0;
	 //char doo[5];
	int logcount=0;
    float temp;
    while(1)
    {
    	logcount++;
        pthread_mutex_lock(&sensor_mutex);
        switch(received->tempformat)
        {
           case CELSIUS:
              TMP102_get_temp_c(0x48,"/dev/i2c-1", &temp);
              break; 
           case FAHRENHEIT:
              TMP102_get_temp_f(0x48,"/dev/i2c-1", &temp);
              break; 
           case KELVIN:
              TMP102_get_temp_c(0x48,"/dev/i2c-1", &temp);
              break;
           default :
              temp = -99.99;  //ERROR, INVALID FORMAT
        }
        pthread_mutex_unlock(&sensor_mutex);
    	//sprintf(doo, "%d", poo);
    	pthread_mutex_lock(&th2_mutex);
    	//poo++;
		msgcpy(received, ptr);	
	    if(received->request == ACTIVE && received->response==CLEAR)
	    {
			received->response = ACTIVE;
			received->request = CLEAR;
            received->sensorvalue = temp;
			if(logcount%TEMP_LOGGING_INTERVAL==0)
			{
				strcpy(received->data,log_str(TEMP_THR, 2, "THIS IS ALSO A TEST, DAWG"));
			}
	    	//sync_printf("My 1 Counter:%d\n",received->counter);
			msgcpy(ptr,received);
	    }
		pthread_mutex_unlock(&th2_mutex);
		nanosleep(&tim, NULL);
    }
    free(received);
   	return NULL;
}

/**
​ ​*​ ​@brief​ ​child thread 3: logger
​ ​*
​ ​*​ ​This child thread reports CPU utilization to the console on 100ms intervals
​ ​*
​ ​*​ ​@param​ ​nfo info struct containing filenames for reading usage stats
​ *
​ ​*​ ​@return​ void
​ ​*/
void *thread3_fnt(void* ptr)
{
     //
	struct msg *received = malloc(sizeof(struct msg));
	struct timespec tim;
	tim.tv_sec = 0;
	tim.tv_nsec = 1000000L; //1ms sampling loop
    while(1)
    {
    	pthread_mutex_lock(&th3_mutex);
		msgcpy(received, ptr);	
	    if(received->request == ACTIVE && received->response==CLEAR)
	    {
			received->response = ACTIVE;
			received->request = CLEAR;
			#if DEBUG == ACTIVE
			sync_printf(received->data);//print the logs to terminal
			#endif
			sync_logwrite(LOGPATH,received->data);
			msgcpy(ptr,received);
	    }
		pthread_mutex_unlock(&th3_mutex);
		nanosleep(&tim, NULL);
    }
	free(received);
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
    pthread_mutex_init(&sensor_mutex, NULL);
    pthread_mutex_init(&th1_mutex, NULL);
    pthread_mutex_init(&th2_mutex, NULL);
    pthread_mutex_init(&th3_mutex, NULL);

    void* shmem_th1 = mmap(NULL, sizeof(struct msg), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    void* shmem_th2 = mmap(NULL, sizeof(struct msg), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    void* shmem_th3 = mmap(NULL, sizeof(struct msg), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);

    struct msg *msg_th1_write= malloc(sizeof(struct msg));
 	struct msg *msg_th2_write= malloc(sizeof(struct msg));
 	struct msg *msg_th3_write= malloc(sizeof(struct msg));

    struct msg *msg_th1_read= malloc(sizeof(struct msg));
 	struct msg *msg_th2_read= malloc(sizeof(struct msg));
 	struct msg *msg_th3_read= malloc(sizeof(struct msg));

    msg_th1_read->response = CLEAR;
    msg_th2_read->response = CLEAR;
    msg_th3_read->response = CLEAR;

    msg_th1_write->request = ACTIVE;
    msg_th2_write->request = ACTIVE;
    msg_th3_write->request = ACTIVE;

    msg_th2_read->tempformat = CELSIUS;

	memcpy(shmem_th1, msg_th1_write, sizeof(struct msg));
	memcpy(shmem_th2, msg_th2_write, sizeof(struct msg));
	memcpy(shmem_th3, msg_th3_write, sizeof(struct msg));

    /* create a first thread which executes thread1_fnt(&x) */

    if(pthread_create(&thread1, NULL, (void *)thread1_fnt, shmem_th1)) {

        fprintf(stderr, "Error creating Thread 1\n");
        return 1;
    }

    /* create a second thread which executes thread2_fnt(&x) */
   	if(pthread_create(&thread2, NULL, (void *)thread2_fnt, shmem_th2)) {

        fprintf(stderr, "Error creating Thread 2\n");
        return 1;
    }

        /* create a second thread which executes thread2_fnt(&x) */
   	if(pthread_create(&thread3, NULL, (void *)thread3_fnt, shmem_th3)) {

        fprintf(stderr, "Error creating Thread 3\n");
        return 1;
    }

    int i=0;

    char logs[5*LOG_SIZE_MAX];
    while(1)
    {
    	pthread_mutex_lock(&th1_mutex);
		msgcpy(msg_th1_read, shmem_th1);
	    if(msg_th1_read->response==ACTIVE)
	    {
	    	//sync_printf("Sensor 1: %s\n", msg_th1_read->data);
	    	msg_th1_write->response = CLEAR;
	    	msg_th1_read->response = CLEAR;
	    	strcpy(logs,msg_th1_read->data);
            //msg_th1_write->data;
	    	//sync_printf(logs);
            #if SENSOR_OUTPUT == ON
                sync_printf("Light sensor value %d: %.5lf lux\n", 
                    i,
                    msg_th1_read->sensorvalue);
            #endif
    		i++;
    		msg_th1_write->counter = i;
	    	msgcpy(shmem_th1, msg_th1_write);
	    }
	    pthread_mutex_unlock(&th1_mutex);

    	pthread_mutex_lock(&th2_mutex);
	   	msgcpy(msg_th2_read, shmem_th2);
	    if(msg_th2_read->response==ACTIVE && msg_th1_read->request==CLEAR)
	    {
	    	//sync_printf("Sensor 2: %s\n", msg_th2_read->data);
	    	msg_th2_write->response = CLEAR;
	    	msg_th2_read->response = CLEAR;

	    	strcat(logs,msg_th2_read->data);
            #if SENSOR_OUTPUT == ON
                sync_printf("Temp sensor value %d: %.4f deg %c\n", 
                    i,
                    msg_th2_read->sensorvalue,
                    msg_th2_read->tempformat);
            #endif
	    	//sync_printf(logs);
	    	//msg_th2_write->request = req2;
	    	msgcpy(shmem_th2, msg_th2_write);
	    }
	    pthread_mutex_unlock(&th2_mutex);

    	pthread_mutex_lock(&th3_mutex);
	   	msgcpy(msg_th3_read, shmem_th3);
	    if(msg_th3_read->response==ACTIVE && msg_th3_read->request==CLEAR)
	    {
	    	//sync_printf("Sensor 2: %s\n", msg_th2_read->data);
	    	msg_th3_write->response = CLEAR;
	    	msg_th3_read->response = CLEAR;
	    	strcpy(msg_th3_write->data,logs);
	    	//sync_printf(logs);
	    	msgcpy(shmem_th3, msg_th3_write);
	    }
	    pthread_mutex_unlock(&th3_mutex);

	}
    /* wait for the second thread to finish */
    if(pthread_join(thread1, NULL)) {

        
        return 2;

    }
    else
    {
        
        
    }

    if(pthread_join(thread2, NULL)) {

        
        return 2;

    }
    else
    {
        
        
    }

    if(pthread_join(thread3, NULL)) {

        
        return 2;

    }
    else
    {
        
        
    }

    free(msg_th1_write);
 	free(msg_th2_write);
 	free(msg_th3_write);
    free(msg_th1_read);
 	free(msg_th2_read);
 	free(msg_th3_read);

    munmap(shmem_th1, sizeof(struct msg));
    munmap(shmem_th2, sizeof(struct msg));
    munmap(shmem_th3, sizeof(struct msg));

    return 0;
}
