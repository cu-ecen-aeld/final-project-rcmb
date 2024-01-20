CC ?= gcc
CFLAGS = -Wall -Wextra 

SRC_FILES = sensor_test.c 

.PHONY: sensor_test clean

sensor_test: $(SRC_FILES)
		$(CC) $(CFLAGS) -o $@ $^ -li2c

clean:
		-rm -f sensor_test


