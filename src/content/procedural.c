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
  wp.path_type = PATH_ARC;
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
    int middle = total_enemies / 2;
    for (int i = 0; i < total_enemies; i++) {
      positions[i].x = (screen_width / (float)(total_enemies + 1)) * (i + 1);
      positions[i].y = 100.0f + 100.0f * abs(i - middle) / (float)middle;
    }
    break;
  case FORMATION_TYPE_CIRCLE:
    break;
    // TODO
  case FORMATION_TYPE_DIAMOND:
    break;
    // TODO
  case FORMATION_TYPE_HEXAGON:
    break;
    // TODO
  case FORMATION_TYPE_STAGGER:
    break;
  }
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

static const SpawnPointFraction SPAWN_POINT_FACTIONS[] = {
    {.x = 0.17f, .y = -0.07f}, {.x = 0.33f, .y = -0.07f},
    {.x = 0.5f, .y = -0.07f},  {.x = 0.67f, .y = -0.07f},
    {.x = 0.83f, .y = -0.07f}, {.x = -0.07f, .y = 0.17f},
    {.x = -0.07f, .y = 0.33f}, {.x = 1.07f, .y = 0.17f},
    {.x = 1.07f, .y = 0.33f},
};

SDL_FPoint generate_spawn_point(int level, int screen_width,
                                int screen_height) {
  SDL_FPoint point;
  if (level <= 5) {
    point.x = screen_width * SPAWN_POINT_FACTIONS[2].x;
    point.y = screen_height * SPAWN_POINT_FACTIONS[2].y;
  } else { // TODO: INIT LEVEL > 5
    point.x = screen_width * SPAWN_POINT_FACTIONS[2].x;
    point.y = screen_height * SPAWN_POINT_FACTIONS[2].y;
  }
  return point;
}

static const FormationDefinition FORMATION_DEFS[] = {
    {.type = FORMATION_TYPE_LINE, .max_per_row = 7},
};
