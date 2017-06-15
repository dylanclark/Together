
// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


// include header
#include <engine.hpp>
#include <textures/textures.hpp>
#include <characters/collisions.hpp>
#include <objects/button.hpp>

// reinitialize textures
extern texture b_button;
extern texture w_button;

// button class
button::button()
{
    // initialize collision rectangle
    col_rect.w = 16;
    col_rect.h = 16;
    col_rect.x = (SCREEN_WIDTH - col_rect.w) / 2;
    col_rect.y = (SCREEN_HEIGHT - col_rect.h) / 2;

    used = false;

    frame = 0;
}

// check for button collision
bool button::check(SDL_Rect dot_rect)
{
    vector repos;
    if(check_collision(col_rect, dot_rect, &repos))
    {
        return true;
    }
    return false;
};

// render
void button::render(SDL_Rect* camera, engine* game)
{
    // flip
    switch(direction)
    {
        case(UP):
        {
            angle = 0.0;
            break;
        }
        case(DOWN):
        {
            angle = 180.0;
            break;
        }
        case(RIGHT):
        {
            angle = 90.0;
            break;
        }
        case (LEFT):
        {
            angle = 270.0;
            break;
        }
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


