#ifndef PLAYER_H
#define PLAYER_H
#include <SDL2/SDL.h>

typedef enum {
  AMMO_PCN,
  AMMO_POLYMYXIN
} AmmoType;

typedef struct {
  float x, y;
  int width;
  int height;
  float speed;
  AmmoType current_ammo;
} Player;

Player player_create(int screen_width, int screen_height);

void player_update(Player *p, const Uint8 *keystate, float deltaTime,
  int screenWidth);

#endif
