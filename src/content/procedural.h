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

typedef enum {
  PATH_ARC,
  PATH_LINE_ISH,
  PATH_TIGHT_HOOK,
  PATH_WIDE_SWEEP,
  PATH_LOOP_DE_LOOP,
} PathType;

typedef struct {
  int total_enemies, max_simult_divers, species_unlocked;
  float speed_scalar, spawn_delay, dive_delay, threshold;
} WaveParams;

typedef struct {
  FormationType type;
  int max_per_row;
} FormationDefinition;

typedef struct {
  SDL_FPoint control_points[8];
  int num_segments;
} EntryPathData;

WaveParams level_to_params(int level);

void generate_formation(SDL_FPoint *positions, int total_enemies,
                        int screen_width, int screen_height,
                        FormationType type);

EntryPathData generate_path(PathType type, int screen_height, int screen_width, SDL_FPoint start, SDL_FPoint end);

#endif
