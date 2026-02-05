CC = gcc
CFLAGS = -Wall -Wextra -g -Isrc
LDFLAGS = -lSDL2 -lm

SRC = src/main.c src/player.c src/bullet.c src/enemy.c src/collision.c src/math_utils.c src/wave.c src/bacteria.c
OUT = game

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)
