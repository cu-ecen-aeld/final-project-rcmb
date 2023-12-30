CC ?= gcc
CFLAGS = -Wall -Wextra -li2c

SRC_FILES = test.c 

.PHONY: custom clean

custom: $(SRC_FILES)
		$(CC) $(CFLAGS) -o $@ $^

clean:
		-rm -f custom

