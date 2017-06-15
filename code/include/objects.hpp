
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

class button
{
public:
    // initialize traits
    button();

    // render the item on the screen
    void render(SDL_Rect* camera, engine* game);

    // angle to flip
    double angle = 0.0;

    // flip axis point
    SDL_Point* center = NULL;

    // flip type
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    // collision rectangle
    SDL_Rect col_rect;

    // texture
    texture tex;

    // check for collision with dot
    bool check(SDL_Rect dot_rect);

    // activated or not
    bool activated;

    // single use
    bool single;

    // used
    bool used;

    // direction
    int direction;

    // animation status
    int status;

    // frame of animation
    int frame;

};

class crate
{
public:
    // velocity
    float x_vel;

    // for collision detection
    SDL_Rect get_col_rect() { return col_rect; };

    // check for wall collisions
    bool check_col(SDL_Rect crate, levelstate* level, vector* repos);

    // update function (aligns crate with tile-grid)
    void update();

    // render function
    void render(int b_status, SDL_Rect* camera, engine* game, levelstate* level);

    // crate texture
    texture tex;

    // type
    int crate_type;
    bool black;

    // pushed flag
    bool pushed;

    // initializer
    crate(int x, int y, int type);

    // tileset
    tile* tileset[MAX_BORDER];
    void create_tiles(int b_status, levelstate* level);
    bool generating;

    // determine which tiletype to render
    int tile_type_top(int type);
    int tile_type_bottom(int type);
    int tile_type_left(int type);
    int tile_type_right(int type);

    // collision rectangle
    SDL_Rect col_rect;
};

class springboard
{
public:
    springboard();

    void render(SDL_Rect* camera, engine* game);
    SDL_Rect col_rect;
    texture tex;

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
    void cross_spring(dot* springer, dot* springee, int type);

};

#endif /* object_hpp */