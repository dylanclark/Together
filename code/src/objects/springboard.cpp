
// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


// include header
#include <levels/level_end.hpp>
#include <characters/characters.hpp>
#include <engine/init.hpp>
#include <textures/textures.hpp>
#include <characters/collisions.hpp>
#include <levels/tiles.hpp>
#include <levels/camera.hpp>
#include <objects/springboard.hpp>

// reinitialize textures
extern texture b_springboard;
extern texture w_springboard;
extern texture b_cross_layer;
extern texture w_cross_layer;

// board class
springboard::springboard()
{
    // initialize collision rectangle
    col_rect.w = 60;
    col_rect.h = 60;
    col_rect.x = (SCREEN_WIDTH - col_rect.w) / 2;
    col_rect.y = (SCREEN_HEIGHT - col_rect.h) / 2;

    // standard initializing traits
    show = false;
    angle = 0.0;
    x_spring = SPRING_X_VEL;
    y_spring = SPRING_Y_VEL;
    frame = 0;
}

// check for springboard collision
bool springboard::check(SDL_Rect dot_rect)
{
    vector repos;
    if(check_collision(col_rect, dot_rect, &repos))
    {
        if (abs(repos.x) > 20 && abs(repos.y) > 50)
        {
            return true;
        }
    }
    return false;
};

// render sprinboard
void springboard::render(SDL_Rect* camera, engine* game)
{
    // flip
    switch(direction)
    {
        // flip types for SDL_RenderCopyEx in angle_render
        case(FLIP_LEFT):
            flip_type = SDL_FLIP_VERTICAL;
            angle = 180;
            break;

        case(FLIP_RIGHT):
            flip_type = SDL_FLIP_NONE;
            break;

    }

    // relavent clips
    SDL_Rect inactive_clip = {0, 0, 16, 16};
    SDL_Rect active_clip = {16 * BOARD_ANIMATION_LENGTH, 0, 16, 16};

    // animating
    switch (status)
    {
        // relevant cases (resetting frame = 0 each time!)
        case BOARD_INACTIVE:
            tex.angle_render(col_rect.x, col_rect.y, &inactive_clip, camera, game, angle, center, flip_type);
            frame = 0;
            break;
        case BOARD_ACTIVE:
            tex.angle_render(col_rect.x, col_rect.y, &active_clip, camera, game, angle, center, flip_type);
            frame = 0;
            break;
        case BOARD_ACTIVATE:
        {
            // next frame!
            frame++;

            // sprite sheet clipper
            SDL_Rect activate_clip = {16 * frame, 0, 16, 16};

            // render that
            tex.angle_render(col_rect.x, col_rect.y, &activate_clip, camera, game, angle, center, flip_type);

            // change the status if animation is over!
            if (frame == BOARD_ANIMATION_LENGTH - 1)
            {
                frame = 0;
                status = (status + 1) % 4;
            }
            break;
        }
        case BOARD_INACTIVATE:
        {
            // next frame!
            frame++;

            // sprite sheet clipper
            SDL_Rect inactivate_clip = {16 * (frame + 6), 0, 16, 16};

            // render that mofo
            tex.angle_render(col_rect.x, col_rect.y, &inactivate_clip, camera, game, angle, center, flip_type);

            // change the status if animation is over!
            if (frame == BOARD_ANIMATION_LENGTH - 1)
            {
                frame = 0;
                status = (status + 1) % 4;
            }
            break;
        }
    }
};

// cross spring logic
void springboard::cross_spring(dot* springer, dot* springee, int type)
{
    activated = true;
    // if VELOCITY or POSITION spring
    if(type == VELOCITY)
    {
        // if there is a y velocity (cant be stationary)
        if(springer->get_y_vel() < 0)
        {
            // animate
            if(status == BOARD_INACTIVE)
            {
                status = (status + 1) % 4;
            }

            // get spring amount
            y_spring = -(springer->get_y_vel() / 3);

            // spring
            springee->spring(x_spring, y_spring, direction);

            // animate
            if(springer->status == CHAR_ACTIVE)
            {
                springer->status = CHAR_INACTIVATE;
            }

            // make sure dot goes to right spot
            if(springee->black)
            {
                while(springer->col_rect.y > (col_rect.y + 50))
                    springer->col_rect.y--;
            }
            else
            {
                while(springer->col_rect.y < (col_rect.y - 50))
                    springer->col_rect.y++;
            }
        }
    }
    else
    {
        // animate
        if(status == BOARD_INACTIVE)
        {
            status = (status + 1) % 4;
        }

        // spring
        springee->spring(x_spring, y_spring, direction);


        // animate
        if(springer->status == CHAR_ACTIVE)
        {
            springer->status = CHAR_INACTIVATE;
        }

        // make sure in right spot
        if(springee->black)
        {
            while(springer->col_rect.y > (col_rect.y + 50))
                springer->col_rect.y--;
        }
        else
        {
            while(springer->col_rect.y < (col_rect.y - 50))
                springer->col_rect.y++;
        }
    }
}