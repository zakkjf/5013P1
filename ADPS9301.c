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

static int ADPS9301_get_CH(int addr, char* interface, int16_t* value, unsigned char ch_lo, unsigned char ch_hi)
{
    unsigned char lobyte, hibyte;

    int i2c_file = i2c_open(interface);

    //get low byte
    if(get_i2c_register_repstart(i2c_file, addr, ADPS9301_CMD | ch_lo, &lobyte)) {
        return i2c_close(i2c_file)-2; // ret -2 if unable to get register, -3 if close fails also
    }
    //get high byte
    if(get_i2c_register_repstart(i2c_file, addr, ADPS9301_CMD | ch_hi, &hibyte)) {
        return i2c_close(i2c_file)-4; //ret -4 if unable to get register, -5 if close fails also
    }

	*value = (hibyte << 8) | lobyte;
    return i2c_close(i2c_file); 
}

int ADPS9301_get_CH0(int addr, char* interface, int16_t* value)
{
    return ADPS9301_get_CH(addr, interface, value, ADPS9301_CH0LOW, ADPS9301_CH0HIGH);
}

int ADPS9301_get_CH1(int addr, char* interface, int16_t* value)
{
    return ADPS9301_get_CH(addr, interface, value, ADPS9301_CH1LOW, ADPS9301_CH1HIGH);
}

int ADPS9301_get_lux(int addr, char* interface, double* value)
{
    int16_t CH0_int, CH1_int;
    double ratio, CH0_d, CH1_d;
    int ret = 0;
    
    ret += ADPS9301_get_CH0(addr, interface, &CH0_int);
    ret += ADPS9301_get_CH1(addr, interface, &CH1_int);

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

int ADPS9301_run_everything(int addr, char* interface)
{
    uint8_t val;
    uint8_t testwrite = 0;
    uint16_t low, high;
    ADPS9301_power_on(addr, interface);
    ADPS9301_get_timing(addr, interface, &val);
    ADPS9301_set_timing(addr, interface, val);
    ADPS9301_set_low_gain(addr, interface);
    ADPS9301_set_high_gain(addr, interface);
    ADPS9301_set_integ_time(addr, interface, testwrite);
    ADPS9301_disable_int(addr, interface);
    ADPS9301_enable_int(addr, interface);
    ADPS9301_get_id(addr, interface, &val);
    ADPS9301_get_thresholds(addr, interface, &low, &high);
    ADPS9301_set_thresholds(addr, interface, low, high);
    return 0;
}

//device must be powered on before reading
int ADPS9301_power_on(int addr, char* interface)
{
    ADPS9301_write_single(addr, interface, ADPS9301_CONTROL, ADPS9301_POWER_ON);
    return 0;
}

//device must be powered on before reading
int ADPS9301_set_timing(int addr, char* interface, uint8_t msg)
{
    ADPS9301_write_single(addr, interface, ADPS9301_TIMING, msg);
    return 0;
}

int ADPS9301_get_timing(int addr, char* interface, uint8_t* value)
{
    ADPS9301_read_single(addr, interface, ADPS9301_TIMING, value);
    return 0;
}

int ADPS9301_set_high_gain(int addr, char* interface)
{
    uint8_t snd;
    ADPS9301_get_timing(addr,interface,&snd);
    ADPS9301_set_timing(addr,interface,snd|ADPS9301_GAIN_MASK);
    return 0;
}

int ADPS9301_set_low_gain(int addr, char* interface)
{
    uint8_t snd;
    ADPS9301_get_timing(addr,interface,&snd);
    ADPS9301_set_timing(addr,interface,snd|(~ADPS9301_GAIN_MASK));
    return 0;
}

int ADPS9301_set_integ_time(int addr, char* interface, uint8_t msg)
{
    uint8_t snd;
    ADPS9301_get_timing(addr,interface,&snd);
    ADPS9301_set_timing(addr,interface,snd|(ADPS9301_TIMING_MASK|msg));
    return 0;
}

int ADPS9301_enable_int(int addr, char* interface)
{
    uint8_t snd;
    ADPS9301_get_timing(addr,interface,&snd);
    ADPS9301_set_timing(addr,interface,snd|ADPS9301_INTR_MASK);
    return 0;
}

int ADPS9301_disable_int(int addr, char* interface)
{
    uint8_t snd;
    ADPS9301_get_timing(addr,interface,&snd);
    ADPS9301_set_timing(addr,interface,snd|(~ADPS9301_INTR_MASK));
    return 0;
}

int ADPS9301_get_id(int addr, char* interface, uint8_t* value)
{
    ADPS9301_read_single(addr,interface,ADPS9301_ID, value);
    return 0;
}


//device must be powered on before reading
int ADPS9301_set_thresholds(int addr, char* interface, uint16_t low, uint16_t high)
{
    ADPS9301_write_single(addr, interface, ADPS9301_THRESHLOWLOW, (uint8_t)(low));
    ADPS9301_write_single(addr, interface, ADPS9301_THRESHLOWHIGH, (uint8_t)(low >> 8));
    ADPS9301_write_single(addr, interface, ADPS9301_THRESHHIGHLOW, (uint8_t)(high));
    ADPS9301_write_single(addr, interface, ADPS9301_THRESHHIGHHIGH, (uint8_t)(high >> 8));

    return 0;
}

int ADPS9301_get_thresholds(int addr, char* interface, uint16_t* low, uint16_t* high)
{
    uint8_t tll, tlh, thl, thh;
    ADPS9301_read_single(addr, interface, ADPS9301_THRESHLOWLOW,&tll);
    ADPS9301_read_single(addr, interface, ADPS9301_THRESHLOWHIGH,&tlh);
    ADPS9301_read_single(addr, interface, ADPS9301_THRESHHIGHLOW,&thl);
    ADPS9301_read_single(addr, interface, ADPS9301_THRESHHIGHHIGH,&thh);

    *low = (tlh << 8) | tll;
    *high = (thh << 8) | thl;

    return 0;
}

//device must be powered on before reading
int ADPS9301_write_single(int addr, char* interface, char reg, char msg)
{
    int i2c_file = i2c_open(interface);

    if(set_i2c_register(i2c_file, addr, ADPS9301_CMD | reg, msg)) {
        return (i2c_close(i2c_file)-2); //can't get register, return -2, can't get register AND close fails, return -3
    }
    else 
    {
        return i2c_close(i2c_file); //return 0, return -1 if close fails
    }

    return 0;
}

int ADPS9301_read_single(int addr, char* interface, char reg, uint8_t* value)
{
    int i2c_file = i2c_open(interface);

    //get low byte
    if(get_i2c_register_repstart(i2c_file, addr, ADPS9301_CMD | reg, value)) {
        return i2c_close(i2c_file)-2; // ret -2 if unable to get register, -3 if close fails also
    }

    return i2c_close(i2c_file); 
}