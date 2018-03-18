#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include "i2c_driver.h"
#include "TMP102.h"

static int TMP102_get_raw_temp(int addr, char* interface, uint16_t* value)
{
    unsigned char bytes[2];

    int i2c_file = i2c_open(interface);

    //get bytes
    if(get_i2c_registers(i2c_file, addr, TMP102_TEMP_REG, 2, bytes)) {
        return i2c_close(i2c_file)-2; // ret -2 if unable to get register, -3 if close fails also
    }

	*value = (bytes[0] << 4) | (bytes[1] >> 4);
    return i2c_close(i2c_file); //ret 0 if all good, -1 if unable to close
}

int TMP102_get_temp_c(int addr, char* interface, float* value)
{
    uint16_t temp_raw;
    int ret = TMP102_get_raw_temp(addr, interface, &temp_raw);

    *value = ((float)(temp_raw))/16; //divide by 16, as LSB resolution of sensor is (0.0625 = 2^-4 = 1/16) deg C 
    return ret;
}

int TMP102_get_temp_f(int addr, char* interface, float* value)
{
    float temp_cel;
    int ret = TMP102_get_temp_c(addr, interface, &temp_cel);

    *value = (temp_cel * 1.8) + 32.0; //conversion from celsius to fahrenheit;

    return ret;
}


int TMP102_get_temp_k(int addr, char* interface, float* value)
{
    float temp_cel;
    int ret = TMP102_get_temp_c(addr, interface, &temp_cel);

    *value = temp_cel + 273.15; //conversion from celsius to kelvin;

    return ret;
}

int TMP102_get_tlow(int addr, char* interface, uint8_t* value)
{
    unsigned char byte;

    int i2c_file = i2c_open(interface);

    //get bytes
    if(get_i2c_registers(i2c_file, addr, TMP102_T_LO_REG, 1, &byte)) {
        return i2c_close(i2c_file)-2; // ret -2 if unable to get register, -3 if close fails also
    }

    *value = byte;
    return i2c_close(i2c_file); //ret 0 if all good, -1 if unable to close
}

int TMP102_get_thigh(int addr, char* interface, uint8_t* value)
{
    unsigned char byte;

    int i2c_file = i2c_open(interface);

    //get bytes
    if(get_i2c_registers(i2c_file, addr, TMP102_T_HI_REG, 2, &byte)) {
        return i2c_close(i2c_file)-2; // ret -2 if unable to get register, -3 if close fails also
    }

    *value = byte;
    return i2c_close(i2c_file); //ret 0 if all good, -1 if unable to close
}


int TMP102_get_config(int addr, char* interface, uint16_t* value)
{
    unsigned char byte;

    int i2c_file = i2c_open(interface);

    //get bytes
    if(get_i2c_registers(i2c_file, addr, TMP102_CONF_REG, 1, &byte)) {
        return i2c_close(i2c_file)-2; // ret -2 if unable to get register, -3 if close fails also
    }

    *value = byte;
    return i2c_close(i2c_file); //ret 0 if all good, -1 if unable to close
}

int TMP102_set_config(int addr, char* interface, uint16_t value)
{
    unsigned char bytes[2];

    bytes[0] = (unsigned char) value;
    bytes[1] = (unsigned char) (value >> 8);

    int i2c_file = i2c_open(interface);

    //get bytes
    if(set_i2c_registers(i2c_file, addr, TMP102_CONF_REG, bytes[0], bytes[1]))
    {
        return i2c_close(i2c_file)-2; // ret -2 if unable to get register, -3 if close fails also
    }

    return i2c_close(i2c_file); //ret 0 if all good, -1 if unable to close
}


int TMP102_shutdown_on(int addr, char* interface)
{
    uint16_t val;

    TMP102_get_config(addr, interface, &val);
    TMP102_set_config(addr, interface, val|TMP102_CONF_SHUTDOWN);
    
    return 0;
}

int TMP102_shutdown_off(int addr, char* interface)
{
    uint16_t val;

    TMP102_get_config(addr, interface, &val);
    TMP102_set_config(addr, interface, val|(~TMP102_CONF_SHUTDOWN));

    return 0;
}

int TMP102_EM_on(int addr, char* interface)
{
    uint16_t val;

    TMP102_get_config(addr, interface, &val);
    TMP102_set_config(addr, interface, val|TMP102_CONF_EM);

    return 0;
}

 int TMP102_EM_off(int addr, char* interface)
{
    uint16_t val;

    TMP102_get_config(addr, interface, &val);
    TMP102_set_config(addr, interface, val|(~TMP102_CONF_EM));

    return 0;
}

int TMP102_set_res(int addr, char* interface, char res)
{
    uint16_t val;

    TMP102_get_config(addr, interface, &val);
    TMP102_set_config(addr, interface, val|(TMP102_CONF_RES|res << 5));

    return 0;
}

int TMP102_set_conv(int addr, char* interface, char conv)
{
    uint16_t val;

    TMP102_get_config(addr, interface, &val);
    TMP102_set_config(addr, interface, val|(TMP102_CONF_CONVRATE|(conv << 14)));

    return 0;
}

int TMP102_set_fault(int addr, char* interface, char fault)
{
    uint16_t val;

    TMP102_get_config(addr, interface, &val);
    TMP102_set_config(addr, interface, val|(TMP102_CONF_FAULT|(fault << 3)));

    return 0;
}


