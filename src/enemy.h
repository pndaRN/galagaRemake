#ifndef ENEMY_H
#define ENEMY_H
#define MAX_ENEMIES 20

#include "bacteria.h"
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
  float x, y, speed;
  int width, height;
  bool active;
  Uint64 state_start_time;
  EnemyState state;

  BacteriaSpecies species;
  DiveState dive_state;
  bool dive_initialized;

  SDL_FPoint control_points[4];
  SDL_FPoint formation_point;

  float t;

} Enemy;

Enemy enemy_init(SDL_FPoint p0, SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3,
                 float speed, SDL_FPoint formation_position,
                 BacteriaSpecies species);

void enemy_update(Enemy *e, float deltaTime, int screen_height, float player_x);

#endif
