#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"
#include "SDL.h"
#include "SDL_ttf.h"

#define FONT_SIZE 12
#define NUMBERS "0123456789"
#define UPPERCASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWERCASE "abcdefghijklmnopqrstuvwxyz"

int main(int argc, char *argv[]) {

  /* get font file from command line */
  if (argc < 2) {
    printf("usage: %s <fontfile>\n", argv[0]);
    exit(1);
  }

  int sdl_initialized = 0;
  const char *font_filename = argv[1];
  SDL_Color white = { 255, 255, 255 };
  SDL_Surface *surfaces[3] = { 0 };

  check(SDL_Init(SDL_INIT_VIDEO) == 0,
        "Failed to initialize SDL: %s", SDL_GetError());
  sdl_initialized = 1;

  check(TTF_Init() == 0,
        "Failed to initialize SDL_TTF: %s", TTF_GetError());

  TTF_Font *font = TTF_OpenFont(font_filename, FONT_SIZE);
  check(font, "Failed to open font at %s: %s", font_filename, TTF_GetError());

  surfaces[0] = TTF_RenderText_Solid(font, NUMBERS, white);
  check(surfaces[0], "Failed to render numbers: %s", TTF_GetError());


  check(SDL_SaveBMP(surfaces[0], "numbers.bmp") == 0,
        "Failed to save numbers to a file: %s", SDL_GetError());

 error:

  for (int i = 0; i < 3; i++) {
    if (surfaces[i]) {
      SDL_FreeSurface(surfaces[i]);
    }
  }

  if (font) {
    TTF_CloseFont(font);
  }

  if (TTF_WasInit()) {
    TTF_Quit();
  }

  if (sdl_initialized) {
    SDL_Quit();
  }
}
