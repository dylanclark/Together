
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
#include "level_end.hpp"

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
    
    // objects
    level_end b_level_end;
    level_end w_level_end;
    
    // textures
    texture b_char_tex;
    texture w_char_tex;
    texture tile_tex;
    texture b_end_tex;
    texture w_end_tex;
    
    // tile
    tile* tileset[MAX_SIZE];
    
    // level file path
    std::string path;
};

#endif /* levelstate_hpp */
