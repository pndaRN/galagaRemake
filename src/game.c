#include "game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <time.h>

static void game_handle_collisions(GameState *state);
static void render_game_world(const GameState *state, SDL_Renderer *renderer);

static const SDL_FPoint test_path[] = {
    {640, -50},
    {900, 50},
    {900, 150},
    {640, 200},
};

static SDL_FPoint formation_positions[5];

void game_init(GameState *state) {
  state->player = player_create(SCREEN_WIDTH, SCREEN_HEIGHT);

  for (int i = 0; i < 5; i++) {
    formation_positions[i].x = (SCREEN_WIDTH / 6.0f) * (i + 1);
    formation_positions[i].y = 200.0f;
  }

  state->wave =
      wave_init(5, 400.0f, test_path[0], test_path[1], test_path[2],
                test_path[3], formation_positions, SCREEN_HEIGHT, SCREEN_WIDTH);

  for (int i = 0; i < MAX_BULLETS; i++) {
    state->bullets[i].active = false;
  }

  for (int i = 0; i < MAX_ENEMIES; i++) {
    state->enemies[i].active = false;
  }

  state->spawn_timer = 0.0f;
  state->current_wave = 0;
  state->level = 1;
  state->mode = STATE_MENU;

  srand(time(NULL));
}

void game_render(const GameState *state, SDL_Renderer *renderer) {
  switch (state->mode) {
  case STATE_MENU:
    break;
  case STATE_PLAYING:
    render_game_world(state, renderer);
    break;
  case STATE_PAUSED:
    render_game_world(state, renderer);
    break;
  case STATE_GAME_OVER:
    render_game_world(state, renderer);
    break;
  }
  SDL_RenderPresent(renderer);
}

void game_update(GameState *state, float deltaTime, const Uint8 *keystate) {
  switch (state->mode) {
  case STATE_MENU:
    break;
  case STATE_PLAYING:
    player_update(&state->player, keystate, deltaTime, SCREEN_WIDTH);
    wave_update(&state->wave, deltaTime, state->enemies, MAX_ENEMIES);
    for (int i = 0; i < MAX_BULLETS; i++) {
      if (state->bullets[i].active) {
        bullet_update(&state->bullets[i], deltaTime);
      }
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
      if (state->enemies[i].active) {
        enemy_update(&state->enemies[i], deltaTime, SCREEN_HEIGHT,
                     state->player.x);
      }
    }
    game_handle_collisions(state);
    break;
  case STATE_PAUSED:
    break;
  case STATE_GAME_OVER:
    break;
  }
}

void game_handle_events(GameState *state, SDL_Event *event, bool *running) {
  while (SDL_PollEvent(event)) {
    if (event->type == SDL_QUIT) {
      *running = false;
    }
    if (event->type == SDL_WINDOWEVENT &&
        event->window.event == SDL_WINDOWEVENT_CLOSE) {
      *running = false;
    }
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_p) {
      *running = false;
    }
    switch (state->mode) {
    case STATE_MENU:
      if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        *running = false;
      }
      if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_RETURN) {
        state->mode = STATE_PLAYING;
      }
      break;
    case STATE_PLAYING:
      if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_SPACE) {
        for (int i = 0; i < MAX_BULLETS; i++) {
          if (!state->bullets[i].active) {
            state->bullets[i] = bullet_init(&state->player);
            break;
          }
        }
      }
      if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_e) {
        if (state->player.current_ammo == AMMO_PCN) {
          state->player.current_ammo = AMMO_POLYMYXIN;
        } else {
          state->player.current_ammo = AMMO_PCN;
        }
      }
      if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_q) {
        if (state->player.current_ammo == AMMO_PCN) {
          state->player.current_ammo = AMMO_POLYMYXIN;
        } else {
          state->player.current_ammo = AMMO_PCN;
        }
      }
      if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        state->mode = STATE_PAUSED;
      }
      break;
    case STATE_PAUSED:
      if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        state->mode = STATE_PLAYING;
      }
      break;
    case STATE_GAME_OVER:
      if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        state->mode = STATE_MENU;
      }
      break;
    }
  }
}

static void render_game_world(const GameState *state, SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_Rect playerRect = {(int)state->player.x, (int)state->player.y,
                         state->player.width, state->player.height};

  if (state->player.current_ammo == AMMO_PCN) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  }
  SDL_RenderFillRect(renderer, &playerRect);

  for (int i = 0; i < MAX_BULLETS; i++) {
    if (state->bullets[i].active) {
      SDL_Rect bulletRect = {(int)state->bullets[i].x, (int)state->bullets[i].y,
                             state->bullets[i].width, state->bullets[i].height};
      if (state->bullets[i].type == AMMO_PCN) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
      }
      SDL_RenderFillRect(renderer, &bulletRect);
    }
  }
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (state->enemies[i].active) {
      SDL_Rect enemyRect = {(int)state->enemies[i].x, (int)state->enemies[i].y,
                            state->enemies[i].width, state->enemies[i].height};
      const BacteriaDefinition *def =
          get_bacteria_def(state->enemies[i].species);
      SDL_SetRenderDrawColor(renderer, def->r, def->g, def->b, 255);
      SDL_RenderFillRect(renderer, &enemyRect);
    }
  }
}
static void game_handle_collisions(GameState *state) {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (state->bullets[i].active) {
      for (int j = 0; j < MAX_ENEMIES; j++) {
        if (state->enemies[j].active) {
          if (check_collision(state->bullets[i].x, state->bullets[i].y,
                              state->bullets[i].width, state->bullets[i].height,
                              state->enemies[j].x, state->enemies[j].y,
                              state->enemies[j].width,
                              state->enemies[j].height)) {
            const BacteriaDefinition *def =
                get_bacteria_def(state->enemies[j].species);
            bool effective = (state->bullets[i].type == AMMO_PCN &&
                              def->gram_type == GRAM_POSITIVE) ||
                             (state->bullets[i].type == AMMO_POLYMYXIN &&
                              def->gram_type == GRAM_NEGATIVE);
            if (effective) {
              state->enemies[j].active = false;
            }
            state->bullets[i].active = false;
            break;
          }
        }
      }
    }
  }
}
