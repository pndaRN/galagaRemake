#include "level.h"
#include "procedural.h"
#include "wave.h"

Level level_init(int level, int screen_height, int screen_width) {
  Level l;
  l.level = level;
  l.wave_count = 0;
  l.screen_height = screen_height;
  l.screen_width = screen_width;
  l.level_end = false;

  for (int i = 0; i < MAX_WAVES; i++) {
    l.wave[i].is_active = false;
  }

  l.default_formation = FORMATION_TYPE_LINE;

  l.formation_bounds = (FormationBounds){
      .x = screen_width * 0.05f,
      .y = screen_height * 0.1f,
      .width = screen_width * 0.9f,
      .height = screen_height * 0.2f,
  };

  WaveParams wp = level_to_params(l.level);

  l.end_chance = 1.0f; // TODO: procgen from level_to_params

  l.p0 = (SDL_FPoint){640, -50};
  l.p1 = (SDL_FPoint){900, 50};
  l.p2 = (SDL_FPoint){900, 150};
  l.p3 = (SDL_FPoint){640, 200};

  float min_spacing = 64.0f; // TODO: Compute from species pool

  FormationResult result = generate_formation(l.formation_positions, wp.total_enemies, min_spacing,
                                              wp.formation_type, wp.formation_params, l.formation_bounds);
  (void)result; // TODO: use place/remaining when multi-shape lands

  l.wave[0] = wave_init(&wp, l.p0, l.p1, l.p2, l.formation_positions,
                        screen_height, screen_width);

  return l;
}

void level_update(Level *l, float deltaTime, EnemyHot *hot, EnemyCold *cold,
                  int max_enemies) {
  WaveParams wp = level_to_params(l->level);
  for (int i = 0; i < MAX_WAVES; i++) {
    if (l->wave[i].is_active) {
      wave_update(&l->wave[i], deltaTime, hot, cold, max_enemies);
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
