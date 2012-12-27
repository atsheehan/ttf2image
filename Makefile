main: main.c
	gcc -Wall -std=c99 -o ttf2image main.c `sdl-config --cflags --libs` -lSDL_ttf