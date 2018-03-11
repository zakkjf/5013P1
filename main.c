#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ADPS9301.h"
#include "TMP102.h"

int main()
{
	int i;
	int err = 0;

	double lux;
	float temp;

	err+=APDS9301_power_on(0x39,"/dev/i2c-1");

	for(i=0;i<20;i++)
	{
		err+=APDS9301_get_lux(0x39,"/dev/i2c-1", &lux);
		err+=TMP102_get_temp_c(0x48,"/dev/i2c-1", &temp);

		printf("Light sensor value %d: %.8lf\n", i, lux);
		printf("Temp sensor value %d: %.4f\n", i, temp);
		sleep(1);
	}

	return 0;
}