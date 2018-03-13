CFLAGS= -Wall -Werror
LDFLAGS=-pthread -lm
CC=gcc
#arm-linux-gnueabihf-gcc
OBJECTS=main.o i2c_driver.o ADPS9301.o TMP102.o
TARGET=5013P1

all: $(TARGET)
	
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

clean:
	rm ./$(TARGET) *.o
