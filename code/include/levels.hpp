#ifndef levels_hpp
#define levels_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <engine.hpp>
#include <textures.hpp>
#include <char.hpp>
#include <objects.hpp>
#include <tiles.hpp>

// camera edge buffer
const float BUFFER = 400;

// max level size
const int MAX_SIZE = 50 * 50;
const int MAX_CRATES = 10;

// speed constants
const float SPEED = 14;
const float CAM_ACC = 2;

class camera
{
public:
    SDL_Rect display;
    SDL_Rect location;

    float x_vel;
    float y_vel;
    float w_vel;
    float h_vel;

    camera(int w, int h);

    void track(SDL_Rect* b_char, SDL_Rect* w_char);
    void move(int level_w, int level_h, engine* game);
    void update(void);
};

class level_end
{
public:
    level_end();

    void render(SDL_Rect* camera, engine* game);

    SDL_Rect col_rect;
    texture tex;

    bool check(SDL_Rect dot_rect);
};

class level_message
{
public:
    level_message();
    void send_message(int type, engine* game);
    void render(SDL_Rect* camera, SDL_Renderer* rend);
    SDL_Rect col_rect;
    texture tex;
    int type;
};

class crate;
class tile;

class levelstate
{
public:
    // tile dimensions of level
    int width;
    int height;
    bool shiftable;

    levelstate() { };

    camera* camera;
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



#endif /* levels_hpp */