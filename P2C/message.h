#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MSG_SIZE_MAX 256
#define QUEUE_SIZE_MAX 10 //10 is the absolute maximum the linux kernel allows
#define MSG_BUFFER_SIZE MSG_SIZE_MAX + 10
#define PERMISSION 0660

typedef enum  { INIT_LOG,
				ERROR_LOG,
				EXIT_LOG} log_t;

typedef enum  { LIGHT_SENSOR,
				TEMP_SENSOR} sensor_t;

typedef enum  { CELSIUS_FORMAT,
				FAHREN_FORMAT,
				KELVIN_FORMAT,
				LUMEN_FORMAT} sformat_t;

typedef enum  { LOG_MSG,
				SENSOR_REQ_MSG,
				SENSOR_VAL_MSG,
				HEARTBEAT_MSG,
				CLOSE_MSG } msg_t;

struct log {    //Used in LOG_MSG
	log_t log_type;
	char level;
	char* text;
	unsigned int text_length;
};

struct sensor { //Used in SENSOR_REQ_MSG and SENSOR_VAL_MSG
	sensor_t sensor_type;
	double sensor_data;
	sformat_t sensor_format;
};

//HEARTBEAT_MSG and CLOSE_MSG both have null msg_info

struct msg {
    char* source;
};
    /*
	//generic information
    int poo;
	int timestamp;
	char* source; //queue name of the source/requester
	msg_t message_type;
	char LED;
	void* msg_info;
};
*/
mqd_t init_my_queue(char* queue_name);

//int send_to_queue(char* destination, struct msg* out_message);

//int read_my_queue(mqd_t my_qd,struct msg* in_message);

int exit_my_queue(mqd_t my_qd, char* queue_name);


int send_to_queue(char* destination, const char* stuff);//struct msg* out_message)

int read_my_queue(mqd_t my_qd, char* stuff);//struct msg* in_message)