
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include header file
#include "characters.hpp"
#include "initialization.hpp"
#include "close.hpp"
#include "textures.hpp"
#include "collisions.hpp"
#include "tiles.hpp"
#include "camera.hpp"

// reinitialize character textures
extern texture b_char_tex;
extern texture w_char_tex;

dot::dot()
{
    // initialize velocity
    x_vel = 0;
    y_vel = 0;
    
    // initialize controllers
    up = false;
    down = false;
    right = false;
    left = false;
    
    // initialize active
    status = TILE_ACTIVE;
    
    // initialize animation
    frame = 0;
    
    // initialize collision rectangle
    col_rect.w = DOT_W;
    col_rect.h = DOT_H;
    col_rect.x = (SCREEN_WIDTH - col_rect.w) / 2;
    col_rect.y = (SCREEN_HEIGHT - col_rect.h) / 2;
}

bool dot::handle_event(SDL_Event &e)
{
    // handle those events, duder
    switch (e.type)
    {
    case SDL_KEYDOWN:
        switch (e.key.keysym.scancode)
        {
            case SDL_SCANCODE_UP:
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_SPACE:
                up = true;
                break;
            case SDL_SCANCODE_DOWN:
            case SDL_SCANCODE_S:
                down = true;
                break;
            case SDL_SCANCODE_LEFT:
            case SDL_SCANCODE_A:
                left = true;
                break;
            case SDL_SCANCODE_RIGHT:
            case SDL_SCANCODE_D:
                right = true;
                break;
            case SDL_SCANCODE_Q:
            case SDL_SCANCODE_LSHIFT:
            case SDL_SCANCODE_RSHIFT:
                status = (status + 1) % 4;
                break;
            case SDL_SCANCODE_ESCAPE:
                return false;
                break;
        }
        break;
    case SDL_KEYUP:
        switch (e.key.keysym.scancode)
        {
            case SDL_SCANCODE_UP:
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_SPACE:
                up = false;
                break;
            case SDL_SCANCODE_DOWN:
            case SDL_SCANCODE_S:
                down = false;
                break;
            case SDL_SCANCODE_LEFT:
            case SDL_SCANCODE_A:
                left = false;
                break;
            case SDL_SCANCODE_RIGHT:
            case SDL_SCANCODE_D:
                right = false;
                break;
        }
        break;
    }
    
    // success! (no quitting)
    return true;
}

