#include "player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

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

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  bool running = true;
  SDL_Event event;

  Player player = player_create(SCREEN_WIDTH, SCREEN_HEIGHT);

  Uint64 lastTime = SDL_GetTicks64();
  float deltaTime = 0.0f;
  
  // GAME LOOP

  while (running) {

    Uint64 currentTime = SDL_GetTicks64();
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    while (SDL_PollEvent(&event)) {
      printf("Event: %d\n", event.type);
      if (event.type == SDL_QUIT) {
        running = false;
      }
      if (event.type == SDL_WINDOWEVENT &&
          event.window.event == SDL_WINDOWEVENT_CLOSE) {
        running = false;
      }
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        running = false;
      }
    }

    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    player_update(&player, keystate, deltaTime, SCREEN_WIDTH);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect playerRect = {
      (int)player.x,
      (int)player.y,
      player.width,
      player.height
    };

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &playerRect);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
