#ifndef PROCEDURAL_H
#define PROCEDURAL_H

#include <SDL2/SDL.h>

typedef enum {
  FORMATION_TYPE_LINE,
  FORMATION_TYPE_V,
  FORMATION_TYPE_STAGGER,
  FORMATION_TYPE_DIAMOND,
  FORMATION_TYPE_HEXAGON,
  FORMATION_TYPE_CIRCLE,
} FormationType;

typedef struct {
  int total_enemies, max_simult_divers, species_unlocked;
  float speed_scalar, spawn_delay, dive_delay, threshold;
} WaveParams;

WaveParams level_to_params(int level);

void generate_formation(SDL_FPoint *positions, int total_enemies,
                        int screen_width, int screen_height,
                        FormationType type);
#endif
