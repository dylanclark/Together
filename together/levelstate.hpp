
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
#include "button.hpp"

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
    
<<<<<<< HEAD
    // objects
    level_end b_level_end;
    level_end w_level_end;
    button b_button;
    button w_button;
=======
    // shift bool
    bool shiftable;
>>>>>>> 2e8ee63... Implemented movable crates!!!
    
    // textures
    texture b_char_tex;
    texture w_char_tex;
    texture tile_tex;
<<<<<<< HEAD
    texture b_end_tex;
    texture w_end_tex;
    texture b_button_tex;
    texture w_button_tex;
=======
    texture crate_tex_four_by_two;
    texture crate_tex_four_by_one;
    texture crate_tex_three_by_two;
    texture crate_tex_two_by_two;
>>>>>>> 2e8ee63... Implemented movable crates!!!
    
    // tileset
    tile* tileset[MAX_SIZE];
    
    // crate array
    std::vector<crate*> crates;
    
    // level file path
    std::string path;
};

#endif /* levelstate_hpp */
