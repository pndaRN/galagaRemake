#include "enemy.h"
#include "math_utils.h"

#include <SDL2/SDL_rect.h>
#include <math.h>
// #include <stdlib.h>

Enemy enemy_init(SDL_FPoint p0, SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3,
                 float speed, SDL_FPoint formation_position, EnemyType type) {
  Enemy e;

  e.width = 50;
  e.height = 50;
  e.speed = speed;
  e.active = true;

  e.state = ENEMY_ENTERING;

  e.control_points[0] = p0;
  e.control_points[1] = p1;
  e.control_points[2] = p2;
  e.control_points[3] = p3;

  e.t = 0.0f;
  e.x = p0.x;
  e.y = p0.y;

  e.formation_point = formation_position;

  e.type = type;

  return e;
}

void enemy_update(Enemy *e, float deltaTime, int screen_height) {
  if (!e->active)
    return;

  switch (e->state) {
  case ENEMY_ENTERING: {

    e->t += deltaTime / 3.0f;
    SDL_FPoint pos =
        bezier_point(e->control_points[0], e->control_points[1],
                     e->control_points[2], e->control_points[3], e->t);

    e->x = pos.x;
    e->y = pos.y;

    if (e->t >= 1.0f) {
      e->t = 1.0f; // clamp it
      e->state = ENEMY_HOLDING;
    }

    break;
  }

  case ENEMY_HOLDING:
    float baseX = e->formation_point.x;
    float baseY = e->formation_point.y;

    float timeFactor = (SDL_GetTicks64() / 2000.0f) * (2.0f * M_PI);
    float yOffset = sinf(timeFactor) * 20.0f;

    e->x = baseX;
    e->y = baseY + yOffset;
    break;

  case ENEMY_DIVING:
    break;

  case ENEMY_RETURNING:
    break;
  }
}
