#ifndef WAVE
#define WAVE

#include "enemy.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <stdbool.h>

typedef struct {
  int total_enemies;
  int spawn_count;
  float speed;
  float spawn_delay;
  float spawn_timer;

  SDL_FPoint control_points[4];
  SDL_FPoint *formation_positions;

  bool is_active;
} Wave;

Wave wave_init(int total_enemies, float speed, SDL_FPoint p0, SDL_FPoint p1,
               SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint *formation_positions);

void wave_update(Wave *w, float deltaTime, Enemy *e, int max_enemies);

#endif
