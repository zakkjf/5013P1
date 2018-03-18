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
​ ​*​ ​@file​ ​main.h
​ ​*​ ​@brief​ ​pThreads exercise header file
​ ​*
​ ​*​ ​This is the header file for a three-thread PThreads example demonstrating
 * syncronous logging, printing, signal handling, file handling,
 * and CPU utilization stats
​ ​*
​ ​*​ ​@author​ ​Zach Farmer
​ ​*​ ​@date​ ​Feb 18 2018
​ ​*​ ​@version​ ​1.0
​ ​*
​ ​*/

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <sys/syscall.h>
#include "main.h"


#define FILEPATH "log.txt"
#define FILEPATH2 "random.txt"
#define FILEPATH3 "/proc/stat"

typedef enum {TEMP_THR,LIGHT_THR,SOCKET_THR,MASTER_THR} source_t;

struct info{
	char * logfile;
	char * infile;
	char * procstat;
};

struct msg{
	char initsuccess;
	//heartbeat notifications
	char response;
	char request;
	char error;
	char net_request;
	double net_response;
	char daynight;
	char close;
	char data[512];
	char format;
	double sensorvalue;
	int counter;
};

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
void* msgcpy( void* dest, const void* src);

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
void sync_printf(const char *format, ...);


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
void sig_handler(int sig);

/**
​ ​*​ ​@brief​ ​logger function for the logger thread
​ ​*
​ ​*​ ​handles logs
​ ​*
​ ​*​ ​@param​ ​source log source (what thread)
 * @param level the log level
 * @param the log message
​ *
​ ​*​ ​@return​ void
​ ​*/
char* log_str(source_t source, int level, char* msg);


/**
​ ​*​ ​@brief​ ​Synchronous logging call for thread and POSIX Ids
​ ​*
​ ​*​ ​@param​ ​filename filename of log
 * @param thread name of present thread (as char *)
​ *
​ ​*​ ​@return​ void
​ ​*/
int sync_log_id(char* filename, char* thread);


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
int sync_logwrite(char* filename,char* log);

/**
​ ​*​ ​@brief​ ​thread 1 = light sensor
​ ​*
​ ​*​ ​This thread produces the light sensor value
 *
​ ​*​ ​@param​ ​void pointer, usually for msg struct
​ *
​ ​*​ ​@return​ void
​ ​*/
void *thread1_fnt(void* ptr);

/**
​ ​*​ ​@brief​ ​child thread 2: temp sensor
​ ​*
​ ​*​ ​This child thread reports temp values in various formats
​ ​*
​ ​*​ ​@param​ ​void pointer, usually for msg struct
​ *
​ ​*​ ​@return​ void
​ ​*/
void *thread2_fnt(void* ptr);

/**
​ ​*​ ​@brief​ ​child thread 3: Logger
​ ​*
​ ​*​ ​This child thread runs the logger
​ ​*
​ ​*​ ​@param​ ​void pointer, usually for msg struct
​ *
​ ​*​ ​@return​ void
​ ​*/
void *thread3_fnt(void* ptr);

/**
​ ​*​ ​@brief​ ​child thread 4: Socket
​ ​*
​ ​*​ ​This child thread runs a server socket interface for getting temp and light sensor values
​ ​*
​ ​*​ ​@param​ ​void pointer, usually for msg struct
​ *
​ ​*​ ​@return​ void
​ ​*/
void *thread4_fnt(void* ptr);

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

#endif
