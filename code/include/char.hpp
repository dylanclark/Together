
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

#define DOT_VEL 12
#define JUMP_VEL 18
#define DOT_ACC 1
#define GRAVITY 0.6
#define PUSH_VEL 3
#define ANIMATION_LENGTH 8

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

    Dot(int x, int y, bool is_black, Texture* texture);
    bool handle_event(SDL_Event &e, Levelstate* level, Engine* game);
    void move(Levelstate* level, Engine* game);
    void render(SDL_Rect* camera, Engine* game);

    SDL_Rect col_rect;
    int jump;

    void spring(int x, int y, int direction);
    bool center(SDL_Rect* end_rect);
    float get_y_vel();

private:
    float x_vel, y_vel;
    Controller* controller;
    bool up, down, left, right;
    Texture* tex;

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
