# ttf2image

Converts a TrueType file (.ttf) into a bitmap image (.bmp) for
rendering in an OpenGL application. Currently only a small subset
of the ASCII table is rendered (32 to 127) which includes the
alphanumeric characters and some punctuation. The range of characters
rendered can be changed by adjusting the START_CHAR and END_CHAR
constants at the top of main.c.

## Usage

tt2image takes the following command line arguments

    ./ttf2image <fontfile> <outfile> <fontsize>

where fontfile is the .ttf file to read from, outfile is the name of
the file to save the bitmap to, and fontsize determines how big each
character is rendered.

## Building

Requires SDL and SDL_ttf. On Debian Linux, the following packages
satisfy these requirements:

* libsdl1.2-dev
* libsdl-ttf2.0-dev

Once those are installed, you can run

    make

from the directory to create the ttf2image executable.

