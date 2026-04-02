#ifndef WAVE
#define WAVE

#include "enemy.h"
#include "procedural.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <stdbool.h>

typedef struct {
  int total_enemies;
  int spawn_count, species_unlocked;
  float threshold;
  float spawn_delay, spawn_timer;
  float dive_delay, dive_timer;
  float speed_scalar;

  int *enemy_indices;

  bool is_active, formation_complete, threshold_crossed;
  Uint64 formation_complete_time;

  SDL_FPoint control_points[3];
  SDL_FPoint *formation_positions;

  int screen_height, screen_width;
} Wave;

Wave wave_init(WaveParams *wp, SDL_FPoint p0, SDL_FPoint p1, SDL_FPoint p2,
               SDL_FPoint *formation_positions, int screen_height,
               int screen_width);

void wave_update(Wave *w, float deltaTime, Enemy *e, int max_enemies);

void wave_free(Wave *w);

#endif
