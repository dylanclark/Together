

#ifndef camera_hpp
#define camera_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include character header file for dot class
#include <characters/characters.hpp>
#include <engine/engine.hpp>
#include <levels/level_draw.hpp>

class levelstate;

// camera edge buffer
const float BUFFER = 400;

// speed constants
const float SPEED = 14;
const float CAM_ACC = 2;

class camera
{
public:
    // the actual rectangle that is the camera's viewport
    SDL_Rect display;

    // the location information (included to aid in smooth camera)
    SDL_Rect location;

    // velocities for each changing feature of the rectangle
    float x_vel;
    float y_vel;
    float w_vel;
    float h_vel;

    // initialize
    camera(int w, int h);

    // track a character
    void track(SDL_Rect* b_char, SDL_Rect* w_char);

    // move the camera location
    void move(int level_w, int level_h, engine* game);

    // update camera display
    void update(void);
};

#endif /* camera_hpp */
