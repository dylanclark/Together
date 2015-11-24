
#ifndef initialization_hpp
#define initialization_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include header files
#include "tiles.hpp"

// screen dimensions!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

// initialize window
bool init();

// load all textures and media
bool load_media(tile* tiles[], int level_w, int level_h);

#endif /* initialization_hpp */