void dot::move(tile* tiles[], int level_w, int level_h)
{
    
    // update y velocity with gravity
    black ? y_vel += GRAVITY : y_vel -= GRAVITY;
    
    // update x velocity
    if (right && !left)
        x_vel += DOT_ACC;
    if (left && !right)
        x_vel -= DOT_ACC;
    if ((!right && !left) || (right && left))
    {
        if (x_vel < 0)
            x_vel += DOT_ACC;
        else if (x_vel > 0)
            x_vel -= DOT_ACC;
    }
    
    // if the square is moving slowly enough, stop him
    if (x_vel < (DOT_ACC) && x_vel > -(DOT_ACC))
        x_vel = 0;
    
    
    // limit top speed
    if (x_vel > DOT_VEL)
        x_vel = DOT_VEL;
    if (x_vel < -DOT_VEL)
        x_vel = -DOT_VEL;
    if (y_vel > JUMP_VEL)
        y_vel = JUMP_VEL;
    if (y_vel < -JUMP_VEL)
        y_vel = -JUMP_VEL;
    
    if (status != CHAR_ACTIVE)
    {
        x_vel = 0;
        //y_vel = 0;
    }
    
    // MOVE THAT FUCKER
    col_rect.x += x_vel;
    col_rect.y += y_vel;
    
    // for collision detection
    vector repos;
    
    // check collisions with black character
    if (black)
    {
        // iterate over all tiles
        for (int i = 0, n = level_w * level_h; i < n; i++)
        {
            // store reposition vector
            if (check_collision(col_rect, tiles[i]->get_col_rect(), &repos))
            {
                // black wall
                if (tiles[i]->wall_b && !tiles[i]->floor_b && !tiles[i]->ceiling_b)
                {
                    col_rect.x += repos.x;
                    x_vel = 0;
                }
                // black floor
                else if (tiles[i]->floor_b && !tiles[i]->wall_b)
                {
                    col_rect.y += repos.y;
                    y_vel = 0;
                    
                    if (up && status == CHAR_ACTIVE)
                    {
                        y_vel -= JUMP_VEL;
                    }
                }
                // black floor edge
                else if (tiles[i]->floor_b && tiles[i]->wall_b)
                {
                    // determine which is smaller, and use that one!
                    if (abs(repos.x) < abs(repos.y))
                    {
                        //adjust x pos (lol expos)
                        col_rect.x += repos.x;
                        x_vel = 0;
                    }
                    else if (y_vel > 0)
                    {
                        // adjust y pos
                        col_rect.y += repos.y;
                        y_vel = 0;
                        
                        // jump! (if you want)
                        if (up && status == CHAR_ACTIVE)
                        {
                            y_vel -= JUMP_VEL;
                        }
                    }
                }
            
                // black ceiling
                else if (tiles[i]->ceiling_b && !tiles[i]->wall_b)
                {
                    // adjust y pos
                    col_rect.y += repos.y;
                    y_vel = 1;
                }
                
                // black ceiling edge
                else if (tiles[i]->ceiling_b && tiles[i]->wall_b)
                {
                    // determine which is smaller, and use that one!
                    if (abs(repos.x) < abs(repos.y))
                    {
                        //adjust x pos (lol expos)
                        col_rect.x += repos.x;
                        x_vel = 0;
                    }
                    else if (y_vel < 0)
                    {
                        // adjust y pos
                        col_rect.y += repos.y;
                        y_vel = 1;
                    }
                }
            }
        }
    }
    
    // check collisions with white character
    else if (!black)
    {
        // iterate over all tiles
        for (int i = 0, n = level_w * level_h; i < n; i++)
        {
            // store reposition vector
            if (check_collision(col_rect, tiles[i]->get_col_rect(), &repos))
            {
                // white wall
                if (tiles[i]->wall_w && !tiles[i]->floor_w && !tiles[i]->ceiling_w)
                {
                    col_rect.x += repos.x;
                    x_vel = 0;
                }
                
                // white floor
                else if (tiles[i]->floor_w && !tiles[i]->wall_w)
                {
                    col_rect.y += repos.y;
                    y_vel = 0;
                    
                    if (up && status == CHAR_ACTIVE)
                    {
                        y_vel += JUMP_VEL;
                    }
                }
                
                // white floor edge
                else if (tiles[i]->floor_w && tiles[i]->wall_w)
                {
                    // determine which is smaller, and use that one!
                    if (abs(repos.x) < abs(repos.y))
                    {
                        //adjust x pos (lol expos)
                        col_rect.x += repos.x;
                        x_vel = 0;
                    }
                    else if (y_vel < 0)
                    {
                        // adjust y pos
                        col_rect.y += repos.y;
                        y_vel = 0;
                        
                        // jump! (if you want)
                        if (up && status == CHAR_ACTIVE)
                        {
                            y_vel += JUMP_VEL;
                        }
                    }
                }
                
                // white ceiling
                else if (tiles[i]->ceiling_w && !tiles[i]->wall_w)
                {
                    // adjust y pos
                    col_rect.y += repos.y;
                    y_vel = -1;
                }
                
                // black ceiling edge
                else if (tiles[i]->ceiling_w && tiles[i]->wall_w)
                {
                    // determine which is smaller, and use that one!
                    if (abs(repos.x) < abs(repos.y))
                    {
                        //adjust x pos (lol expos)
                        col_rect.x += repos.x;
                        x_vel = 0;
                    }
                    else if (y_vel > 0)
                    {
                        // adjust y pos
                        col_rect.y += repos.y;
                        y_vel = -1;
                    }
                }
            }
        }
    }
    // check sides & ceiling
    if (col_rect.x < 0)
    {
        x_vel = 0;
        col_rect.x = 0;
    }
    if (col_rect.x + col_rect.w > level_w * TILE_WIDTH)
    {
        x_vel = 0;
        col_rect.x = level_w * TILE_WIDTH - col_rect.w;
    }
    if (col_rect.y < 0)
    {
        y_vel = 0;
        col_rect.y = 0;
    }
    if (col_rect.y + col_rect.h > level_h * TILE_WIDTH)
    {
        y_vel = 0;
        col_rect.y = level_h * TILE_WIDTH - col_rect.h;
    }
}

void dot::render(SDL_Rect* camera)
{
    SDL_Rect active_clip = {0, 0, 16, 16};
    SDL_Rect inactive_clip = {16 * 9, 0, 16, 16};
    
    switch (status)
    {
        case CHAR_ACTIVE:
            tex.render(col_rect.x, col_rect.y, &active_clip, camera);
            break;
        case CHAR_INACTIVE:
            tex.render(col_rect.x, col_rect.y, &inactive_clip, camera);
            break;
        case CHAR_INACTIVATE:
        {
            // next frame!
            frame++;
            
            // sprite sheet clipper
            SDL_Rect inactivate_clip = {16 * frame, 0, 16, 16};
            
            // render that mofo
            tex.render(col_rect.x, col_rect.y, &inactivate_clip, camera);
            
            // change the status if animation is over!
            if (frame == ANIMATION_LENGTH - 1)
            {
                frame = 0;
                status = (status + 1) % 4;
            }
            break;
        }
        case CHAR_ACTIVATE:
        {
            // next frame!
            frame++;
            
            // sprite sheet clipper
            SDL_Rect activate_clip = {16 * (frame + 8), 0, 16, 16};
            
            // render that mofo
            tex.render(col_rect.x, col_rect.y, &activate_clip, camera);
            
            // change the status if animation is over!
            if (frame == ANIMATION_LENGTH - 1)
            {
                frame = 0;
                status = (status + 1) % 4;
            }
            break;
        }
    }
};