#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include "i2c_driver.h"
#include "ADPS9301.h"

int ADPS9301_get_light(int addr, char* interface, uint16_t* value)
{
    unsigned char lobyte, hibyte;

    int i2c_file = i2c_open(interface);

    //get low byte
    if(get_i2c_register_repstart(i2c_file, addr, APDS9300_CMD | APDS9300_DATA0LOW, &lobyte)) {
        return i2c_close(i2c_file)-2; // ret -2 if unable to get register, -3 if close fails also
    }
    //get high byte
    if(get_i2c_register_repstart(i2c_file, addr, APDS9300_CMD | APDS9300_DATA0HIGH, &hibyte)) {
        return i2c_close(i2c_file)-4; //ret -4 if unable to get register, -5 if close fails also
    }

	*value = (hibyte << 8) | lobyte;
    return i2c_close(i2c_file); 
}

//device must be powered on before reading
int ADPS9301_power_on(int addr, char* interface)
{
	int i2c_file = i2c_open(interface);

    if(set_i2c_register(i2c_file, addr, APDS9300_CMD | APDS9300_CONTROL, APDS9300_POWER_ON)) {
        return (i2c_close(i2c_file)-2); //can't get register, return -2, can't get register AND close fails, return -3
    }
    else 
    {
        return i2c_close(i2c_file); //return 0, return -1 if close fails
    }

    return 0;
}

