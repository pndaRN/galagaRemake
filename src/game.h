#ifndef GAME_H
#define GAME_H

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#include "bullet.h"
#include "collision.h"
#include "enemy.h"
#include "player.h"
#include "wave.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

typedef enum {
STATE_MENU,
STATE_PLAYING,
STATE_PAUSED,
STATE_GAME_OVER
} GameState_Mode;

typedef struct {
  Player player;
  Bullet bullets[MAX_BULLETS];
  Enemy enemies[MAX_ENEMIES];
  Wave wave;
  GameState_Mode mode;

  float spawn_timer;
  int current_wave;
  int level;
} GameState;

void game_init(GameState *state);
void game_render(const GameState *state, SDL_Renderer *renderer);
void game_update(GameState *state, float deltaTime, const Uint8 *keystate);
void game_handle_events(GameState *state, SDL_Event *event, bool *running);

#endif
