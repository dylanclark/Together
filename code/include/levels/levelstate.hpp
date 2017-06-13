
#ifndef levelstate_hpp
#define levelstate_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// include headers
#include <engine/engine.hpp>
#include <engine/gamestate.hpp>
#include <levels/tiles.hpp>
#include <characters/characters.hpp>
#include <levels/camera.hpp>
#include <levels/level_end.hpp>
#include <objects/button.hpp>
#include <objects/springboard.hpp>
#include <levels/level_messages.hpp>

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
    button b_button2;
    springboard w_springboard;
    springboard b_springboard;
    springboard w_cross_spring;
    springboard b_cross_spring;
    level_message level1_end;
    level_message level2_end;
    level_message level1_start;
    level_message level2_start;

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
    texture w_crate_tex_four_by_two;
    texture w_platform;
    texture crate_tex_four_by_one;
    texture crate_tex_three_by_two;
    texture crate_tex_two_by_two;
    texture b_springboard_tex;
    texture w_springboard_tex;
    texture w_end_animate;
    texture b_end_animate;
    texture b_cross_spring_tex;
    texture w_cross_spring_tex;
    texture level1_end_tex;
    texture level1_start_tex;
    texture level2_end_tex;
    texture level2_start_tex;

    // tileset
    tile* tileset[MAX_SIZE];

    // crate array
    std::vector<crate*> crates;

    // level file path
    std::string path;
};

#endif /* levelstate_hpp */
