#include "game.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not init! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window =
      SDL_CreateWindow("Galaga", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  GameState state;
  game_init(&state);

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
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
