#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

SDL_Texture *texture_load(SDL_Renderer *renderer, const char *path);

#endif
