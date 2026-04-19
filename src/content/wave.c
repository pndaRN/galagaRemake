#include "wave.h"
#include "bacteria.h"
#include "enemy.h"

Wave wave_init(WaveParams *wp, SDL_FPoint p0, SDL_FPoint p1, SDL_FPoint p2,
               SDL_FPoint *formation_positions, int screen_height,
               int screen_width) {
  Wave w;
  w.level = wp->level;
  w.total_enemies = wp->total_enemies;

  w.spawn_delay = wp->spawn_delay;
  w.species_unlocked = wp->species_unlocked;
  w.dive_delay = wp->dive_delay;
  w.threshold = wp->threshold;
  w.speed_scalar = wp->speed_scalar;
  w.threshold_crossed = false;

  w.spawn_count = 0;
  w.spawn_timer = 0.3f;
  w.path = wp->path_type;
  w.dive_timer = 0.0;
  w.formation_complete = false;
  w.formation_complete_time = 0;

  memcpy(w.formation_positions, formation_positions,
         w.total_enemies * sizeof(SDL_FPoint));

  w.control_points[0] = p0;
  w.control_points[1] = p1;
  w.control_points[2] = p2;
  w.is_active = true;

  w.screen_height = screen_height;
  w.screen_width = screen_width;

  return w;
}

void wave_update(Wave *w, float deltaTime, EnemyHot *hot, EnemyCold *cold,
                 int max_enemies) {
  if (!w->is_active) {
    return;
  }

  w->spawn_timer += deltaTime;

  // SPAWN ENEMIES

  if (w->spawn_timer >= w->spawn_delay && w->spawn_count < w->total_enemies) {
    for (int i = 0; i < max_enemies; i++) {
      if (!hot[i].active) {
        BacteriaSpecies species =
            (BacteriaSpecies)(w->spawn_count % w->species_unlocked);
        SDL_FPoint spawn_point =
            generate_spawn_point(w->level, w->screen_width, w->screen_height);
        EntryPathData path_data =
            generate_path(w->path, w->screen_height, w->screen_width,
                          spawn_point, w->formation_positions[w->spawn_count]);
        enemy_init(&hot[i], &cold[i], w->speed_scalar, path_data,
                   w->formation_positions[w->spawn_count], species);

        w->enemy_indices[w->spawn_count] = i;
        w->spawn_count += 1;
        w->spawn_timer = 0.0f;
        break;
      }
    }
  }

  // CHECK IF SPAWNED ENEMIES ARE IN HOLDING

  if (w->spawn_count == w->total_enemies && !w->formation_complete) {
    bool all_holding = true;
    for (int i = 0; i < w->spawn_count; i++) {
      if (cold[w->enemy_indices[i]].state != ENEMY_HOLDING) {
        all_holding = false;
        break;
      }
    }
    if (all_holding) {
      w->formation_complete = true;
      w->formation_complete_time = SDL_GetTicks64();
    }
  }

  // ENEMY STARTS DIVE

  w->dive_timer += deltaTime;

  if (w->formation_complete &&
      SDL_GetTicks64() >= w->formation_complete_time + 1000.0f &&
      w->dive_timer >= w->dive_delay) {
    int candidates[w->spawn_count];
    int candidates_count = 0;

    for (int i = 0; i < w->spawn_count; i++) {
      int idx = w->enemy_indices[i];
      if (hot[idx].active && cold[idx].state == ENEMY_HOLDING) {
        candidates[candidates_count] = idx;
        candidates_count++;
      }
    }
    if (candidates_count > 0) {
      int pick = rand() % candidates_count;
      int rand_index = candidates[pick];
      cold[rand_index].state = ENEMY_DIVING;
      cold[rand_index].state_start_time = SDL_GetTicks64();
      w->dive_timer = 0.0f;
    }
  }

  // CHECKS IF ALL ENEMIES IN WAVE ARE INACTIVE
  int active_count = 0;
  bool all_dead;
  for (int i = 0; i < w->spawn_count; i++) {
    if (hot[w->enemy_indices[i]].active) {
      active_count++;
    }
  }
  all_dead = active_count == 0;
  if (w->spawn_count > 0) {
    int killed = w->spawn_count - active_count;
    float percentage_killed = (float)killed / w->spawn_count;
    if (percentage_killed >= w->threshold) {
      w->threshold_crossed = true;
    }
  }
  if (all_dead && w->spawn_count == w->total_enemies) {
    w->is_active = false;
  }
}
