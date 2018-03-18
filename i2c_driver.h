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
​ ​*​ ​@file​ ​i2c_driver.h
​ ​*​ ​@brief​ ​i2c driver Project 1
​ ​*
​ ​*​ ​This​ ​is the project 1 i2c base driver
​ ​*
​ ​*​ ​@author​ ​Zach Farmer
​ ​*​ ​@date​ ​Mar 16 2018
​ ​*
​ ​*/

#ifndef _I2C_DRIVER_H_
#define _I2C_DRIVER_H_

#include <stdlib.h>

/**
​ ​*​ ​@brief​ ​open a hardware location
​ ​*
​ ​*​ ​opens a hardware location
​ ​*
​ ​*​ ​@param​ ​filename the name of the hardware /dev file
​ *
​ ​*​ ​@return​ void
​ ​*/
int i2c_open(char* filename);
/**
​ ​*​ ​@brief​ ​close a hardware location
​ ​*
​ ​*​ ​closes a hardware location
​ ​*
​ ​*​ ​@param​ ​i2c file int
​ *
​ ​*​ ​@return​ void
​ ​*/
int i2c_close(int i2c_file);

/**
​ ​*​ ​@brief set a single i2c register
​ ​*
​ ​*
​ ​*​ ​@param​ ​file file int, produced by open
 * @param addr address of device
 * @param reg register on device
 * @param value value to write to reg
​ *
​ ​*​ ​@return​ void
​ ​*/
int set_i2c_register(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char value);

/**
​ ​*​ ​@brief set a double (16bit) i2c register
​ ​*
​ ​*
​ ​*​ ​@param​ ​file file int, produced by open
 * @param addr address of device
 * @param reg register on device
 * @param lo byte to write to reg
 * @param hi byte to write to reg
​ *
​ ​*​ ​@return​ void
​ ​*/
int set_i2c_registers(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char lo,
                            unsigned char hi);

/**
​ ​*​ ​@brief repeated-start read a register
​ ​*
​ ​*
​ ​*​ ​@param​ ​file file int, produced by open
 * @param addr address of device
 * @param reg register on device
 * @param val value pointer to read in from register
​ *
​ ​*​ ​@return​ void
​ ​*/
int get_i2c_register_repstart(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char *val);

/**
​ ​*​ ​@brief read a multiple-byte register
​ ​*
​ ​*
​ ​*​ ​@param​ ​file file int, produced by open
 * @param addr address of device
 * @param reg register on device
 * @param count number of bytes to read
 * @param vals value pointer to read in from register
​ *
​ ​*​ ​@return​ void
​ ​*/
int get_i2c_registers(int file,
                    unsigned char addr,
                    unsigned char reg,
                    unsigned char count,
                    unsigned char *vals);

#endif
