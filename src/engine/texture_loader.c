#include "texture_loader.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>

SDL_Texture *texture_load(SDL_Renderer *renderer, const char *path) {
  if (!path) {
    fprintf(stderr, "texture_load called with NULL path \n");
    return NULL;
  }
  SDL_Surface *surface = IMG_Load(path);
  if (!surface) {
    fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\n", path,
            IMG_GetError());
    return NULL;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  if (!texture) {
    fprintf(stderr, "Unable to create texture from %s! SDL Error: %s\n", path,
            SDL_GetError());
  }
  return texture;
}
