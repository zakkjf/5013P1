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

	uint16_t lightval, tempval;
	float temp;

	err+=ADPS9301_power_on(0x39,"/dev/i2c-1");

	for(i=0;i<20;i++)
	{
		err+=ADPS9301_get_light(0x39,"/dev/i2c-1", &lightval);
		err+=TMP102_get_temp(0x48,"/dev/i2c-1", &tempval);

		temp = ((double)(tempval))/16;
		printf("Light sensor value %d: %d\n", i, lightval);
		printf("Temp sensor value %d: %.4f\n", i, temp);
		sleep(1);
	}

	return 0;
}