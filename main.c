#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dbg.h"
#include "SDL.h"
#include "SDL_ttf.h"

#define NUMBERS "0123456789"
#define UPPERCASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWERCASE "abcdefghijklmnopqrstuvwxyz"

#define PADDING 2

#define START_CHAR  32
#define END_CHAR    127
#define TOTAL_CHARS (END_CHAR - START_CHAR)

int main(int argc, char *argv[]) {

  /* get font file from command line */
  if (argc < 4) {
    printf("usage: %s <fontfile> <outfile> <fontsize>\n", argv[0]);
    exit(1);
  }

  int sdl_initialized = 0;
  const char *font_filename = argv[1];
  const char *out_filename = argv[2];

  int font_size = atoi(argv[3]);
  check(font_size > 0, "Must specify a font size greater than 0.");

  SDL_Color white = { 255, 255, 255 };

  SDL_Surface *temp_surfaces[TOTAL_CHARS];
  SDL_Surface *out_surface = NULL;

  check(SDL_Init(SDL_INIT_VIDEO) == 0,
        "Failed to initialize SDL: %s", SDL_GetError());
  sdl_initialized = 1;

  check(TTF_Init() == 0,
        "Failed to initialize SDL_TTF: %s", TTF_GetError());

  TTF_Font *font = TTF_OpenFont(font_filename, font_size);
  check(font, "Failed to open font at %s: %s", font_filename, TTF_GetError());

  char string[2];
  int max_height = 0;
  int max_width = 0;

  for (int i = 0; i < TOTAL_CHARS; i++) {
    sprintf(string, "%c", START_CHAR + i);
    temp_surfaces[i] = TTF_RenderText_Solid(font, string, white);
    check(temp_surfaces[i], "TTF_RenderText_Solid failed for string %s: %s",
          string, TTF_GetError());

    if (temp_surfaces[i]->h > max_height) {
      max_height = temp_surfaces[i]->h;
    }

    if (temp_surfaces[i]->w > max_width) {
      max_width = temp_surfaces[i]->w;
    }
  }

  int chars_per_line = (int)ceil(sqrt((double)(TOTAL_CHARS * max_height) / (double)max_width));
  int rows = (int)ceil((double)TOTAL_CHARS / (double)chars_per_line);

  int char_width = max_width + PADDING;
  int char_height = max_height + PADDING;

  debug("char_width: %d, char_height: %d, chars_per_line: %d, rows: %d",
        char_width, char_height, chars_per_line, rows);

  int total_width = char_width * chars_per_line;
  int total_height = char_height * rows;

  out_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, total_width, total_height,
                                     32, 0, 0, 0, 0);
  check(out_surface, "Failed to create final surface: %s", SDL_GetError());

  SDL_Rect dest = (SDL_Rect){ 0, 0, 0, 0 };

  for (int i = 0; i < TOTAL_CHARS; i++) {
    int row = i / chars_per_line;
    int col = i % chars_per_line;

    /* Blit each character into the appropriate cell of the grid,
       but add an offset depending on the width of the character
       so that it is rendered in the center. */
    int cell_x = col * char_width;
    int cell_y = row * char_height;

    int x_offset = (char_width - temp_surfaces[i]->w) / 2;
    int y_offset = (char_height - temp_surfaces[i]->h) / 2;

    dest.x = cell_x + x_offset;
    dest.y = cell_y + y_offset;

    check(SDL_BlitSurface(temp_surfaces[i], NULL, out_surface, &dest) == 0,
          "Failed to blit onto final surface: %s", SDL_GetError());
  }

  check(SDL_SaveBMP(out_surface, out_filename) == 0,
        "Failed to save numbers to a file: %s", SDL_GetError());

 error:
  if (out_surface) {
    SDL_FreeSurface(out_surface);
  }

  for (int i = 0; i < TOTAL_CHARS; i++) {
    if (temp_surfaces[i]) {
      SDL_FreeSurface(temp_surfaces[i]);
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
