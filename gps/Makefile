CFLAGS= -Wall -Werror -g
LDFLAGS=-pthread -lm
CC=gcc
#arm-linux-gnueabihf-gcc
OBJECTS=test.o gps_decoder.o gps_calc.o gps_dist.o
TARGET=P2_GPS_UNIT_TESTS

all: $(TARGET)
	
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

clean:
	rm ./$(TARGET) *.o
