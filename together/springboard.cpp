
// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>


// include header
#include "level_end.hpp"
#include "characters.hpp"
#include "initialization.hpp"
#include "textures.hpp"
#include "collisions.hpp"
#include "tiles.hpp"
#include "camera.hpp"
#include "springboard.hpp"

// reinitialize textures
<<<<<<< HEAD
extern texture b_board;
extern texture w_board;
=======
extern texture b_springboard;
extern texture w_springboard;
extern texture b_cross_layer;
extern texture w_cross_layer;
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,

// button class
springboard::springboard()
{
    // initialize collision rectangle
    col_rect.w = 60;
    col_rect.h = 60;
    col_rect.x = (SCREEN_WIDTH - col_rect.w) / 2;
    col_rect.y = (SCREEN_HEIGHT - col_rect.h) / 2;
    
<<<<<<< HEAD
=======
    show = false;
    
    angle = 0.0;
    
    x_spring = SPRING_X_VEL;
    
    y_spring = SPRING_Y_VEL;
    
    frame = 0;
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
}

// check for button collision
bool springboard::check(SDL_Rect dot_rect)
{
    vector repos;
    
    if(check_collision(col_rect, dot_rect, &repos))
    {
        return true;
    }
    return false;
};


void springboard::render(SDL_Rect* camera, engine* game)
{
    // flip
    switch(direction)
    {
        case(LEFT):
            flip = SDL_FLIP_HORIZONTAL;
            
        case(RIGHT):
            flip = SDL_FLIP_NONE;
            
    }
    
    SDL_Rect inactive_clip = {0, 0, 16, 16};
    SDL_Rect active_clip = {16 * BOARD_ANIMATION_LENGTH, 0, 16, 16};
    
    
    switch (status)
    {
        case BOARD_INACTIVE:
<<<<<<< HEAD
<<<<<<< HEAD
            tex.angle_render(col_rect.x, col_rect.y, &inactive_clip, camera, rend, angle, center, flip);
            break;
        case BOARD_ACTIVE:
            tex.angle_render(col_rect.x, col_rect.y, &active_clip, camera, rend, angle, center, flip);
=======
            tex.angle_render(col_rect.x, col_rect.y, &inactive_clip, camera, rend, angle, center, flip_type);
=======
            tex.angle_render(col_rect.x, col_rect.y, &inactive_clip, camera, game, angle, center, flip_type);
>>>>>>> 4ff27ea... Finished dynamic camera! (finally)
            frame = 0;
            break;
        case BOARD_ACTIVE:
            tex.angle_render(col_rect.x, col_rect.y, &active_clip, camera, game, angle, center, flip_type);
            frame = 0; // do this elsewhere!
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
            break;
        case BOARD_ACTIVATE:
        {
            // next frame!
            frame++;
            
            // sprite sheet clipper
            SDL_Rect activate_clip = {16 * frame, 0, 16, 16};
            
            // render that mofo
<<<<<<< HEAD
            tex.angle_render(col_rect.x, col_rect.y, &activate_clip, camera, rend, angle, center, flip);
=======
            tex.angle_render(col_rect.x, col_rect.y, &activate_clip, camera, game, angle, center, flip_type);
>>>>>>> 4ff27ea... Finished dynamic camera! (finally)
            
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
            SDL_Rect inactivate_clip = {16 * (frame + 8), 0, 16, 16};
            
            // render that mofo
<<<<<<< HEAD
            tex.angle_render(col_rect.x, col_rect.y, &inactivate_clip, camera, rend, angle, center, flip);
=======
            tex.angle_render(col_rect.x, col_rect.y, &inactivate_clip, camera, game, angle, center, flip_type);
>>>>>>> 4ff27ea... Finished dynamic camera! (finally)

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

void springboard::cross_spring(dot* springer, dot* springee, int type)
{
    activated = true;
    
    if(type == VELOCITY)
    {
        if(springer->get_y_vel() < 0)
        {
            
            if(status == BOARD_INACTIVE)
            {
                status = (status + 1) % 4;
            }
            
            y_spring = -(springer->get_y_vel());
            
            springee->spring(x_spring, y_spring, direction);
            
            
            if(springer->status == CHAR_ACTIVE)
            {
                springer->status = CHAR_INACTIVATE;
            }
            
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
        if(status == BOARD_INACTIVE)
        {
            status = (status + 1) % 4;
        }
        
        springee->spring(x_spring, y_spring, direction);
        
        
        if(springer->status == CHAR_ACTIVE)
        {
            springer->status = CHAR_INACTIVATE;
        }
        
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
