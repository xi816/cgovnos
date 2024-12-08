#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINW 640
#define WINH 480

void DRAW1010(SDL_Renderer* r, short px, short py) {
  for (short i = 0; i < 10; i++) {
    for  (short j = 0; j < 10; j++) {
      SDL_RenderDrawPoint(r, px*10+i, py*10+j);
    }
  }
}

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *WIN = SDL_CreateWindow(
      "CGovnOS", 100, 100, WINW, WINH, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(
      WIN, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  SDL_Event event;
  bool quit = false;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  DRAW1010(renderer, 5, 5);
  DRAW1010(renderer, 6, 5);
  DRAW1010(renderer, 7, 5);
  DRAW1010(renderer, 6, 4);
  DRAW1010(renderer, 6, 3);
  SDL_RenderPresent(renderer);

  while(!quit) {
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) quit = true;
    }
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(WIN);
  SDL_Quit();
}

