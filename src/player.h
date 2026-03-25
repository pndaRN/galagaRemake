#ifndef PLAYER_H
#define PLAYER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
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
  SDL_Texture *ship_texture[WEAPON_COUNT];
  SDL_Texture *bullet_texture[WEAPON_COUNT];
} Player;

Player player_create(int screen_width, int screen_height,
                     SDL_Renderer *renderer);
void player_update(Player *p, const Uint8 *keystate, float deltaTime,
                   int screenWidth);

#endif
