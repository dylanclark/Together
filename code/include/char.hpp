
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

class Levelstate;

class Dot
{
public:
    Dot(int x, int y, bool is_black, SDL_Renderer* rend, SDL_Color* palette = NULL);

    bool black;

    bool handle_event(SDL_Event &e, Levelstate* level, Engine* game);
    void move(Levelstate* level, Engine* game);
    void render(SDL_Rect* camera, Engine* game);
    SDL_Rect get_rect();

    void spring(int x, int y, int direction);
    bool center(SDL_Rect* end_rect);
    float get_y_vel();

private:
    // status = idle, jump, run, inactive, etc.
    int status;
    // which way is the dot facing?
    int dir;
    // what animation frame is the dot on?
    int jump_start;

    // is the dot jumping?
    int short_hop;
    float x_vel, y_vel;
    bool up, down, left, right;
    Controller* controller;
    Texture tex;
    SDL_Rect col_rect;

    bool crate_col(Levelstate* level, Engine* game);
    bool tile_col(Tile* tileset[], int size, Engine* game);
};

class Vector
{
public:
    // init
    Vector();

    // coords
    int x;
    int y;
};

bool check_collision(SDL_Rect a, SDL_Rect b, Vector* overlap);

#endif /* char_hpp */
