#ifndef ENEMY_H
#define ENEMY_H
#define MAX_ENEMIES 20

#include "bacteria.h"
#include "procedural.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <math.h>
#include <stdbool.h>

typedef enum {
  ENEMY_ENTERING,
  ENEMY_HOLDING,
  ENEMY_DIVING,
  ENEMY_RETURNING
} EnemyState;


typedef struct Enemy {
  float x, y, speed, speed_scalar;
  int width, height, health;
  bool active;
  Uint64 state_start_time;
  EnemyState state;

  BacteriaSpecies species;
  DiveState dive_state;
  bool dive_initialized;

  EntryPathData entry_path;
  int current_segment;

  SDL_FPoint formation_point;

  float t;
  int screen_height, screen_width;
} Enemy;

Enemy enemy_init(float speed_scalar, EntryPathData path_data, SDL_FPoint formation_position,
                 BacteriaSpecies species, int screen_height, int screen_width);

void enemy_update(Enemy *e, float deltaTime, int screen_height, float player_x);

#endif
