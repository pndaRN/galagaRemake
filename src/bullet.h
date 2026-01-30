#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "player.h"

typedef struct {
  float x, y, speed;
  int width, height;
  bool active;
  AmmoType type;
} Bullet;

Bullet bullet_init(Player *player);

void bullet_update(Bullet *b, float deltaTime);

#endif
