SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
CC = gcc

CFLAGS = -Wall -lpthread -g -Wunused-function -Wunused-variable -Wint-conversion

pokeprogram.out: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
		rm -f $(OBJ) pokeprogram