#include "bacteria.h"
#include "enemy.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

void strep_dive_init(Enemy *e, float player_x) {
  e->dive_state.type = DIVE_SINE;
  e->dive_state.sine.phase = 0.0f;
  e->dive_state.sine.amplitude = 75.0f;
  e->dive_state.sine.frequency = 0.5f;
  e->dive_state.sine.start_x = e->x;
  e->dive_state.sine.dive_speed = 200.0;
}

void strep_dive_update(Enemy *e, float deltaTime, int screen_height,
                       float player_x) {
  e->dive_state.sine.phase +=
      e->dive_state.sine.frequency * 2.0f * M_PI * deltaTime;
  e->x = e->dive_state.sine.start_x +
         sinf(e->dive_state.sine.phase) * e->dive_state.sine.amplitude;
  e->y += e->dive_state.sine.dive_speed * deltaTime;
  if (e->y > screen_height) {
    e->active = false;
  }
}

void staph_dive_init(Enemy *e, float player_x) {
  e->dive_state.type = DIVE_SCATTER;
  e->dive_state.scatter.start_x = e->x;
  e->dive_state.scatter.start_y = e->y;
  e->dive_state.scatter.phase = IS_BURSTING;
  e->dive_state.scatter.timer = 0.0f;
  e->dive_state.scatter.burst_duration = 0.4f;
  e->dive_state.scatter.pause_duration = 0.6f;

  e->dive_state.scatter.burst_speed = 500.0f;

  float random_angle = ((float)rand() / RAND_MAX) * 2.0 * M_PI;
  e->dive_state.scatter.burst_angle = random_angle;
}

void staph_dive_update(Enemy *e, float deltaTime, int screen_height,
                       float player_x) {
  if (e->dive_state.scatter.phase == IS_BURSTING) {
    e->x += cosf(e->dive_state.scatter.burst_angle) *
            e->dive_state.scatter.burst_speed * deltaTime;
    e->y += sinf(e->dive_state.scatter.burst_angle) *
            e->dive_state.scatter.burst_speed * deltaTime;
    e->dive_state.scatter.timer += deltaTime;

    if (e->dive_state.scatter.timer >= e->dive_state.scatter.burst_duration) {
      e->dive_state.scatter.phase = IS_PAUSING;
      e->dive_state.scatter.timer = 0.0f;
    }
  } else if (e->dive_state.scatter.phase == IS_PAUSING) {
    e->dive_state.scatter.timer += deltaTime;
    if (e->dive_state.scatter.timer >= e->dive_state.scatter.pause_duration) {
      e->dive_state.scatter.phase = IS_DIVING;
      e->dive_state.scatter.target_x =
          player_x + ((float)rand() / RAND_MAX - 0.5f) * 200.0f;
    }
  } else if (e->dive_state.scatter.phase == IS_DIVING) {
    float diff = e->dive_state.scatter.target_x - e->x;
    if (diff > 1.0f) {
      e->x += e->dive_state.scatter.burst_speed * 0.3f * deltaTime;
    } else if (diff < -1.0f) {
      e->x -= e->dive_state.scatter.burst_speed * 0.3f * deltaTime;
    }

    e->y += deltaTime * e->dive_state.scatter.burst_speed;

    if (e->y > screen_height) {
      e->active = false;
    }
  }
}

void ecoli_dive_init(Enemy *e, float player_x) {
  // TODO: init state
}

void ecoli_dive_update(Enemy *e, float deltaTime, int screen_height,
                       float player_x) {
  // TODO: movement
  e->y += 200.0f * deltaTime;
  if (e->y > screen_height) {
    e->active = false;
  }
}

void pseudomonas_dive_init(Enemy *e, float player_x) {
  // TODO: init sin wave state
}

void pseudomonas_dive_update(Enemy *e, float deltaTime, int screen_height,
                             float player_x) {
  // TODO: Sine wave movement
  e->y += 200.0f * deltaTime;
  if (e->y > screen_height) {
    e->active = false;
  }
}

static const BacteriaDefinition BACTERIA_DEFS[] = {
    {SPECIES_STREPTOCOCCUS, GRAM_POSITIVE, DIVE_SINE, strep_dive_init,
     strep_dive_update, 0, 200, 0},
    {SPECIES_STAPHYLOCOCCUS, GRAM_POSITIVE, DIVE_SCATTER, staph_dive_init,
     staph_dive_update, 150, 200, 0},
    {SPECIES_ECOLI, GRAM_NEGATIVE, DIVE_ZIGZAG, ecoli_dive_init,
     ecoli_dive_update, 0, 100, 200},
    {SPECIES_PSEUDOMONAS, GRAM_NEGATIVE, DIVE_SWEEP, pseudomonas_dive_init,
     pseudomonas_dive_update, 0, 200, 200}};

const BacteriaDefinition *get_bacteria_def(BacteriaSpecies species) {
  return &BACTERIA_DEFS[species];
}
