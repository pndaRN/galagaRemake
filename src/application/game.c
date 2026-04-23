#include "game.h"
#include "collision.h"
#include "enemy.h"
#include "level.h"
#include "player.h"
#include "shared_types.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <time.h>

static void game_handle_collisions(GameState *state);
static void render_game_world(const GameState *state, SDL_Renderer *renderer);

void game_init(GameState *state, SDL_Renderer *renderer) {
  assets_init(&state->assets, renderer);
  state->player = player_create(SCREEN_WIDTH, SCREEN_HEIGHT);
  state->level = level_init(1, SCREEN_HEIGHT, SCREEN_WIDTH);

  for (int i = 0; i < MAX_BULLETS; i++) {
    state->bullets[i].active = false;
  }

  for (int i = 0; i < MAX_ENEMIES; i++) {
    state->enemy_hot[i].active = false;
  }
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
  case STATE_LEVEL_TRANSITION:
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
    level_update(&state->level, deltaTime, state->enemy_hot, state->enemy_cold,
                 MAX_ENEMIES);
    if (state->level.level_end == true) {
      int new_level = state->level.level + 1;
      state->level = level_init(new_level, SCREEN_HEIGHT, SCREEN_WIDTH);
      for (int i = 0; i < MAX_ENEMIES; i++) {
        state->enemy_hot[i].active = false;
      }
      for (int i = 0; i < MAX_BULLETS; i++) {
        state->bullets[i].active = false;
      }
    }
    for (int i = 0; i < MAX_BULLETS; i++) {
      if (state->bullets[i].active) {
        bullet_update(&state->bullets[i], deltaTime);
      }
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
      if (state->enemy_hot[i].active) {
        enemy_update(&state->enemy_hot[i], &state->enemy_cold[i], deltaTime,
                     SCREEN_HEIGHT, SCREEN_WIDTH, state->player.x);
      }
    }
    game_handle_collisions(state);
    break;
  case STATE_PAUSED:
    break;
  case STATE_GAME_OVER:
    break;
  case STATE_LEVEL_TRANSITION:
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
        state->player.current_ammo =
            (state->player.current_ammo + 1) % WEAPON_COUNT;
      }
      if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_q) {
        state->player.current_ammo =
            (state->player.current_ammo + WEAPON_COUNT - 1) % WEAPON_COUNT;
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
    case STATE_LEVEL_TRANSITION:
      break;
    }
  }
}

static void render_game_world(const GameState *state, SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  if (state->player.active) {
    SDL_Rect playerRect = {(int)state->player.x, (int)state->player.y,
                           state->player.width, state->player.height};
    SDL_RenderCopy(renderer, state->assets.ships[state->player.current_ammo],
                   NULL, &playerRect);

    // HITBOX TEST

    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // SDL_Rect hbRect = {(int)(state->player.x + state->player.hb_offset_x),
    //                   (int)(state->player.y + state->player.hb_offset_y),
    //                   state->player.hb_width, state->player.hb_height};
    // SDL_RenderDrawRect(renderer, &hbRect);
  }

  for (int i = 0; i < MAX_BULLETS; i++) {
    if (state->bullets[i].active &&
        state->assets.bullets[state->bullets[i].type]) {
      SDL_Rect bulletRect = {(int)state->bullets[i].x, (int)state->bullets[i].y,
                             state->bullets[i].width, state->bullets[i].height};
      SDL_RenderCopy(renderer, state->assets.bullets[state->bullets[i].type],
                     NULL, &bulletRect);
    }
  }
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (state->enemy_hot[i].active) {
      SDL_Rect enemyRect = {
          (int)state->enemy_hot[i].x, (int)state->enemy_hot[i].y,
          state->enemy_hot[i].width, state->enemy_hot[i].height};

      SDL_Texture *texture =
          state->assets.bacteria[state->enemy_hot[i].species];
      if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, &enemyRect);
      } else {
        const BacteriaDefinition *def =
            get_bacteria_def(state->enemy_hot[i].species);
        SDL_SetRenderDrawColor(renderer, def->r, def->g, def->b, 255);
        SDL_RenderFillRect(renderer, &enemyRect);
      }
    }
  }
}

static void game_handle_collisions(GameState *state) {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (state->bullets[i].active) {
      for (int j = 0; j < MAX_ENEMIES; j++) {
        if (state->enemy_hot[j].active) {
          if (check_collision(state->bullets[i].x, state->bullets[i].y,
                              state->bullets[i].width, state->bullets[i].height,
                              state->enemy_hot[j].x, state->enemy_hot[j].y,
                              state->enemy_hot[j].width,
                              state->enemy_hot[j].height)) {
            const BacteriaDefinition *bacteria_def =
                get_bacteria_def(state->enemy_hot[j].species);
            const WeaponDefinition *weapon_def =
                get_weapon_def(state->bullets[i].type);
            state->enemy_hot[j].health -=
                calculate_damage(weapon_def, bacteria_def);
            if (state->enemy_hot[j].health <= 0) {
              state->enemy_hot[j].active = false;
            }
            state->bullets[i].active = false;
            break;
          }
        }
      }
    }
  }
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (state->enemy_hot[i].active) {
      if (check_collision(state->player.hb_offset_x + state->player.x,
                          state->player.hb_offset_y + state->player.y,
                          state->player.hb_width, state->player.hb_height,
                          state->enemy_hot[i].x, state->enemy_hot[i].y,
                          state->enemy_hot[i].width,
                          state->enemy_hot[i].height)) {
        state->player.active = false;
        state->enemy_hot[i].active = false;
        state->mode = STATE_GAME_OVER;
        break;
      }
    }
  }
}

void game_cleanup(GameState *state) { assets_destroy(&state->assets); }
