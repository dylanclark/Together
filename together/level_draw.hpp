
#ifndef level_draw_hpp
#define level_draw_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include header files
#include "tiles.hpp"

// max level size
const int MAX_SIZE = 50 * 50;
const int MAX_CRATES = 10;

// draw the tilemap
bool set_tiles(tile* tiles[], std::string path, int level_w, int level_h);

// close the tilemap
void close_tiles(tile* tiles[]);

#endif /* level_draw_hpp */
