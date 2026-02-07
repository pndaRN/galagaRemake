#include "wave.h"
#include "bacteria.h"
#include "enemy.h"

Wave wave_init(int total_enemies, float speed, SDL_FPoint p0, SDL_FPoint p1,
               SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint *formation_positions,
               int screen_height, int screen_width) {
  Wave w;
  w.total_enemies = total_enemies;
  w.enemy_indices = (int *)malloc(total_enemies * sizeof(int));

  w.speed = speed;

  w.spawn_count = 0;
  w.spawn_delay = 0.3f;
  w.spawn_timer = 0.3f;
  w.dive_timer = 0.0;
  w.dive_delay = 2.0;

  w.formation_complete = false;
  w.formation_complete_time = 0;

  w.control_points[0] = p0;
  w.control_points[1] = p1;
  w.control_points[2] = p2;
  w.control_points[3] = p3;

  w.formation_positions = formation_positions;
  w.is_active = true;

  w.screen_height = screen_height;
  w.screen_width = screen_width;

  return w;
}

void wave_update(Wave *w, float deltaTime, Enemy *e, int max_enemies) {
  if (!w->is_active) {
    return;
  }

  w->spawn_timer += deltaTime;

  // SPAWN ENEMIES

  if (w->spawn_timer >= w->spawn_delay && w->spawn_count < w->total_enemies) {
    for (int i = 0; i < max_enemies; i++) {
      if (!e[i].active) {
        BacteriaSpecies species = (BacteriaSpecies)(w->spawn_count % 4);
        e[i] = enemy_init(w->control_points[0], w->control_points[1],
                          w->control_points[2],
                          w->formation_positions[w->spawn_count], w->speed,
                          w->formation_positions[w->spawn_count], species,
                          w->screen_height, w->screen_width);
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
      if (e[w->enemy_indices[i]].state != ENEMY_HOLDING) {
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
      if (e[idx].active && e[idx].state == ENEMY_HOLDING) {
        candidates[candidates_count] = idx;
        candidates_count++;
      }
    }
    if (candidates_count > 0) {
      int pick = rand() % candidates_count;
      int rand_index = candidates[pick];
      e[rand_index].state = ENEMY_DIVING;
      e[rand_index].state_start_time = SDL_GetTicks64();
      w->dive_timer = 0.0f;
    }
  }

  // CHECKS IF ALL ENEMIES IN WAVE ARE INACTIVE
  bool all_dead = true;
  for (int i = 0; i < w->spawn_count; i++) {
    if (e[w->enemy_indices[i]].active) {
      all_dead = false;
      break;
    }
  }
  if (all_dead && w->spawn_count == w->total_enemies) {
    w->is_active = false;
  }
}
