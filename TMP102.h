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

int TMP102_get_temp_c(int addr, char* interface, float* value);

int TMP102_get_temp_f(int addr, char* interface, float* value);


#endif

