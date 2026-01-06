CC = gcc
CFLAGS = -Wall -Wextra -g -Isrc
LDFLAGS = -lSDL2

SRC = src/main.c src/player.c src/bullet.c src/enemy.c
OUT = game

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)
