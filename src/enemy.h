#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>

typedef enum {
  ENEMY_ENTERING,
  ENEMY_HOLDING,
  ENEMY_DIVING,
  ENEMY_RETURNING
} EnemyState;

typedef struct {
  float x, y, speed;
  int width, height;
  bool active;
  EnemyState state;
  int currentWaypoint;
  SDL_FPoint path[10];
  int pathLength;
} Enemy;

Enemy enemy_init(SDL_FPoint *path, int pathLength, float speed);

void enemy_update(Enemy *e, float deltaTime, int screen_height);

#endif
