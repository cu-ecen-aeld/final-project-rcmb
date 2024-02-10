CC ?= gcc
CFLAGS    = -Wall -Wextra  -lm
I2CFLAGS  = -li2c
MATHFLAGS = -lm
SERVER_SRC_FILES = sensor_server.c cJSON.c sensor_fx.c 
SENSOR_SRC_FILES = sensor_test.c

.PHONY: all sensor_server sensor_test clean

all: sensor_server sensor_test

sensor_server: $(SERVER_SRC_FILES) 
		$(CC) $(CFLAGS) -o $@ $^ $(I2CFLAGS) $(MATHFLAGS)

sensor_test: $(SENSOR_SRC_FILES)
		$(CC) $(CFLAGS) -o $@ $^ $(I2CFLAGS)

clean:
		-rm -f sensor_server sensor_test



