#ifndef PROCEDURAL_H
#define PROCEDURAL_H
#include <SDL2/SDL.h>

typedef struct {
    int total_enemies, max_simult_divers; 
    float speed, spawn_delay, dive_delay;
} WaveParams;

WaveParams level_to_params(int level);

#endif