#include "wave.h"

Wave wave_init(int total_enemies, float speed, SDL_FPoint p0, SDL_FPoint p1,
               SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint *formation_positions) {
  Wave w;
  w.total_enemies = total_enemies;
  w.speed = speed;

  w.spawn_count = 0;
  w.spawn_delay = 0.3f;
  w.spawn_timer = 0.3f;  // start ready to spawn immediately
  //
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

    if (w->spawn_timer >= w->spawn_delay && w->spawn_count < w->total_enemies){
        for (int i = 0; i < max_enemies; i++) {
          if (!e[i].active) {
            e[i] = enemy_init(w->control_points[0], 
                              w->control_points[1],
                              w->control_points[2],
                              w->control_points[3], 
                              w->speed,
                              w->formation_positions[w->spawn_count]);
            w->spawn_count += 1;
            w->spawn_timer = 0.0f;
            break;
          }
        }
    }
    if (w->spawn_count >= w->total_enemies) {
        w->is_active = false;
    }
}
