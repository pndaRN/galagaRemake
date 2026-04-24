#include "procedural.h"
#include <SDL2/SDL_rect.h>

WaveParams level_to_params(int level) {
  WaveParams wp;
  wp.level = level;

  int block_position = (level - 1) % 5 + 1;

  wp.total_enemies = 3 + (level * 2);
  wp.species_unlocked = (level - 1) / 5 + 1;
  wp.max_simult_divers = 1;
  wp.speed_scalar = 1.0f + block_position * 0.1f;
  wp.spawn_delay = 0.3f;
  wp.dive_delay = 2.0f / (block_position * 0.5 + 0.5);
  wp.threshold = .8;
  wp.path_type = PATH_LINE_ISH;
  wp.formation_type = FORMATION_TYPE_LINE;
  wp.formation_params.line.max_per_row = -1;
  wp.formation_params.line.row_spacing_fraction = 1.25f;
  return wp;
}



EntryPathData generate_path(PathType type, int screen_height, int screen_width,
                            SDL_FPoint start, SDL_FPoint end) {
  EntryPathData path;
  path.num_segments = 0;
  float dx, dy;
  float offset;

  switch (type) {
  case PATH_ARC:
    path.control_points[0] = start;
    path.control_points[3] = end;

    dx = end.x - start.x;
    dy = end.y - start.y;

    path.control_points[1].x = start.x + dx * (1.0f / 3.0f);
    path.control_points[1].y = start.y + dy * (1.0f / 3.0f);

    path.control_points[2].x = start.x + dx * (2.0f / 3.0f);
    path.control_points[2].y = start.y + dy * (2.0f / 3.0f);

    offset = (start.x > screen_width / 2.0f) ? -screen_width * 0.12f
                                             : screen_width * 0.12f;
    path.control_points[1].x += offset;
    path.control_points[2].x += offset;

    path.num_segments = 1;
    break;

  case PATH_LINE_ISH:
    path.control_points[0] = start;
    path.control_points[3] = end;

    dx = end.x - start.x;
    dy = end.y - start.y;

    path.control_points[1].x = start.x + dx * (1.0f / 3.0f);
    path.control_points[1].y = start.y + dy * (1.0f / 3.0f);

    path.control_points[2].x = start.x + dx * (2.0f / 3.0f);
    path.control_points[2].y = start.y + dy * (2.0f / 3.0f);

    offset = (start.x > screen_width / 2.0f) ? -screen_width * 0.06f
                                             : screen_width * 0.06f;
    path.control_points[1].x += offset;
    path.control_points[2].x += offset;

    path.num_segments = 1;
    break;

  case PATH_TIGHT_HOOK:
    path.control_points[0] = start;
    path.control_points[3] = end;
  case PATH_WIDE_SWEEP:
    break;

  case PATH_LOOP_DE_LOOP:
    break;
  }
  return path;
}

static const SpawnPointFraction SPAWN_POINT_FRACTIONS[] = {
    {.x = -0.07f, .y = 0.33f}, {.x = -0.07f, .y = 0.17f},
    {.x = 0.17f, .y = -0.07f}, {.x = 0.33f, .y = -0.07f},
    {.x = 0.5f, .y = -0.07f},  {.x = 0.67f, .y = -0.07f},
    {.x = 0.83f, .y = -0.07f}, {.x = 1.07f, .y = 0.17f},
    {.x = 1.07f, .y = 0.33f},
};

SDL_FPoint generate_spawn_point(int level, int screen_width,
                                int screen_height) {
  SDL_FPoint point;
  if (level <= 5) {
    point.x = screen_width * SPAWN_POINT_FRACTIONS[4].x;
    point.y = screen_height * SPAWN_POINT_FRACTIONS[4].y;
  } else { // TODO: INIT LEVEL > 5
    point.x = screen_width * SPAWN_POINT_FRACTIONS[4].x;
    point.y = screen_height * SPAWN_POINT_FRACTIONS[4].y;
  }
  return point;
}

typedef struct {
  int cols, max_rows, max_count;
} LineLayout;

static LineLayout line_compute_layout(FormationBounds bounds, float min_spacing, FormationParams params) {
  LineLayout layout;

  int width_capacity = (int)(bounds.width / min_spacing) - 1;
  if (width_capacity < 1) width_capacity = 1;

  int cols = width_capacity;
  if (params.line.max_per_row > 0 && params.line.max_per_row < cols) {
    cols = params.line.max_per_row;
  }
  layout.cols = cols;

  float row_spacing = min_spacing * params.line.row_spacing_fraction;
  layout.max_rows = (int)(bounds.height / row_spacing);
  if (layout.max_rows < 1) layout.max_rows = 1;  
 
  layout.max_count = layout.cols * layout.max_rows;

  return layout;
}

static bool line_fits(int count, float min_spacing, FormationBounds bounds, FormationParams params) {
  LineLayout layout = line_compute_layout(bounds, min_spacing, params);
  return count <= layout.max_count;
}

static void line_sizes (float min_spacing, FormationBounds bounds, FormationParams params, int *out_sizes, int *out_count, int max_sizes) {
  LineLayout layout = line_compute_layout(bounds, min_spacing, params);
  int n = layout.max_count;
  if (n > max_sizes) n = max_sizes;
  for (int i = 0; i < n; i++) {
    out_sizes[i] = i + 1;
  }
  *out_count = n;
}

static FormationResult line_generate(SDL_FPoint *positions, int count,
                                     float min_spacing, FormationBounds bounds,
                                     FormationParams params) {
  LineLayout layout = line_compute_layout(bounds, min_spacing, params);
  int placed = (count <= layout.max_count) ? count : layout.max_count;

  int rows_used = (placed + layout.cols - 1) / layout.cols;

  float v_step = bounds.height / (float)(rows_used + 1);

  for (int i = 0; i < placed; i ++) {
    int row = i / layout.cols;
    int col = i % layout.cols;

    int cols_in_row = layout.cols;
    if (row == rows_used - 1) {
      int remainder = placed % layout.cols;
      if (remainder > 0) cols_in_row = remainder;
    }

    float h_step = bounds.width / (float)(cols_in_row + 1);
    positions[i].x = bounds.x + (col + 1) * h_step;
    positions[i].y = bounds.y + (row + 1) * v_step;
  }

  return (FormationResult){.placed = placed, .remaining = count - placed}; 
}

static const FormationDefinition FORMATION_DEFS[] = {
  [FORMATION_TYPE_LINE] = {
    .type = FORMATION_TYPE_LINE,
    .fits = line_fits,
    .sizes = line_sizes, 
    .generate = line_generate,
  },
};

const FormationDefinition *get_formation_def(FormationType type) {
  return &FORMATION_DEFS[type];
}

FormationResult generate_formation(SDL_FPoint *positions, int count, float min_spacing,
                        FormationType type, FormationParams params, FormationBounds bounds) {
  const FormationDefinition *def = get_formation_def(type);
  return def->generate(positions, count, min_spacing, bounds, params);
}