
#ifndef springboard_hpp
#define springboard_hpp

// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include header files
#include "characters.hpp"
#include "initialization.hpp"
#include "textures.hpp"
#include "collisions.hpp"
#include "tiles.hpp"
#include "camera.hpp"



// button status constants
const int BOARD_INACTIVE = 0;
const int BOARD_ACTIVATE = 1;
const int BOARD_ACTIVE = 2;
const int BOARD_INACTIVATE = 3;

// animation length
const int BOARD_ANIMATION_LENGTH = 6;

// direction
const int FLIP_LEFT = 0;
const int FLIP_RIGHT = 1;


class springboard
{
public:
    // initialize traits
    springboard();
    
    // render the item on the screen
    void render(SDL_Rect* camera, SDL_Renderer* rend);
    
    // collision rectangle
    SDL_Rect col_rect;
    
    // texture
    texture tex;
    
    // check for collision with dot
    bool check(SDL_Rect dot_rect);
    
    // activated or not
    bool activated;
    
    // shown or not
    bool show;
    
    // direction
    int direction;
    
    // animation status
    int status;
    
    // frame of animation
    int frame;
    
    // angle to flip
    double angle = 0.0;
    
    // flip axis point
    SDL_Point* center = NULL;
    
    // flip type
    SDL_RendererFlip flip_type;
    
    // spring velocity
    int x_spring;
    
    int y_spring;
    
    // default velocity and acceleration constants
    const float SPRING_X_VEL = 2;
    const float SPRING_Y_VEL = 10;

    
};

#endif /* springboard_hpp */

