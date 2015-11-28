
#ifndef levelstate_hpp
#define levelstate_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include headers
#include "engine.hpp"
#include "gamestate.hpp"
#include "tiles.hpp"
#include "characters.hpp"
#include "camera.hpp"

class crate;

class levelstate
{
public:
    // tile dimensions of level
    int width;
    int height;
    
    // constructor
    levelstate() { };
    
    // camera
    camera camera;
    
    // characters
    dot b_char;
    dot w_char;
    
    // shift bool
    bool shiftable;
    
    // textures
    texture b_char_tex;
    texture w_char_tex;
    texture tile_tex;
    texture crate_tex_four_by_two;
    texture crate_tex_four_by_one;
    texture crate_tex_three_by_two;
    texture crate_tex_two_by_two;
    
    // tileset
    tile* tileset[MAX_SIZE];
    
    // crate array
    std::vector<crate*> crates;
    
    // level file path
    std::string path;
};

#endif /* levelstate_hpp */
