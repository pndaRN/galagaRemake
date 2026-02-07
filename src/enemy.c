#include "enemy.h"
#include "bacteria.h"
#include "math_utils.h"

#include <SDL2/SDL_rect.h>
#include <math.h>
// #include <stdlib.h>

Enemy enemy_init(SDL_FPoint p0, SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3,
                 float speed, SDL_FPoint formation_position,
                 BacteriaSpecies species, int screen_height, int screen_width) {
  Enemy e;

  e.width = 50;
  e.height = 50;
  e.speed = speed;
  e.active = true;

  e.state_start_time = 0;

  e.state = ENEMY_ENTERING;

  e.control_points[0] = p0;
  e.control_points[1] = p1;
  e.control_points[2] = p2;
  e.control_points[3] = p3;

  e.t = 0.0f;
  e.x = p0.x;
  e.y = p0.y;

  e.formation_point = formation_position;

  e.dive_initialized = false;
  e.species = species;

  e.screen_height = screen_height;
  e.screen_width = screen_width;

  return e;
}

void enemy_update(Enemy *e, float deltaTime, int screen_height,
                  float player_x) {
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
      e->state_start_time = SDL_GetTicks64();
    }

    break;
  }

  case ENEMY_HOLDING:
    float baseX = e->formation_point.x;
    float baseY = e->formation_point.y;

    Uint64 elapsed = SDL_GetTicks64() - e->state_start_time;
    float timeFactor = (elapsed / 2000.0f) * (2.0f * M_PI);

    float ramp = fminf(elapsed / 1000.0f, 1.0f);
    float yOffset = sinf(timeFactor) * 20.0f * ramp;

    e->x = baseX;
    e->y = baseY + yOffset;
    break;

  case ENEMY_DIVING:
    const BacteriaDefinition *def = get_bacteria_def(e->species);

    if (!e->dive_initialized) {
      def->dive_init(e, player_x);
      e->dive_initialized = true;
    }

    def->dive_update(e, deltaTime, screen_height, player_x);
    break;

  case ENEMY_RETURNING:
    if (!e->dive_initialized) {
      e->control_points[0].x = e->x;
      e->control_points[0].y = 0 - e->height;
      e->control_points[3].x = e->formation_point.x;
      e->control_points[3].y = e->formation_point.y;

      float dx = e->control_points[3].x - e->control_points[0].x;
      float dy = e->control_points[3].y - e->control_points[0].y;

      e->control_points[1].x = e->control_points[0].x + dx * (1.0f / 3.0f);
      e->control_points[1].y = e->control_points[0].y + dy * (1.0f / 3.0f);

      e->control_points[2].x = e->control_points[0].x + dx * (2.0f / 3.0f);
      e->control_points[2].y = e->control_points[0].y + dy * (2.0f / 3.0f);

      float offset = (e->x > e->screen_width / 2.0f) ? -150.0f : 150.0f;
      e->control_points[1].x += offset;
      e->control_points[2].x += offset;

      e->t = 0.0f;
      e->dive_initialized = true;
    }
    e->t += deltaTime / 2.0f;
    SDL_FPoint pos =
        bezier_point(e->control_points[0], e->control_points[1],
                     e->control_points[2], e->control_points[3], e->t);

    e->x = pos.x;
    e->y = pos.y;
    if (e->t >= 1.0f) {
      e->t = 1.0f; // clamp it
      e->state = ENEMY_HOLDING;
      e->state_start_time = SDL_GetTicks64();
      e->dive_initialized = false;
    }
  }
}
