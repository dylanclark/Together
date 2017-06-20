
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

static const int DOT_VEL = 12;
static const int JUMP_VEL = 18;
static const int DOT_ACC = 1;
static const float GRAVITY = 0.6;
static const int PUSH_VEL = 3;
static const int ANIMATION_LENGTH = 8;

typedef enum _char_status {
    CHAR_ACTIVE = 0,
    CHAR_INACTIVATE,
    CHAR_INACTIVE,
    CHAR_ACTIVATE
} char_status;

class Levelstate;

// dot class definition
class Dot
{
public:
    int status;
    int frame;
    bool black;

    Dot(int x, int y, bool is_black, SDL_Renderer* rend);
    bool handle_event(SDL_Event &e, Levelstate* level, Engine* game);
    void move(Levelstate* level, Engine* game);
    void render(SDL_Rect* camera, Engine* game);
    SDL_Rect get_rect();

    void spring(int x, int y, int direction);
    bool center(SDL_Rect* end_rect);
    float get_y_vel();

private:
    int jump;
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
