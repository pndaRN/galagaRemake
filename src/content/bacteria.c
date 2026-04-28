#include "bacteria.h"
#include "enemy.h"
#include "math_utils.h"
#include "shared_types.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

void strep_dive_init(EnemyHot *hot, EnemyCold *cold, int screen_height,
                     int screen_width, float player_x) {
  (void)screen_width;
  (void)screen_height;
  (void)player_x;
  cold->dive_state.type = DIVE_SINE;
  cold->dive_state.sine.phase = 0.0f;
  cold->dive_state.sine.amplitude = 10.0f;
  cold->dive_state.sine.frequency = 1.0f;
  cold->dive_state.sine.start_x = hot->x;
  cold->dive_state.sine.dive_speed = 200.0;
}

void strep_hold_update(EnemyHot *hot, EnemyCold *cold, float deltaTime) {
  (void)deltaTime;

  const BacteriaDefinition *def = get_bacteria_def(hot->species);

  float baseX = cold->formation_point.x;
  float baseY = cold->formation_point.y;

  Uint64 elapsed = SDL_GetTicks64() - cold->state_start_time;
  float elapsed_seconds = elapsed / 1000.0f;

  float phase = elapsed_seconds * 2.0f * M_PI;

  float ramp = fminf(elapsed_seconds, 1.0f);

  hot->x = baseX + sinf(phase) * ramp * def->width;
  hot->y = baseY;

  float cycle_position =
      fmodf(cold->dive_state.sine.phase, 2.0f * M_PI) / (2.0f * M_PI);
  if (cycle_position < 0)
    cycle_position += 1.0f;
  hot->current_frame = (int)(cycle_position * def->frame_count);
  if (hot->current_frame >= def->frame_count)
    hot->current_frame = def->frame_count - 1;
}

void strep_dive_update(EnemyHot *hot, EnemyCold *cold, float deltaTime,
                       int screen_height, int screen_width, float player_x) {
  (void)screen_width;
  (void)player_x;
  cold->dive_state.sine.phase +=
      cold->dive_state.sine.frequency * 2.0f * M_PI * deltaTime;
  float s = sinf(cold->dive_state.sine.phase);
  float eased = s;
  hot->x =
      cold->dive_state.sine.start_x + eased * cold->dive_state.sine.amplitude;
  const BacteriaDefinition *def = get_bacteria_def(hot->species);
  float cycle_position =
      fmodf(cold->dive_state.sine.phase, 2.0f * M_PI) / (2.0f * M_PI);
  if (cycle_position < 0)
    cycle_position += 1.0f;
  hot->current_frame = (int)(cycle_position * def->frame_count);
  if (hot->current_frame >= def->frame_count)
    hot->current_frame = def->frame_count - 1;
  hot->y += cold->dive_state.sine.dive_speed * deltaTime;

  hot->angle = atan2f(cold->dive_state.sine.dive_speed,
                      cosf(cold->dive_state.sine.phase) *
                          cold->dive_state.sine.amplitude *
                          cold->dive_state.sine.frequency) *
                   (180.0f / M_PI) -
               90.0f;

  if (hot->y > screen_height) {
    cold->state = ENEMY_RETURNING;
    cold->dive_initialized = false;
  }
}

void staph_dive_init(EnemyHot *hot, EnemyCold *cold, int screen_height,
                     int screen_width, float player_x) {
  (void)screen_width;
  (void)screen_height;
  (void)player_x;
  cold->dive_state.type = DIVE_SCATTER;
  cold->dive_state.scatter.start_x = hot->x;
  cold->dive_state.scatter.start_y = hot->y;
  cold->dive_state.scatter.phase = IS_BURSTING;
  cold->dive_state.scatter.timer = 0.0f;
  cold->dive_state.scatter.burst_duration = 0.4f;
  cold->dive_state.scatter.pause_duration = 0.6f;

  cold->dive_state.scatter.burst_speed = 500.0f;

  float random_angle = ((float)rand() / RAND_MAX) * M_PI;
  cold->dive_state.scatter.burst_angle = random_angle;
}

