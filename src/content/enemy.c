#include "enemy.h"
#include "bacteria.h"
#include "math_utils.h"
#include "procedural.h"

#include <SDL2/SDL_rect.h>
#include <math.h>
// #include <stdlib.h>

void enemy_init(EnemyHot *hot, EnemyCold *cold, float speed_scalar,
                EntryPathData path_data, SDL_FPoint formation_position,
                BacteriaSpecies species) {
  const BacteriaDefinition *bacteria_def = get_bacteria_def(species);

  hot->width = bacteria_def->width;
  hot->height = bacteria_def->height;
  hot->health = bacteria_def->health;
  hot->active = true;
  hot->species = species;
  hot->x = path_data.control_points[0].x;
  hot->y = path_data.control_points[0].y;

  cold->speed_scalar = speed_scalar;
  cold->speed = bacteria_def->base_speed * speed_scalar;
  cold->state_start_time = 0;
  cold->state = ENEMY_ENTERING;
  cold->t = 0.0f;
  cold->entry_path = path_data;
  cold->formation_point = formation_position;
  cold->dive_initialized = false;
}

void enemy_update(EnemyHot *hot, EnemyCold *cold, float deltaTime,
                  int screen_height, int screen_width, float player_x) {
  if (!hot->active) {
    return;
  }

  switch (cold->state) {
  case ENEMY_ENTERING: {

    cold->t += deltaTime / 3.0f;
    SDL_FPoint pos = bezier_point(cold->entry_path.control_points[0],
                                  cold->entry_path.control_points[1],
                                  cold->entry_path.control_points[2],
                                  cold->entry_path.control_points[3], cold->t);

    hot->x = pos.x;
    hot->y = pos.y;

    if (cold->t >= 1.0f) {
      cold->t = 1.0f; // clamp it
      cold->state = ENEMY_HOLDING;
      cold->state_start_time = SDL_GetTicks64();
    }

    break;
  }

  case ENEMY_HOLDING: {
    float baseX = cold->formation_point.x;
    float baseY = cold->formation_point.y;

    Uint64 elapsed = SDL_GetTicks64() - cold->state_start_time;
    float timeFactor = (elapsed / 2000.0f) * (2.0f * M_PI);

    float ramp = fminf(elapsed / 1000.0f, 1.0f);
    float yOffset = sinf(timeFactor) * 20.0f * ramp;

    hot->x = baseX;
    hot->y = baseY + yOffset;
    break;
  }

  case ENEMY_DIVING: {
    const BacteriaDefinition *def = get_bacteria_def(hot->species);

    if (!cold->dive_initialized) {
      def->dive_init(hot, cold, screen_height, screen_width, player_x);
      cold->dive_initialized = true;
    }

    def->dive_update(hot, cold, deltaTime, screen_height, screen_width,
                     player_x);
    break;
  }

  case ENEMY_RETURNING: {
    if (!cold->dive_initialized) {
      cold->entry_path.control_points[0].x = hot->x;
      cold->entry_path.control_points[0].y = 0 - hot->height;
      cold->entry_path.control_points[3].x = cold->formation_point.x;
      cold->entry_path.control_points[3].y = cold->formation_point.y;

      float dx = cold->entry_path.control_points[3].x -
                 cold->entry_path.control_points[0].x;
      float dy = cold->entry_path.control_points[3].y -
                 cold->entry_path.control_points[0].y;

      cold->entry_path.control_points[1].x =
          cold->entry_path.control_points[0].x + dx * (1.0f / 3.0f);
      cold->entry_path.control_points[1].y =
          cold->entry_path.control_points[0].y + dy * (1.0f / 3.0f);

      cold->entry_path.control_points[2].x =
          cold->entry_path.control_points[0].x + dx * (2.0f / 3.0f);
      cold->entry_path.control_points[2].y =
          cold->entry_path.control_points[0].y + dy * (2.0f / 3.0f);

      float offset = (hot->x > screen_width / 2.0f) ? -150.0f : 150.0f;
      cold->entry_path.control_points[1].x += offset;
      cold->entry_path.control_points[2].x += offset;

      cold->t = 0.0f;
      cold->dive_initialized = true;
    }
    cold->t += deltaTime / 2.0f;
    SDL_FPoint pos = bezier_point(cold->entry_path.control_points[0],
                                  cold->entry_path.control_points[1],
                                  cold->entry_path.control_points[2],
                                  cold->entry_path.control_points[3], cold->t);

    hot->x = pos.x;
    hot->y = pos.y;
    if (cold->t >= 1.0f) {
      cold->t = 1.0f; // clamp it
      cold->state = ENEMY_HOLDING;
      cold->state_start_time = SDL_GetTicks64();
      cold->dive_initialized = false;
    }
    break;
  }
  }
}
