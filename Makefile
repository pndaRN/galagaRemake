CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lSDL2

SRC = src/main.c
OUT = game

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)
