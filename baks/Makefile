CFLAGS= -Wall -Werror -g
LDFLAGS=-pthread 
CC=gcc
#arm-linux-gnueabihf-gcc
OBJECTS=main.o
TARGET=5013HW3

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -pthread -o $@

clean:
	rm ./$(TARGET) *.o
