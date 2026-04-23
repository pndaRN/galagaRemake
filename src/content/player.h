#ifndef PLAYER_H
#define PLAYER_H
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "weapon.h"

typedef struct {
  float x, y;
  int width, height;
  int hb_offset_x, hb_offset_y;
  int hb_width, hb_height;
  float speed;
  bool active;
  WeaponType current_ammo;
} Player;

Player player_create(int screen_width, int screen_height);
void player_update(Player *p, const Uint8 *keystate, float deltaTime,
                   int screenWidth);

#endif
