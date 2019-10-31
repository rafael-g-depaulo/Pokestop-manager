SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
CC = gcc

CFLAGS = -Wall -lpthread

pokeprogram: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
		rm -f $(OBJ) pokeprogram