void staph_dive_update(EnemyHot *hot, EnemyCold *cold, float deltaTime,
                       int screen_height, int screen_width, float player_x) {
  (void)screen_width;
  if (cold->dive_state.scatter.phase == IS_BURSTING) {
    hot->x += cosf(cold->dive_state.scatter.burst_angle) *
              cold->dive_state.scatter.burst_speed * deltaTime;
    hot->y += sinf(cold->dive_state.scatter.burst_angle) *
              cold->dive_state.scatter.burst_speed * deltaTime;
    cold->dive_state.scatter.timer += deltaTime;

    if (cold->dive_state.scatter.timer >=
        cold->dive_state.scatter.burst_duration) {
      cold->dive_state.scatter.phase = IS_PAUSING;
      cold->dive_state.scatter.timer = 0.0f;
    }
  } else if (cold->dive_state.scatter.phase == IS_PAUSING) {
    cold->dive_state.scatter.timer += deltaTime;
    if (cold->dive_state.scatter.timer >=
        cold->dive_state.scatter.pause_duration) {
      cold->dive_state.scatter.phase = IS_DIVING;
      cold->dive_state.scatter.target_x =
          player_x + ((float)rand() / RAND_MAX - 0.5f) * 200.0f;
    }
  } else if (cold->dive_state.scatter.phase == IS_DIVING) {
    float diff = cold->dive_state.scatter.target_x - hot->x;
    if (diff > 1.0f) {
      hot->x += cold->dive_state.scatter.burst_speed * 0.3f * deltaTime;
    } else if (diff < -1.0f) {
      hot->x -= cold->dive_state.scatter.burst_speed * 0.3f * deltaTime;
    }

    hot->y += deltaTime * cold->dive_state.scatter.burst_speed;

    if (hot->y > screen_height) {
      cold->state = ENEMY_RETURNING;
      cold->dive_initialized = false;
    }
  }
}

void ecoli_dive_init(EnemyHot *hot, EnemyCold *cold, int screen_height,
                     int screen_width, float player_x) {
  (void)screen_width;
  (void)screen_height;

  cold->dive_state.type = DIVE_ZIGZAG;
  if (player_x >= hot->x) {
    cold->dive_state.zigzag.direction = 1;
  } else {
    cold->dive_state.zigzag.direction = -1;
  }
  cold->dive_state.zigzag.dash_cooldown = 0.2f;
  cold->dive_state.zigzag.timer = 0.0f;
  cold->dive_state.zigzag.start_x = hot->x;
  cold->dive_state.zigzag.start_y = hot->y;
  cold->dive_state.zigzag.phase = DASHING;
  cold->dive_state.zigzag.speed = 300.0f;
}

void ecoli_dive_update(EnemyHot *hot, EnemyCold *cold, float deltaTime,
                       int screen_height, int screen_width, float player_x) {

  (void)screen_width;

  if (cold->dive_state.zigzag.phase == DASHING) {
    hot->y += cold->dive_state.zigzag.speed * deltaTime;
    hot->x += cold->dive_state.zigzag.speed * deltaTime *
              cold->dive_state.zigzag.direction;
    cold->dive_state.zigzag.timer += deltaTime;
    if (cold->dive_state.zigzag.timer >=
        cold->dive_state.zigzag.dash_cooldown) {
      cold->dive_state.zigzag.phase = PAUSING;
      cold->dive_state.zigzag.timer = 0.0f;
    }
  } else if (cold->dive_state.zigzag.phase == PAUSING) {
    cold->dive_state.zigzag.timer += deltaTime;
    if (player_x >= hot->x) {
      cold->dive_state.zigzag.direction = 1;
    } else {
      cold->dive_state.zigzag.direction = -1;
    }
    if (cold->dive_state.zigzag.timer >=
        cold->dive_state.zigzag.dash_cooldown) {
      cold->dive_state.zigzag.phase = DASHING;
      cold->dive_state.zigzag.timer = 0.0f;
    }
  }
  if (hot->y > screen_height) {
    cold->state = ENEMY_RETURNING;
    cold->dive_initialized = false;
  }
}

