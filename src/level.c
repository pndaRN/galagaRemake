#include "level.h"
#include "procedural.h"
#include "wave.h"

Level level_init(int level, int screen_height, int screen_width) {
  Level l;
  l.level = level;
  l.wave_count = 1;
  l.screen_height = screen_height;
  l.screen_width = screen_width;
  l.level_end = false;

  for (int i = 0; i < MAX_WAVES; i++) {
    l.wave[i].is_active = false;
    l.wave[i].enemy_indices = NULL;
  }

  WaveParams wp = level_to_params(l.level);

  l.end_chance = 1.0f; // TODO: procgen from level_to_params

  l.p0 = (SDL_FPoint){640, -50};
  l.p1 = (SDL_FPoint){900, 50};
  l.p2 = (SDL_FPoint){900, 150};
  l.p3 = (SDL_FPoint){640, 200};

  l.formation_positions =
      (SDL_FPoint *)malloc(wp.total_enemies * sizeof(SDL_FPoint));

  for (int i = 0; i < wp.total_enemies; i++) {
    l.formation_positions[i].x = (screen_width / 6.0f) * (i + 1);
    l.formation_positions[i].y = 200.0f;
  }

  l.wave[0] = wave_init(&wp, l.p0, l.p1, l.p2, l.formation_positions,
                        screen_height, screen_width);

  return l;
}

void level_update(Level *l, float deltaTime, Enemy *e, int max_enemies) {
  WaveParams wp = level_to_params(l->level);
  for (int i = 0; i < MAX_WAVES; i++) {
    if (l->wave[i].is_active) {
      wave_update(&l->wave[i], deltaTime, e, max_enemies);
      if (l->wave[i].threshold_crossed) {
        l->wave[i].threshold_crossed = false;
        float roll = (float)rand() / RAND_MAX;
        if (roll < l->end_chance) {
          l->level_end = true;
        } else {
          for (int j = 0; j < MAX_WAVES; j++) {
            if (!l->wave[j].is_active) {
              l->wave[j] =
                  wave_init(&wp, l->p0, l->p1, l->p2, l->formation_positions,
                            l->screen_height, l->screen_width);
              l->wave_count++;
              break;
            }
          }
        }
      }
    }
  }
}

void level_free(Level *l) {
  for (int i = 0; i < MAX_WAVES; i++) {
    wave_free(&l->wave[i]);
  }
}
