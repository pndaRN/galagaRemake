#include "bacteria.h"
#include "enemy.h"

void strep_dive_init(Enemy *e, float player_x) {
  // TODO: init sin wave state
}

void strep_dive_update(Enemy *e, float deltaTime, int screen_height) {
  // TODO: Sine wave movement
}

void staph_dive_init(Enemy *e, float player_x) {
  // TODO: init scatter state
}

void staph_dive_update(Enemy *e, float deltaTime, int screen_height) {
  // TODO: scatter movement
}

void ecoli_dive_init(Enemy *e, float player_x) {
  // TODO: init sin wave state
}

void ecoli_dive_update(Enemy *e, float deltaTime, int screen_height) {
  // TODO: Sine wave movement
}

void pseudomonas_dive_init(Enemy *e, float player_x) {
  // TODO: init sin wave state
}

void pseudomonas_dive_update(Enemy *e, float deltaTime, int screen_height) {
  // TODO: Sine wave movement
}

static const BacteriaDefinition BACTERIA_DEFS[] = {
    {SPECIES_STREPTOCOCCUS, GRAM_POSITIVE, DIVE_SINE, strep_dive_init,
     strep_dive_update},
    {SPECIES_STAPHYLOCOCCUS, GRAM_POSITIVE, DIVE_SCATTER, staph_dive_init,
     staph_dive_update},
    {SPECIES_ECOLI, GRAM_NEGATIVE, DIVE_ZIGZAG, ecoli_dive_init,
     ecoli_dive_update},
    {SPECIES_PSEUDOMONAS, GRAM_NEGATIVE, DIVE_SWEEP, pseudomonas_dive_init,
     pseudomonas_dive_update}};

const BacteriaDefinition *get_bacteria_def(BacteriaSpecies species) {
  return &BACTERIA_DEFS[species];
}
