#include "enemy.h"
#include <SDL2/SDL_rect.h>
#include <math.h>
// #include <stdlib.h>

Enemy enemy_init(SDL_FPoint *path, int pathLength, float speed) {
  printf("Path received:\n");
  for (int i = 0; i < pathLength; i++) {
    printf("  path[%d] = (%.1f, %.1f)\n", i, path[i].x, path[i].y);
  }
  Enemy e;

  e.width = 50;
  e.height = 50;
  e.speed = speed;
  e.active = true;

  e.state = ENEMY_ENTERING;
  e.currentWaypoint = 1;

  e.pathLength = pathLength;
  for (int i = 0; i < pathLength; i++) {
    e.path[i] = path[i];
  }

  e.x = path[0].x;
  e.y = path[0].y;

  printf("Enemy initialized at (%.1f, %.1f)\n", e.x, e.y);

  return e;
}

void enemy_update(Enemy *e, float deltaTime, int screen_height) {
  if (!e->active)
    return;

  static int frame = 0;
  printf("Frame %d: Enemy at (%.1f, %.1f) state=%d waypoint=%d\n", frame++,
         e->x, e->y, e->state, e->currentWaypoint);

  switch (e->state) {
  case ENEMY_ENTERING: {
    SDL_FPoint target = e->path[e->currentWaypoint];

    float dx = target.x - e->x;
    float dy = target.y - e->y;

    float length = sqrt(dx * dx + dy * dy);

    if (length < 1.0f) {
      e->currentWaypoint++;
      if (e->currentWaypoint >= e->pathLength) {
        e->state = ENEMY_HOLDING;
      }
    } else {
      dx = dx / length;
      dy = dy / length;
      e->x += dx * e->speed * deltaTime;
      e->y += dy * e->speed * deltaTime;
    }
    break;
  }

  case ENEMY_HOLDING:
    break;

  case ENEMY_DIVING:
    break;

  case ENEMY_RETURNING:
    break;
  }
}
