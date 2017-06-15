
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

class Levelstate;

// dot class definition
class Dot
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
    Controller* controller;
    int status;
    bool color;
    int frame;

    Dot(int x, int y, bool is_black, Texture* texture);
    bool handle_event(SDL_Event &e, Levelstate* level, Engine* game);
    void move(Levelstate* level, Engine* game);
    bool crate_col(Levelstate* level, Engine* game);
    bool tile_col(Tile* tileset[], int size, Engine* game);
    void render(SDL_Rect* camera, Engine* game);

    SDL_Rect col_rect;
    Texture* tex;
    bool black;
    int jump;

    void spring(int x, int y, int direction);
    bool center(SDL_Rect* end_rect);
    float get_y_vel();

private:
    float x_vel, y_vel;
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