void pseudomonas_dive_init(EnemyHot *hot, EnemyCold *cold, int screen_height,
                           int screen_width, float player_x) {
  (void)player_x;
  cold->dive_state.type = DIVE_SWEEP;
  cold->dive_state.sweep.t = 0.0f;
  cold->dive_state.sweep.control_points[0].x = hot->x;
  cold->dive_state.sweep.control_points[0].y = hot->y;
  if (hot->x >= screen_width / 2.0f) {
    cold->dive_state.sweep.control_points[3].x = 0;
    cold->dive_state.sweep.control_points[3].y = screen_height + hot->height;
    cold->dive_state.sweep.control_points[2].x = screen_width * 0.25f;

  } else {
    cold->dive_state.sweep.control_points[3].x = screen_width;
    cold->dive_state.sweep.control_points[3].y = screen_height + hot->height;
    cold->dive_state.sweep.control_points[2].x = screen_width * 0.75f;
  }
  cold->dive_state.sweep.control_points[1].x = hot->x;
  cold->dive_state.sweep.control_points[1].y = hot->y + 500.f;
  cold->dive_state.sweep.control_points[2].y = screen_height * 0.9f;
}

void pseudomonas_dive_update(EnemyHot *hot, EnemyCold *cold, float deltaTime,
                             int screen_height, int screen_width,
                             float player_x) {
  (void)screen_width;
  (void)screen_height;
  (void)player_x;
  cold->dive_state.sweep.t += deltaTime / 2.0f;
  SDL_FPoint pos = bezier_point(cold->dive_state.sweep.control_points[0],
                                cold->dive_state.sweep.control_points[1],
                                cold->dive_state.sweep.control_points[2],
                                cold->dive_state.sweep.control_points[3],
                                cold->dive_state.sweep.t);

  hot->x = pos.x;
  hot->y = pos.y;

  if (cold->dive_state.sweep.t >= 1.0f) {
    cold->dive_state.sweep.t = 1.0f; // clamp it
    cold->state = ENEMY_RETURNING;
    cold->dive_initialized = false;
  }
}

static const BacteriaDefinition BACTERIA_DEFS[] = {
    {.species = SPECIES_STREPTOCOCCUS,
     .weakness = WEAPON_PCN,
     .dive_type = DIVE_SINE,
     .dive_init = strep_dive_init,
     .dive_update = strep_dive_update,
     .r = 0,
     .g = 200,
     .b = 0,
     .health = 6,
     .base_speed = 400.0f,
     .width = 32,
     .height = 64,
     .hb_width = 22,
     .hb_height = 64,
     .offset_x = 5,
     .offset_y = 0,
     .frame_count = 12,
     .frame_duration = 1.0f / 12.0f,
     .texture_path = "assets/bacteria/strep.png"},

    {.species = SPECIES_STAPHYLOCOCCUS,
     .weakness = WEAPON_PCN,
     .dive_type = DIVE_SCATTER,
     .dive_init = staph_dive_init,
     .dive_update = staph_dive_update,
     .r = 150,
     .g = 200,
     .b = 0,
     .health = 6,
     .base_speed = 300.0f,
     .width = 50,
     .height = 50,
     .hb_width = 50,
     .hb_height = 50,
     .offset_x = 0,
     .offset_y = 0,
     .frame_count = 1,
     .frame_duration = 0.0f,
     .texture_path = "assets/bacteria/staph.png"},

    {.species = SPECIES_ECOLI,
     .weakness = WEAPON_PMX,
     .dive_type = DIVE_ZIGZAG,
     .dive_init = ecoli_dive_init,
     .dive_update = ecoli_dive_update,
     .r = 0,
     .g = 100,
     .b = 200,
     .health = 6,
     .base_speed = 500.0f,
     .width = 50,
     .height = 50,
     .hb_width = 50,
     .hb_height = 50,
     .offset_x = 0,
     .offset_y = 0,
     .texture_path = "assets/bacteria/ecoli.png"},

    {.species = SPECIES_PSEUDOMONAS,
     .weakness = WEAPON_PMX,
     .dive_type = DIVE_SWEEP,
     .dive_init = pseudomonas_dive_init,
     .dive_update = pseudomonas_dive_update,
     .r = 0,
     .g = 200,
     .b = 200,
     .health = 6,
     .base_speed = 400.0f,
     .width = 50,
     .height = 50,
     .hb_width = 50,
     .hb_height = 50,
     .offset_x = 0,
     .offset_y = 0,
     .texture_path = "assets/bacteria/pseudomonas.png"}};

const BacteriaDefinition *get_bacteria_def(BacteriaSpecies species) {
  return &BACTERIA_DEFS[species];
}
