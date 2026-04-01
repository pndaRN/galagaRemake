#ifndef PROCEDURAL_H
#define PROCEDURAL_H

typedef struct {
  int total_enemies, max_simult_divers, species_unlocked;
  float speed_scalar, spawn_delay, dive_delay, threshold;
} WaveParams;

WaveParams level_to_params(int level);

#endif
