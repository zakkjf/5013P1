#ifndef _I2C_DRIVER_H_
#define _I2C_DRIVER_H_

#include <stdlib.h>

#define I2C_FILE_NAME "/dev/i2c-0"
#define USAGE_MESSAGE \
    "Usage:\n" \
    "  %s r [addr] [register]   " \
        "to read value from [register]\n" \
    "  %s w [addr] [register] [value]   " \
        "to write a value [value] to register [register]\n" \
    ""

int i2c_open(char* filename);

int i2c_close(int i2c_file);

int set_i2c_register(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char value);

int get_i2c_register_repstart(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char *val);

int get_i2c_registers(int file,
                    unsigned char addr,
                    unsigned char reg,
                    unsigned char count,
                    unsigned char *val);

#endif
