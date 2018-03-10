CFLAGS= -Wall -Werror
LDFLAGS= -pthread 
CC=arm-linux-gnueabihf-gcc
OBJECTS=APDS9301.o
TARGET=5013P1

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -pthread -o $@

clean:
	rm ./$(TARGET) *.o
