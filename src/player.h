#ifndef PLAYER_H
#define PLAYER_H
#include <SDL2/SDL.h>

typedef struct {
  float x, y;
  int width;
  int height;
  float speed;
} Player;

Player player_create(int screen_width, int screen_height);

void player_update(Player *p, const Uint8 *keystate, float deltaTime,
  int screenWidth);

#endif
