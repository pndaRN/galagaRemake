#ifndef ASSETS_H
#define ASSETS_H

#include "bacteria.h"
#include "shared_types.h"
#include <SDL2/SDL.h>

typedef struct {
  SDL_Texture *ships[WEAPON_COUNT];
  SDL_Texture *bullets[WEAPON_COUNT];
  SDL_Texture *bacteria[SPECIES_COUNT];
} Assets;

void assets_init(Assets *a, SDL_Renderer *renderer);
void assets_destroy(Assets *assets);

#endif
