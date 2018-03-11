#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <math.h>
#include "i2c_driver.h"
#include "ADPS9301.h"

static int ADPS9301_get_CH(int addr, char* interface, uint16_t* value, unsigned char ch_lo, unsigned char ch_hi)
{
    unsigned char lobyte, hibyte;

    int i2c_file = i2c_open(interface);

    //get low byte
    if(get_i2c_register_repstart(i2c_file, addr, APDS9301_CMD | ch_lo, &lobyte)) {
        return i2c_close(i2c_file)-2; // ret -2 if unable to get register, -3 if close fails also
    }
    //get high byte
    if(get_i2c_register_repstart(i2c_file, addr, APDS9301_CMD | ch_hi, &hibyte)) {
        return i2c_close(i2c_file)-4; //ret -4 if unable to get register, -5 if close fails also
    }

	*value = (hibyte << 8) | lobyte;
    return i2c_close(i2c_file); 
}

int APDS9301_get_CH0(int addr, char* interface, uint16_t* value)
{
    return ADPS9301_get_CH(addr, interface, value, APDS9301_CH0LOW, APDS9301_CH0HIGH);
}

int APDS9301_get_CH1(int addr, char* interface, uint16_t* value)
{
    return ADPS9301_get_CH(addr, interface, value, APDS9301_CH1LOW, APDS9301_CH1HIGH);
}

int APDS9301_get_lux(int addr, char* interface, double* value)
{
    uint16_t CH0_int, CH1_int;
    double ratio, CH0_d, CH1_d;
    int ret = 0;
    
    ret += APDS9301_get_CH0(addr, interface, &CH0_int);
    ret += APDS9301_get_CH1(addr, interface, &CH1_int);

    //begin conversion
    CH0_d = (double)CH0_int;
    CH1_d = (double)CH0_int;
    ratio = CH1_d/CH0_d;

    if(ratio > 0 && ratio <= 0.5)
    {
        *value = (0.0304 * CH0_d) - (0.062 * CH0_d * pow(ratio, 1.4));
    }
    else if(ratio > 0.5 && ratio <= 0.61)
    {
        *value = (0.0224 * CH0_d) - (0.031 * CH1_d);
    }
    else if(ratio > 0.61 && ratio <= 0.8)
    {
        *value = (0.0128 * CH0_d) - (0.0153 * CH1_d);
    }
    else if(ratio > 0.8 && ratio <= 1.3)
    {
        *value = (0.00146 * CH0_d) - (0.00112 * CH1_d);
    }
    else if(ratio > 1.3)
    {
        *value = 0;
    }
    else
    {
        return -7; //conversion error
    }

    return ret;
}


//device must be powered on before reading
int APDS9301_power_on(int addr, char* interface)
{
	int i2c_file = i2c_open(interface);

    if(set_i2c_register(i2c_file, addr, APDS9301_CMD | APDS9301_CONTROL, APDS9301_POWER_ON)) {
        return (i2c_close(i2c_file)-2); //can't get register, return -2, can't get register AND close fails, return -3
    }
    else 
    {
        return i2c_close(i2c_file); //return 0, return -1 if close fails
    }

    return 0;
}

