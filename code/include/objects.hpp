
#ifndef object_hpp
#define object_hpp

// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
#include <textures.hpp>
#include <char.hpp>
#include <engine.hpp>

// button status constants
const int BUTT_INACTIVE = 0;
const int BUTT_ACTIVATE = 1;
const int BUTT_ACTIVE = 2;
const int BUTT_INACTIVATE = 3;

// animation length
const int BUTT_ANIMATION_LENGTH = 4;

// direction constants
const int UP = 0;
const int RIGHT = 1;
const int DOWN = 2;
const int LEFT = 3;

// crate types
const int FOUR_BY_TWO = 0;
const int FOUR_BY_ONE = 1;
const int TWO_BY_TWO = 2;
const int THREE_BY_TWO = 3;

// max number of tiles that this crate will need to render
const int MAX_BORDER = 12;

// button status constants
const int BOARD_INACTIVE = 0;
const int BOARD_ACTIVATE = 1;
const int BOARD_ACTIVE = 2;
const int BOARD_INACTIVATE = 3;

// animation length
const int BOARD_ANIMATION_LENGTH = 7;

// direction
const int FLIP_LEFT = 0;
const int FLIP_RIGHT = 1;

const int LOCATION = 0;
const int VELOCITY = 1;

class Button
{
public:
    Button();

    void render(SDL_Rect* camera, Engine* game);

    double angle = 0.0;
    SDL_Point* center = NULL;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Rect col_rect;

    Texture tex;

    bool check(SDL_Rect dot_rect);

    bool activated;
    bool single;
    bool used;
    int direction;
    int status;
    int frame;

};

class Crate
{
public:
    float x_vel;
    SDL_Rect get_col_rect() { return col_rect; };
    bool check_col(SDL_Rect crate, Levelstate* level, Vector* repos);
    void update();
    void render(int b_status, SDL_Rect* camera, Engine* game, Levelstate* level);

    Texture tex;
    int crate_type;
    bool black;
    bool pushed;

    Crate(int x, int y, int type);

    Tile* tileset[MAX_BORDER];
    void create_tiles(int b_status, Levelstate* level);
    bool generating;

    int tile_type_top(int type);
    int tile_type_bottom(int type);
    int tile_type_left(int type);
    int tile_type_right(int type);

    SDL_Rect col_rect;
};

class Springboard
{
public:
    Springboard();

    void render(SDL_Rect* camera, Engine* game);
    SDL_Rect col_rect;
    Texture tex;

    bool check(SDL_Rect dot_rect);

    bool activated;
    bool show;
    int direction;
    int status;
    int frame;

    // type. can be LOCATION for defualt jumping or VELOCITY for jumping the
    // abount of the dot doing the springing
    int type;
    double angle = 0.0;
    SDL_Point* center = NULL;
    SDL_RendererFlip flip_type;

    float x_spring;
    float y_spring;

    const float SPRING_X_VEL = 0;
    const float SPRING_Y_VEL = 10;

    // cross-layer spring logic
    void cross_spring(Dot* springer, Dot* springee, int type);

};

#endif /* object_hpp */