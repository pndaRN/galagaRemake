#include "game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>

int main() {
  int result = 0;
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  bool game_started = false;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not init! SDL_Error: %s\n", SDL_GetError());
    result = 1;
    goto cleanup;
  }

  window =
      SDL_CreateWindow("Galaga", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
    result = 1;
    goto cleanup;
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    printf("SDL_image could not init! IMG_Error: %s\n", IMG_GetError());
    result = 1;
    goto cleanup;
  }

  GameState state;
  game_init(&state, renderer);
  game_started = true;

  bool running = true;
  SDL_Event event;
  Uint64 lastTime = SDL_GetTicks64();

  // GAME LOOP

  while (running) {
    Uint64 currentTime = SDL_GetTicks64();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    game_handle_events(&state, &event, &running);
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    game_update(&state, deltaTime, keystate);
    game_render(&state, renderer);
  }

cleanup:
  if (game_started)
    game_cleanup(&state);
  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
  return result;
}
