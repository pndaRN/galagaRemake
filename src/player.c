#include "player.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>

Player player_create(int screen_width, int screen_height,
                     SDL_Renderer *renderer) {
  Player p;

  p.width = 64;
  p.height = 128;
  p.x = (screen_width / 2.0f) - (p.width / 2.0f);
  p.y = screen_height - p.height - 20;
  p.hb_height = 65;
  p.hb_width = 22;
  p.hb_offset_x = (p.width - p.hb_width) / 2.0f;
  p.hb_offset_y = (p.height - p.hb_height) / 2.0f + 10.0f;
  p.speed = 300;

  p.current_ammo = WEAPON_NEUTRAL;
  p.active = true;

  const char *ship_paths[WEAPON_COUNT] = {"assets/ships/ship_neutral.png",
                                          "assets/ships/ship_pcn.png",
                                          "assets/ships/ship_pmx.png"};

  for (int i = 0; i < WEAPON_COUNT; i++) {
    SDL_Surface *surface = IMG_Load(ship_paths[i]);
    if (!surface) {
      printf("Failed to load image! IMG_Error: %s\n", IMG_GetError());
      p.ship_texture[i] = NULL;
    } else {
      p.ship_texture[i] = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_FreeSurface(surface);
    }
    surface = IMG_Load(get_weapon_def(i)->texture_path);
    if (!surface) {
      printf("Failed to load image! IMG_Error: %s\n", IMG_GetError());
      p.bullet_texture[i] = NULL;
    } else {
      p.bullet_texture[i] = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_FreeSurface(surface);
    }
  }
  return p;
}

void player_update(Player *p, const Uint8 *keystate, float deltaTime,
                   int screenWidth) {
  if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]) {
    p->x -= p->speed * deltaTime;
  }
  if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]) {
    p->x += p->speed * deltaTime;
  }

  if (p->x < 0) {
    p->x = 0;
  }
  if (p->x + p->width > screenWidth) {
    p->x = screenWidth - p->width;
  }
}
