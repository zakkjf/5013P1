obj-m = chip_i2c.o
KDIR := /home/razor2/dev/bbb/linux2/linux
all:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
