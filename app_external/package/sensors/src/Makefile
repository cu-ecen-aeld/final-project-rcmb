CC ?= gcc
CFLAGS = -Wall -Wextra 
I2CFLAGS = -li2c

SERVER_SRC_FILES = sensor_server.c cJSON.c 
SENSOR_SRC_FILES = sensor_test.c

.PHONY: all sensor_server sensor_test clean

all: sensor_server sensor_test

sensor_server: $(SERVER_SRC_FILES)
		$(CC) $(CFLAGS) -o $@ $^ 

sensor_test: $(SENSOR_SRC_FILES)
		$(CC) $(CFLAGS) -o $@ $^ $(I2CFLAGS)

clean:
		-rm -f sensor_server sensor_test



