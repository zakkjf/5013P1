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
​ ​*​ ​@file​ ​i2c_driver.c
​ ​*​ ​@brief​ ​i2c driver Project 1
​ ​*
​ ​*​ ​This​ ​is the project 1 i2c base driver
​ ​*
​ ​*​ ​@author​ ​Zach Farmer
​ ​*​ ​@date​ ​Mar 16 2018
​ ​*
​ ​*/

#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include "i2c_driver.h"

/**
​ ​*​ ​@brief​ ​open a hardware location
​ ​*
​ ​*​ ​opens a hardware location
​ ​*
​ ​*​ ​@param​ ​filename the name of the hardware /dev file
​ *
​ ​*​ ​@return​ void
​ ​*/
int i2c_open(char* filename) {
    int i2c_file;

    // Open a connection to the I2C userspace control file.
    if ((i2c_file = open(filename, O_RDWR)) < 0) {
        perror("Unable to open i2c control file");
        return -1;
    }

    return i2c_file;
}

/**
​ ​*​ ​@brief​ ​close a hardware location
​ ​*
​ ​*​ ​closes a hardware location
​ ​*
​ ​*​ ​@param​ ​i2c file int
​ *
​ ​*​ ​@return​ void
​ ​*/
int i2c_close(int i2c_file) {
    // close connection to the I2C userspace control file.
    return close(i2c_file);
}

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
                            unsigned char value) {

    unsigned char outbuf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = outbuf;

    outbuf[0] = reg;

    outbuf[1] = value;

    packets.msgs  = messages;
    packets.nmsgs = 1;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return 1;
    }

    return 0;
}

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
                            unsigned char hi) {

    unsigned char outbuf[3];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = outbuf;

    outbuf[0] = reg;

    outbuf[1] = lo;
    outbuf[2] = hi;

    packets.msgs  = messages;
    packets.nmsgs = 1;

    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return 1;
    }

    return 0;
}

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
                            unsigned char *val) {
    //unsigned char inbuf, outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = 2;//sizeof(outbuf);
    messages[0].buf   = &reg;

    messages[1].addr  = addr;
    messages[1].flags = I2C_M_RD;
    messages[1].len   = 1;//sizeof(inbuf);
    messages[1].buf   = val;//&inbuf;


    packets.msgs      = messages;
    packets.nmsgs     = 2;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return 1;
    }

    return 0;
}

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
                    unsigned char *vals) {
    //unsigned char inbuf, outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    /*
     * can read multiple registers here
     */
    //outbuf = reg;
    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = 2;//sizeof(outbuf);
    messages[0].buf   = &reg;

    /* The data will be returned in this structure */
    messages[1].addr  = addr;
    messages[1].flags = I2C_M_RD | I2C_M_NOSTART;
    messages[1].len   = count;//sizeof(inbuf);
    messages[1].buf   = vals;//&inbuf;

    /* Send the request to the kernel and receive result*/
    packets.msgs      = messages;
    packets.nmsgs     = 2;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return 1;
    }
    //*val = inbuf;

    return 0;
}