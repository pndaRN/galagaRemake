#ifndef ENEMY_H
#define ENEMY_H

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

typedef struct {
  float x, y, speed;
  int width, height;
  bool active;
  EnemyState state;

  SDL_FPoint control_points[4];
  SDL_FPoint formation_point;

  float t;

} Enemy;

Enemy enemy_init(SDL_FPoint p0, SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3,
                 float speed, SDL_FPoint formation_position);

void enemy_update(Enemy *e, float deltaTime, int screen_height);

#endif
