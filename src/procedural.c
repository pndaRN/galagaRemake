#include "procedural.h"
#include <SDL2/SDL_rect.h>

WaveParams level_to_params(int level) {
  WaveParams wp;

  int block_position = (level - 1) % 5 + 1;

  wp.total_enemies = 3 + (level * 2);
  wp.species_unlocked = (level - 1) / 5 + 1;
  wp.max_simult_divers = 1;
  wp.speed_scalar = 1.0f + block_position * 0.1f;
  wp.spawn_delay = 0.3f;
  wp.dive_delay = 2.0f / (block_position * 0.5 + 0.5);
  wp.threshold = .8;

  return wp;
}

void generate_formation(SDL_FPoint *positions, int total_enemies,
                        int screen_width, int screen_height,
                        FormationType type) {
  switch (type) {
  case FORMATION_TYPE_LINE:
    for (int i = 0; i < total_enemies; i++) {
      positions[i].x = (screen_width / (float)(total_enemies + 1)) * (i + 1);
      positions[i].y = 200.0f;
    }
    break;
  case FORMATION_TYPE_V:

    break;
  }
