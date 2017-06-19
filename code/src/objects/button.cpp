
// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


// include header
#include <engine.hpp>
#include <textures.hpp>
#include <char.hpp>
#include <objects.hpp>

// reinitialize textures
extern Texture b_button;
extern Texture w_button;

// button class
Button::Button()
{
    // initialize collision rectangle
    col_rect.w = 16;
    col_rect.h = 16;
    col_rect.x = 0;
    col_rect.y = 0;

    used = false;
    angle = 0.0;
    flip = SDL_FLIP_NONE;

    frame = 0;
}

// check for button collision
bool Button::check(SDL_Rect dot_rect)
{
    Vector repos;
    if(check_collision(col_rect, dot_rect, &repos))
    {
        return true;
    }
    return false;
};

// render
void Button::render(SDL_Rect* camera, Engine* game)
{
    // flip
    switch(direction) {
        case(UP):
            angle = 0.0;
            break;
        case(DOWN):
            angle = 180.0;
            break;
        case(RIGHT):
            angle = 90.0;
            break;
        case (LEFT):
            angle = 270.0;
            break;
        default:
            angle = 0.0;
            break;
    }

    // make sure it only animates if not single & used
    if(single && used)
        status = BUTT_ACTIVE;

    // relevant clips
    SDL_Rect inactive_clip = {0, 0, 16, 16};
    SDL_Rect active_clip = {16 * BUTT_ANIMATION_LENGTH, 0, 16, 16};


    // render relevant parts
    switch (status)
    {
        case BUTT_INACTIVE:
            tex.angle_render(col_rect.x, col_rect.y, &inactive_clip, camera, game, angle, center, flip);
            frame = 0;
            break;
        case BUTT_ACTIVE:
            tex.angle_render(col_rect.x, col_rect.y, &active_clip, camera, game, angle, center, flip);
            frame = 0;
            break;
        case BUTT_ACTIVATE:
        {
            // next frame!
            frame++;

            // sprite sheet clipper
            SDL_Rect activate_clip = {16 * frame, 0, 16, 16};

            // render that
            tex.angle_render(col_rect.x, col_rect.y, &activate_clip, camera, game, angle, center, flip);

            // change the status if animation is over!
            if (frame == BUTT_ANIMATION_LENGTH - 1)
            {
                frame = 0;
                status = (status + 1) % 4;
            }
            break;
        }
        case BUTT_INACTIVATE:
        {
            // next frame!
            frame++;

            // sprite sheet clipper
            SDL_Rect inactivate_clip = {(16 * (frame + 3)) , 0, 16, 16};

            // render that
            tex.angle_render(col_rect.x, col_rect.y, &inactivate_clip, camera, game, angle, center, flip);

            // change the status if animation is over!
            if (frame == BUTT_ANIMATION_LENGTH - 1)
            {
                frame = 0;
                status = (status + 1) % 4;
            }
            break;
        }
    }
};


