#include "enemy.h"
#include "bacteria.h"
#include "math_utils.h"
#include "procedural.h"

#include <SDL2/SDL_rect.h>
#include <math.h>

void enemy_init(EnemyHot *hot, EnemyCold *cold, float speed_scalar,
                EntryPathData path_data, SDL_FPoint formation_position,
                BacteriaSpecies species) {
  const BacteriaDefinition *bacteria_def = get_bacteria_def(species);

  hot->width = bacteria_def->width;
  hot->height = bacteria_def->height;
  hot->hb_height = bacteria_def->hb_height;
  hot->hb_width = bacteria_def->hb_width;
  hot->offset_x = bacteria_def->offset_x;
  hot->offset_y = bacteria_def->offset_y;
  hot->health = bacteria_def->health;
  hot->active = true;
  hot->species = species;
  hot->x = path_data.control_points[0].x;
  hot->y = path_data.control_points[0].y;
  hot->current_frame = rand() % bacteria_def->frame_count;
  hot->animation_timer = 0.0f;
  hot->angle = 0.0f;

  cold->speed_scalar = speed_scalar;
  cold->speed = bacteria_def->base_speed * speed_scalar;
  cold->state_start_time = 0;
  cold->state = ENEMY_ENTERING;
  cold->t = 0.0f;
  cold->entry_path = path_data;
  cold->formation_point = formation_position;
  cold->dive_initialized = false;
  cold->return_initialized = false;
}

void enemy_update(EnemyHot *hot, EnemyCold *cold, float deltaTime,
                  int screen_height, int screen_width, float player_x) {
  const BacteriaDefinition *def = get_bacteria_def(hot->species);
  if (def->frame_count > 1) {
    hot->animation_timer += deltaTime;
    if (hot->animation_timer >= def->frame_duration) {
      hot->animation_timer -= def->frame_duration;
      hot->current_frame = (hot->current_frame + 1) % def->frame_count;
    }
  }
  if (!hot->active) {
    return;
  }

  switch (cold->state) {
  case ENEMY_ENTERING: {

    cold->t += deltaTime / 3.0f;

    SDL_FPoint tangent = bezier_tangent(
        cold->entry_path.control_points[0], cold->entry_path.control_points[1],
        cold->entry_path.control_points[2], cold->entry_path.control_points[3],
        cold->t);

    hot->angle = atan2f(tangent.y, tangent.x) * (180.0f / M_PI) - 90.0f;

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
    const BacteriaDefinition *def = get_bacteria_def(hot->species);
    hot->angle = hot->angle + (0.0f - hot->angle) * (deltaTime * 3.0f);
    if (def->hold_update) {
      def->hold_update(hot, cold, deltaTime);
    }

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
    const BacteriaDefinition *def = get_bacteria_def(hot->species);
    if (!cold->return_initialized) {
      if (def->return_init) {
        def->return_init(hot, cold, screen_height, screen_width, player_x);
        cold->return_initialized = true;
      }
    }
    cold->t += deltaTime / 1.5f;

    SDL_FPoint tangent = bezier_tangent(
        cold->entry_path.control_points[0], cold->entry_path.control_points[1],
        cold->entry_path.control_points[2], cold->entry_path.control_points[3],
        cold->t);

    hot->angle = atan2f(tangent.y, tangent.x) * (180.0f / M_PI) - 90.0f;

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
      cold->return_initialized = false;
    }
    break;
  }
  }
}
