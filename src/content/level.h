#ifndef LEVEL_H
#define LEVEL_H
#define MAX_WAVES 10

#include "enemy.h"
#include "wave.h"

typedef struct {
  Wave wave[MAX_WAVES];
  int wave_count, level;
  bool level_end;
  float end_chance;

  // Temp vars until procgen is done
  int screen_height, screen_width;
  SDL_FPoint p0, p1, p2, p3;
  SDL_FPoint formation_positions[MAX_ENEMIES];

} Level;

Level level_init(int level, int screen_height, int screen_width);
void level_update(Level *l, float deltaTime, EnemyHot *hot, EnemyCold *cold,
                  int max_enemies);

#endif
