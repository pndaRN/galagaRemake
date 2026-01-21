#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>

SDL_FPoint bezier_point(SDL_FPoint p0, SDL_FPoint p1, SDL_FPoint p2,
                        SDL_FPoint p3, float t);

#endif
