#include "procedural.h"

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
