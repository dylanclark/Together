
#ifndef char_hpp
#define char_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
#include <textures.hpp>
#include <engine.hpp>
#include <tiles.hpp>

// dot status constants
const int CHAR_ACTIVE = 0;
const int CHAR_INACTIVATE = 1;
const int CHAR_INACTIVE = 2;
const int CHAR_ACTIVATE = 3;

// animation length
const int ANIMATION_LENGTH = 8;

class levelstate;

// dot class definition
class dot
{
public:
    static const int DOT_W = TILE_WIDTH;
    static const int DOT_H = TILE_WIDTH;
    const float DOT_VEL = 12;
    const float JUMP_VEL = 18;
    const float DOT_ACC = DOT_VEL / 12;
    const float GRAVITY = JUMP_VEL / 26;
    const float PUSH_VEL = DOT_VEL / 4;
    bool up, down, left, right;
    controller* controller;
    int status;
    bool color;
    int frame;

    dot();
    bool handle_event(SDL_Event &e, levelstate* level, engine* game);
    void move(levelstate* level, engine* game);
    bool crate_col(levelstate* level, engine* game);
    bool tile_col(tile* tileset[], int size, engine* game);
    void render(SDL_Rect* camera, engine* game);

    SDL_Rect col_rect;
    texture tex;
    bool black;
    int jump;

    void spring(int x, int y, int direction);
    bool center(SDL_Rect* end_rect);
    float get_y_vel();

private:
    float x_vel, y_vel;
};

class vector
{
public:
    // init
    vector();

    // coords
    int x;
    int y;
};

bool check_collision(SDL_Rect a, SDL_Rect b, vector* overlap);

#endif /* char_hpp */
