
#ifndef button_hpp
#define button_hpp

// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
#include <characters/characters.hpp>
#include <engine/init.hpp>
#include <textures/textures.hpp>
#include <characters/collisions.hpp>
#include <levels/tiles.hpp>
#include <levels/camera.hpp>


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

#endif /* button_hpp */

