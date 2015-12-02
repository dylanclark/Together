
#ifndef levelstate_hpp
#define levelstate_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

// include headers
#include "engine.hpp"
#include "gamestate.hpp"
#include "tiles.hpp"
#include "characters.hpp"
#include "camera.hpp"
#include "level_end.hpp"
#include "button.hpp"
#include "springboard.hpp"

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
    camera* camera;
    
    // characters
    dot b_char;
    dot w_char;
    
    // objects
    level_end b_level_end;
    level_end w_level_end;
    button b_button;
    button w_button;
    springboard w_springboard;
    springboard b_springboard;
    
    // shift bool
    bool shiftable;
    
    // textures
    texture b_char_tex;
    texture w_char_tex;
    texture tile_tex;
    texture b_end_tex;
    texture w_end_tex;
    texture b_button_tex;
    texture w_button_tex;
    texture crate_tex_four_by_two;
    texture crate_tex_four_by_one;
    texture crate_tex_three_by_two;
    texture crate_tex_two_by_two;
    texture b_springboard_tex;
    texture w_springboard_tex;
    texture w_end_animate;
    texture b_end_animate;
    
    // tileset
    tile* tileset[MAX_SIZE];
    
    // crate array
    std::vector<crate*> crates;
    
    // level file path
    std::string path;
};

#endif /* levelstate_hpp */
