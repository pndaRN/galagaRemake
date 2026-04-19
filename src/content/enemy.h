#ifndef ENEMY_H
#define ENEMY_H
#define MAX_ENEMIES 250

#include "bacteria.h"
#include "procedural.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <stdbool.h>

typedef enum {
  ENEMY_ENTERING,
  ENEMY_HOLDING,
  ENEMY_DIVING,
  ENEMY_RETURNING
} EnemyState;

typedef struct EnemyHot {
  float x, y;
  int width, height;
  bool active;
  int health;
  BacteriaSpecies species;
} EnemyHot;

typedef struct EnemyCold {
  EnemyState state;
  Uint64 state_start_time;
  float t, speed, speed_scalar;
  DiveState dive_state;
  bool dive_initialized;
  EntryPathData entry_path;
  int current_segment;
  SDL_FPoint formation_point;
} EnemyCold;

void enemy_init(EnemyHot *hot, EnemyCold *cold, float speed_scalar,
                EntryPathData path_data, SDL_FPoint formation_position,
                BacteriaSpecies species);
void enemy_update(EnemyHot *hot, EnemyCold *cold, float deltaTime,
                  int screen_height, int screen_width, float player_x);

#endif
