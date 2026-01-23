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

Wave wave_update(Wave *w, float deltaTime, Enemy *e, int max_enemies) {
    // 1. Check if wave is active
    
    // 2. Add deltaTime to spawn_timer
    
    // 3. If spawn_timer >= spawn_delay AND spawn_count < total_enemies:
    //    - Find inactive enemy slot
    //    - Spawn enemy with enemy_init
    //    - Increment spawn_count
    //    - Reset spawn_timer
    
    // 4. If spawn_count >= total_enemies, set is_active = false
}
