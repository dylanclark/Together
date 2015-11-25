
#ifndef level_draw_hpp
#define level_draw_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include header files
#include "tiles.hpp"

// activity bools
extern bool b_active;
extern bool w_active;

// max level size
const int MAX_SIZE = 50 * 50;

// level class
class level
{
public:
    // level dimensions
    int width, height;
    
    // character origins
    int b_orig_x, b_orig_y, w_char_x, w_char_y;
    
    std::string path;
    
    // pointer to next level
    level* next_level;
};

// draw the tilemap
bool set_tiles(tile* tiles[], std::string path, int level_w, int level_h);

// close the tilemap
void close_tiles(tile* tiles[]);

#endif /* level_draw_hpp */
