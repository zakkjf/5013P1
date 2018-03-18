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
​ ​*​ ​@file​ ​ADPS9301.h
​ ​*​ ​@brief​ ​light sensor driver Project 1
​ ​*
​ ​*​ ​This​ ​is the project 1 i2c light sensor driver
​ ​*
​ ​*​ ​@author​ ​Zach Farmer
​ ​*​ ​@date​ ​Mar 16 2018
​ ​*
​ ​*/
#ifndef _ADPS9301_H_
#define _ADPS9301_H_

#include <stdint.h>

#define BIT(n) (0x1U << (n))

/* Command register bits */
#define ADPS9301_CMD			BIT(7) /* Select command register. Must write as 1 */
#define ADPS9301_WORD			BIT(5) /* I2C write/read: if 1 word, if 0 byte */
#define ADPS9301_CLEAR			BIT(6) /* Interrupt clear. Clears pending interrupt */

/* Register set */
#define ADPS9301_CONTROL		0x00 /* Control of basic functions */
#define ADPS9301_TIMING			0x01 /* Timing and gain register */
#define ADPS9301_THRESHLOWLOW	0x02 /* Low byte of low interrupt threshold */
#define ADPS9301_THRESHLOWHIGH	0x03 /* Low byte of low interrupt threshold */
#define ADPS9301_THRESHHIGHLOW	0x04 /* Low byte of high interrupt threshold */
#define ADPS9301_THRESHHIGHHIGH	0x05 /* Low byte of high interrupt threshold */
#define ADPS9301_INTERRUPT		0x06 /* Interrupt control */
#define ADPS9301_ID				0x0A /* Product ID (Test Reg)*/

#define ADPS9301_CH0LOW			0x0C /* Low byte of ADC channel 0 */
#define ADPS9301_CH0HIGH		0x0D /* High byte of ADC channel 0 */
#define ADPS9301_CH1LOW			0x0E /* Low byte of ADC channel 1 */
#define ADPS9301_CH1HIGH		0x0F /* High byte of ADC channel 1 */

/* Power on/off value for ADPS9301_CONTROL register */
#define ADPS9301_POWER_ON		0x03
#define ADPS9301_POWER_OFF		0x00

#define ADPS9301_GAIN_MASK		0x04
#define ADPS9301_INTR_MASK   	0x04
#define ADPS9301_TIMING_MASK	0x03

/**
​ ​*​ ​@brief​ ​get channel 0 of light sensor
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param value to pass data into
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_get_CH0(int addr, char* interface, int16_t* value);

/**
​ ​*​ ​@brief​ ​get channel 1 of light sensor
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param value to pass data into
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_get_CH1(int addr, char* interface, int16_t* value);

/**
​ ​*​ ​@brief​ ​get converted lux value of sensor
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param value to pass data into
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_get_lux(int addr, char* interface, double* value);

/**
​ ​*​ ​@brief​ ​power on sensor
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_power_on(int addr, char* interface);

/**
​ ​*​ ​@brief​ ​run tests for all sensors
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_run_everything(int addr, char* interface);

/**
​ ​*​ ​@brief​ ​set sensor timing
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param msg to set timing to (two bits)
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_set_timing(int addr, char* interface, uint8_t msg);

/**
​ ​*​ ​@brief​ ​get sensor timing
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param val pointer to pass data into
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_get_timing(int addr, char* interface, uint8_t* val);

/**
​ ​*​ ​@brief​ ​set high gain for sensor
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_set_high_gain(int addr, char* interface);

/**
​ ​*​ ​@brief​ ​set low gain for sensor
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_set_low_gain(int addr, char* interface);

/**
​ ​*​ ​@brief​ ​set sensor integration time 
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param msg to set timing to (two bits)
​ *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_set_integ_time(int addr, char* interface, uint8_t msg);

/**
​ ​*​ ​@brief​ ​enable interrupt
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_enable_int(int addr, char* interface);

/**
​ ​*​ ​@brief​ ​disable interrupt
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_disable_int(int addr, char* interface);

/**
​ ​*​ ​@brief​ ​get sensor id
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param value to pass data into
 *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_get_id(int addr, char* interface, uint8_t* value);

/**
​ ​*​ ​@brief​ ​set sensor thresholds
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param low threshold
 * @param high threshold
 *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_set_thresholds(int addr, char* interface, uint16_t low, uint16_t high);

/**
​ ​*​ ​@brief​ ​get sensor thresholds
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param low threshold
 * @param high threshold
 *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_get_thresholds(int addr, char* interface, uint16_t* low, uint16_t* high);

/**
​ ​*​ ​@brief​ ​write single register
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param reg register to write to
 * @param msg message to write
 *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_write_single(int addr, char* interface, char reg, char msg);

/**
​ ​*​ ​@brief​ ​write single register
​ ​*
​ ​*
​ ​*​ ​@param​ addr address of sensor
 * @param interface of sensor, "dev/i2c-1", etc
 * @param reg register to write to
 * @param value value to read in
 *
​ ​*​ ​@return​ 0 if successful
​ ​*/
int ADPS9301_read_single(int addr, char* interface, char reg, uint8_t* value);

#endif

