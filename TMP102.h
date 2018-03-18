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
​ ​*​ ​@brief​ ​tmp102 driver Project 1
​ ​*
​ ​*​ ​This​ ​is the project 1 tmp102 temp sensor driver
​ ​*
​ ​*​ ​@author​ ​Zach Farmer
​ ​*​ ​@date​ ​Mar 16 2018
​ ​*
​ ​*/
#ifndef _TMP102_H_
#define _TMP102_H_

#include <stdint.h>

/* Command register bits */
#define TMP102_TEMP_REG 0x00
#define TMP102_CONF_REG 0x01
#define TMP102_T_LO_REG 0x02
#define TMP102_T_HI_REG 0x03

#define TMP102_CONVRATE_POINT25HZ 0x00
#define TMP102_CONVRATE_1HZ 0x01
#define TMP102_CONVRATE_4HZ 0x02 //default
#define TMP102_CONVRATE_8HZ 0x03

#define TMP102_CONF_SHUTDOWN 0x0001
#define TMP102_CONF_RES 	 0x0060
#define TMP102_CONF_FAULT 	 0x0018
#define TMP102_CONF_EM 		 0x1000
#define TMP102_CONF_CONVRATE 0xC000

/**
​ ​*​ ​@brief​ ​get temp in celsius
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param value float to pass temp into
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int TMP102_get_temp_c(int addr, char* interface, float* value);

/**
​ ​*​ ​@brief​ ​get temp in fahrenheit
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param value float to pass temp into
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int TMP102_get_temp_f(int addr, char* interface, float* value);

/**
​ ​*​ ​@brief​ ​get temp in kelvin
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param value float to pass temp into
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int TMP102_get_temp_k(int addr, char* interface, float* value);

/**
​ ​*​ ​@brief​ ​get tlow threshhold register
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param value float to pass tlow into
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int TMP102_get_tlow(int addr, char* interface, uint8_t* value);

/**
​ ​*​ ​@brief​ ​get thigh  threshold register
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param value float to pass temp into
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int TMP102_get_thigh(int addr, char* interface, uint8_t* value);

/**
​ ​*​ ​@brief​ ​get sensor config
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param value value to pass data into
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int TMP102_get_config(int addr, char* interface, uint16_t* value);

/**
​ ​*​ ​@brief​ ​get sensor config
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param value to pass data into sensor
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int TMP102_set_config(int addr, char* interface, uint16_t value);

/**
​ ​*​ ​@brief​ ​shutdown sensor
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int TMP102_shutdown_on(int addr, char* interface);

/**
​ ​*​ ​@brief​ ​turn on sensor
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int TMP102_shutdown_off(int addr, char* interface);

/**
​ ​*​ ​@brief​ ​turn on EM mode
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int TMP102_EM_on(int addr, char* interface);

/**
​ ​*​ ​@brief​ ​turn off EM mode
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int TMP102_EM_off(int addr, char* interface);

/**
​ ​*​ ​@brief​ ​set resolution of sensor
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param res resolution of sensor (LSB two bits only)
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int TMP102_set_res(int addr, char* interface, char res);

/**
​ ​*​ ​@brief​ ​set conv rate of sensor (see defines for choices)
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param res resolution of sensor (LSB two bits only)
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int TMP102_set_conv(int addr, char* interface, char conv);

/**
​ ​*​ ​@brief​ ​set fault bits in sensor
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param res resolution of sensor (LSB two bits only)
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int TMP102_set_fault(int addr, char* interface, char fault);

#endif

