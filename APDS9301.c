#include <linux/i2c.h>
#include <stdio.h>

int main()
{
	char * msg;
	*msg = 0x8A;
	printf("Sending %#04x\n", *msg);
	i2c_master_send(APDS9301_client, msg, 1);
	i2c_master_recv(APDS9301_client, msg, 1);
	printf("Read back %#04x\n", *msg);
	return 0;
}
