
#ifndef characters_hpp
#define characters_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
#include <textures/textures.hpp>
#include <levels/tiles.hpp>
#include <levels/level_end.hpp>
#include <levels/camera.hpp>
#include <engine/engine.hpp>
#include <engine/gamepad.hpp>

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

    // width and height constants
    static const int DOT_W = TILE_WIDTH;
    static const int DOT_H = TILE_WIDTH;

    // velocity and acceleration constants
    const float DOT_VEL = 12;
    const float JUMP_VEL = 18;
    const float DOT_ACC = DOT_VEL / 12;
    const float GRAVITY = JUMP_VEL / 26;
    const float PUSH_VEL = DOT_VEL / 4;

    // controller bools
    bool up, down, left, right;

    // gamepad
    controller* controller;

    // activity!
    int status;

    // color
    bool color;

    // frame of animation
    int frame;

    // initialize traits
    dot();

    // handle input (bool for quit registration)
    bool handle_event(SDL_Event &e, levelstate* level, engine* game);

    // move the square
    void move(levelstate* level, engine* game);
    bool crate_col(levelstate* level, engine* game);
    bool tile_col(tile* tileset[], int size, engine* game);

    // render the square on the screen
    void render(SDL_Rect* camera, engine* game);

    // collision rectangle
    SDL_Rect col_rect;

    // texture
    texture tex;

    // color
    bool black;

    // jumps
    int jump;
<<<<<<< HEAD
<<<<<<< HEAD:together/characters.hpp
    
<<<<<<< HEAD
    // end of level animation
    void completed(int width, int height, SDL_Rect* end_rect);
    
    // spring
    void spring();
=======
=======

>>>>>>> 201bf90... gigantic makefile revamp:code/include/characters/characters.hpp
=======

>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
    // spring
    void spring(int x, int y, int direction);

    // center on object
    bool center(SDL_Rect* end_rect);

    float get_y_vel();

<<<<<<< HEAD
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
private:

    // velocity variables
    float x_vel, y_vel;
};

#endif /* characters_hpp */
