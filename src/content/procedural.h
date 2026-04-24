#ifndef PROCEDURAL_H
#define PROCEDURAL_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef enum {
  FORMATION_TYPE_LINE,
  FORMATION_TYPE_V,
  FORMATION_TYPE_STAGGER,
  FORMATION_TYPE_DIAMOND,
  FORMATION_TYPE_HEXAGON,
  FORMATION_TYPE_CIRCLE,
  FORMATION_TYPE_COUNT,
} FormationType;

typedef enum {
  PATH_ARC,
  PATH_LINE_ISH,
  PATH_TIGHT_HOOK,
  PATH_WIDE_SWEEP,
  PATH_LOOP_DE_LOOP,
} PathType;

typedef struct {
  SDL_FPoint control_points[8];
  int num_segments;
} EntryPathData;

typedef struct {
  float x, y;
} SpawnPointFraction;

typedef struct {
  float x, y, width, height;
} FormationBounds;

typedef union {
  struct {
        int max_per_row;
        float row_spacing_fraction;
  } line;
  struct {
    float apex_angle;
  } v;
  struct {
    float radius_fraction;
  } circle;
  struct {
    int rows; 
  } stagger;
  struct {
    float aspect_ratio;
  } diamond;
  struct {
    float side_fraction;
  } hexagon;
} FormationParams;

typedef struct {
  int placed;
  int remaining;
} FormationResult;

typedef bool (*FormationFitsFn)(int count, float min_spacing, FormationBounds bounds, FormationParams params);

typedef void (*FormationSizesFn)(float min_spacing, FormationBounds bounds, FormationParams params,
                                int *out_sizes, int *out_count, int max_sizes);
                              
typedef FormationResult (*FormationGenFn)(SDL_FPoint *positions, int count, float min_spacing, FormationBounds bounds, FormationParams params);

typedef struct {
  FormationType type;
  FormationFitsFn fits;
  FormationSizesFn sizes;
  FormationGenFn generate;
} FormationDefinition;

typedef struct {
  int total_enemies, max_simult_divers, species_unlocked, level;
  float speed_scalar, spawn_delay, dive_delay, threshold;
  PathType path_type;
  FormationType formation_type;
  FormationParams formation_params;
} WaveParams;

WaveParams level_to_params(int level);

FormationResult generate_formation(SDL_FPoint *positions, int count, float min_spacing, 
                        FormationType type, FormationParams params,
                        FormationBounds bounds);

EntryPathData generate_path(PathType type, int screen_height, int screen_width,
                            SDL_FPoint start, SDL_FPoint end);

SDL_FPoint generate_spawn_point(int level, int screen_width, int screen_height);

const FormationDefinition *get_formation_def(FormationType type);

#endif
