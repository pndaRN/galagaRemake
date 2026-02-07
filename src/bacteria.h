#ifndef BACTERIA_H
#define BACTERIA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <stdbool.h>

typedef struct Enemy Enemy;

typedef enum { DIVE_SINE, DIVE_SCATTER, DIVE_ZIGZAG, DIVE_SWEEP } DiveType;
typedef enum {
  SPECIES_STREPTOCOCCUS,
  SPECIES_STAPHYLOCOCCUS,
  SPECIES_ECOLI,
  SPECIES_PSEUDOMONAS
} BacteriaSpecies;

typedef enum { GRAM_POSITIVE, GRAM_NEGATIVE } GramType;

typedef enum { IS_BURSTING, IS_PAUSING, IS_DIVING } ScatterPhase;
typedef enum { DASHING, PAUSING } ZigZagPhase;

typedef struct {
  DiveType type;
  union {
    struct {
      float phase;
      float amplitude;
      float frequency;
      float start_x;
      float dive_speed;
    } sine;
    struct {
      float burst_angle, burst_speed;
      float timer, burst_duration, pause_duration;
      float start_x, start_y, target_x;
      ScatterPhase phase;
    } scatter;
    struct {
      int direction;
      float dash_cooldown, timer, speed;
      float start_x, start_y;
      ZigZagPhase phase;
    } zigzag;
    struct {
      SDL_FPoint control_points[4];
      float t;
    } sweep;
  };
} DiveState;

typedef void (*DiveInitFn)(Enemy *e, float player_x);
typedef void (*DiveUpdateFn)(Enemy *e, float deltaTime, int screen_height,
                             float player_x);

typedef struct {
  BacteriaSpecies species;
  GramType gram_type;
  DiveType dive_type;
  DiveInitFn dive_init;
  DiveUpdateFn dive_update;
  Uint8 r, g, b;
} BacteriaDefinition;

const BacteriaDefinition *get_bacteria_def(BacteriaSpecies species);

#endif
