#include "wave.h"

Wave wave_init(int total_enemies, float speed, SDL_FPoint p0, SDL_FPoint p1,
               SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint *formation_positions) {
  Wave w;
  w.total_enemies = total_enemies;
  w.enemy_indices = (int *)malloc(total_enemies * sizeof(int));

  w.speed = speed;

  w.spawn_count = 0;
  w.spawn_delay = 0.3f;
  w.spawn_timer = 0.3f; 

  bool formation_complete = false;
  Uint64 formation_complete_time = 0;

  w.control_points[0] = p0;
  w.control_points[1] = p1;
  w.control_points[2] = p2;
  w.control_points[3] = p3;

  w.formation_positions = formation_positions;
  w.is_active = true;
  return w;
}

void wave_update(Wave *w, float deltaTime, Enemy *e, int max_enemies) {
  if (!w->is_active) {
    return;
  }

  w->spawn_timer += deltaTime;

  if (w->spawn_timer >= w->spawn_delay && w->spawn_count < w->total_enemies) {
    for (int i = 0; i < max_enemies; i++) {
      if (!e[i].active) {
        EnemyType type = (w->spawn_count % 2 == 0) ? BACTERIA_GRAM_POSITIVE
                                                   : BACTERIA_GRAM_NEGATIVE;
        e[i] = enemy_init(w->control_points[0], w->control_points[1],
                          w->control_points[2],
                          w->formation_positions[w->spawn_count], w->speed,
                          w->formation_positions[w->spawn_count], type);
        w->enemy_indices[spawn_count] = i;
        w->spawn_count += 1;
        w->spawn_timer = 0.0f;
        break;
      }
    }
  }
  if (w->spawn_count == w->total_enemies && !w->formation_complete){
    bool all_holding = true; 
    for (int i = 0; i < w->spawn_count; i++){
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
  if (w->spawn_count >= w->total_enemies) {
    w->is_active = false;
  }
}
