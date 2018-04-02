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
​ ​*​ ​@file​ ​driver_unit_test.c
​ ​*​ ​@brief​ ​a simple driver unit test demonstrating working sensor drivers
​ ​*
​ ​*​ ​This​ ​is the project 1 i2c sensor driver
​ ​*
​ ​*​ ​@author​ ​Zach Farmer
​ ​*​ ​@date​ ​Mar 16 2018
​ ​*
​ ​*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ADPS9301.h"
#include "TMP102.h"

int main()
{
	int i=0;
	int err = 0;

	double lux;
	float tempc, tempf;

	err+=ADPS9301_power_on(0x39,"/dev/i2c-1");

	while(1)
	{
		i++; //counter
		err+=ADPS9301_get_lux(0x39,"/dev/i2c-1", &lux);
		err+=TMP102_get_temp_c(0x48,"/dev/i2c-1", &tempc);
		err+=TMP102_get_temp_f(0x48,"/dev/i2c-1", &tempf);


		printf("Light sensor value %d: %.5lf lux\n", i, lux);
		printf("Temp sensor value %d: %.4f degrees Celcius \n", i, tempc);
		printf("Temp sensor value %d: %.4f degrees Fahrenheit \n", i, tempf);

		sleep(1);
	}

	return 0;
}
