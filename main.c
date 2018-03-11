#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ADPS9301.h"

int main()
{
	int i;
	int err = 0;

	uint16_t value;

	err+=ADPS9301_power_on(0x39,"/dev/i2c-1");

	for(i=0;i<20;i++)
	{
		err+=ADPS9301_get_light(0x39,"/dev/i2c-1", &value);
		printf("Light sensor value %d: %d\n", i, value);
		sleep(1);
	}

	return 0;
}