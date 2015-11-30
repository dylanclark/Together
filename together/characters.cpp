
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include header file
#include "characters.hpp"
#include "initialization.hpp"
#include "textures.hpp"
#include "collisions.hpp"
#include "tiles.hpp"
#include "camera.hpp"
#include "level_end.hpp"
#include "levelstate.hpp"
#include "crate.hpp"
#include "springboard.hpp"


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

bool dot::handle_event(SDL_Event &e, levelstate* level, engine* game)
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
                if (level->shiftable)
                    status = (status + 1) % 4;
                break;
            case SDL_SCANCODE_R:
                game->restart_state();
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

void dot::move(levelstate* level)
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
    if (y_vel > 1.5 * JUMP_VEL)
        y_vel = 1.5 * JUMP_VEL;
    if (y_vel < 1.5 * -JUMP_VEL)
        y_vel = 1.5 * -JUMP_VEL;
    
    if (status != CHAR_ACTIVE)
    {
        x_vel = 0;
        y_vel = 0;
    }
    
    // MOVE THAT FUCKER
    col_rect.x += x_vel;
    col_rect.y += y_vel;
    
    if (!crate_col(level))
    {
        if (tile_col(level->tileset, level->width * level->height) && level->shiftable == true)
            level->shiftable = true;
        else
            level->shiftable = false;
    }
    else
    {
        level->shiftable = false;
    }
    
    // check edges
    if (col_rect.x < 0)
    {
        col_rect.x = 0;
        x_vel = 0;
    }
    if (col_rect.y < 0)
    {
        col_rect.y = 0;
        y_vel = 0;
    }
    if (col_rect.x + col_rect.w > level->width*TILE_WIDTH)
    {
        col_rect.x = level->width*TILE_WIDTH - col_rect.w;
        x_vel = 0;
    }
    if (col_rect.y + col_rect.h > level->height*TILE_WIDTH)
    {
        col_rect.y = level->height*TILE_WIDTH - col_rect.h;
        y_vel = 0;
    }
}

bool dot::tile_col(tile* tileset[], int size)
{
    vector repos;
    
    bool shiftable = false;
    
    if (black)
    {
        for (int i = 0, n = size; i < n; i++)
        {
            // store reposition vector
            if (check_collision(col_rect, tileset[i]->get_col_rect(), &repos))
            {
                // black wall
                if (tileset[i]->wall_b && !tileset[i]->floor_b && !tileset[i]->ceiling_b)
                {
                    col_rect.x += repos.x;
                    x_vel = 0;
                    
                    shiftable = false;
                }
                // black floor
                else if (tileset[i]->floor_b && !tileset[i]->wall_b)
                {
                    col_rect.y += repos.y;
                    y_vel = 0;
                    
                    shiftable = true;
                    
                    if (up && status == CHAR_ACTIVE)
                    {
                        y_vel -= JUMP_VEL;
                        shiftable = false;
                    }
                }
                // black floor edge
                else if (tileset[i]->floor_b && tileset[i]->wall_b)
                {
                    // determine which is smaller, and use that one!
                    if (abs(repos.x) <= abs(repos.y))
                    {
                        //adjust x pos (lol expos)
                        col_rect.x += repos.x;
                        x_vel = 0;
                        
                        shiftable = false;
                    }
                    else if (y_vel > 0)
                    {
                        // adjust y pos
                        col_rect.y += repos.y;
                        y_vel = 0;
                        
                        shiftable = true;
                        
                        // jump! (if you want)
                        if (up && status == CHAR_ACTIVE)
                        {
                            y_vel -= JUMP_VEL;
                            shiftable = false;
                        }
                    }
                }
                
                // black ceiling
                else if (tileset[i]->ceiling_b && !tileset[i]->wall_b)
                {
                    // adjust y pos
                    col_rect.y += repos.y;
                    y_vel = 1;
                    
                    shiftable = false;
                }
                
                // black ceiling edge
                else if (tileset[i]->ceiling_b && tileset[i]->wall_b)
                {
                    // determine which is smaller, and use that one!
                    if (abs(repos.x) <= abs(repos.y))
                    {
                        //adjust x pos (lol expos)
                        col_rect.x += repos.x;
                        x_vel = 0;
                        
                        shiftable = false;
                    }
                    else if (y_vel < 0)
                    {
                        // adjust y pos
                        col_rect.y += repos.y;
                        y_vel = 1;
                        
                        shiftable = false;
                    }
                }
            }
        }
        return shiftable;
    }
    
    // check collisions with white character
    else if (!black)
    {
        // iterate over all tiles
        for (int i = 0, n = size; i < n; i++)
        {
            // store reposition vector
            if (check_collision(col_rect, tileset[i]->get_col_rect(), &repos))
            {
                // white wall
                if (tileset[i]->wall_w && !tileset[i]->floor_w && !tileset[i]->ceiling_w)
                {
                    col_rect.x += repos.x;
                    x_vel = 0;
                    
                    shiftable = false;
                }
                
                // white floor
                else if (tileset[i]->floor_w && !tileset[i]->wall_w)
                {
                    col_rect.y += repos.y;
                    y_vel = 0;
                    
                    shiftable = true;
                    
                    if (up && status == CHAR_ACTIVE)
                    {
                        y_vel += JUMP_VEL;
                        shiftable = false;
                    }
                }
                
                // white floor edge
                else if (tileset[i]->floor_w && tileset[i]->wall_w)
                {
                    // determine which is smaller, and use that one!
                    if (abs(repos.x) <= abs(repos.y))
                    {
                        //adjust x pos (lol expos)
                        col_rect.x += repos.x;
                        x_vel = 0;
                        
                        shiftable = false;
                    }
                    else if (y_vel < 0)
                    {
                        // adjust y pos
                        col_rect.y += repos.y;
                        y_vel = 0;
                        
                        shiftable = true;
                        
                        // jump! (if you want)
                        if (up && status == CHAR_ACTIVE)
                        {
                            y_vel += JUMP_VEL;
                            shiftable = false;
                        }
                    }
                }
                
                // white ceiling
                else if (tileset[i]->ceiling_w && !tileset[i]->wall_w)
                {
                    // adjust y pos
                    col_rect.y += repos.y;
                    y_vel = -1;
                    
                    shiftable = false;
                }
                
                // black ceiling edge
                else if (tileset[i]->ceiling_w && tileset[i]->wall_w)
                {
                    // determine which is smaller, and use that one!
                    if (abs(repos.x) <= abs(repos.y))
                    {
                        //adjust x pos (lol expos)
                        col_rect.x += repos.x;
                        x_vel = 0;
                        
                        shiftable = false;
                    }
                    else if (y_vel > 0)
                    {
                        // adjust y pos
                        col_rect.y += repos.y;
                        y_vel = -1;
                        
                        shiftable = false;
                    }
                }
            }
        }
    }
    return shiftable;
}

bool dot::crate_col(levelstate* level)
{
    vector repos;

    // check all crates
    for (int i = 0; i < level->crates.size(); i++)
    {
        if (black)
        {
            level->crates[i]->pushed = false;
            
            if (check_collision(col_rect, level->crates[i]->get_col_rect(), &repos))
            {
                if (!level->crates[i]->black)
                {
                    tile_col(level->crates[i]->tileset, MAX_BORDER);
                    
                    level->shiftable = true;
                }
                else
                {
                    // push and move the crate
                    if (abs(repos.x) <= abs(repos.y))
                    {
                        // adjust dot speed
                        if (x_vel > PUSH_VEL || x_vel < -PUSH_VEL)
                            x_vel = (x_vel > 0) ? PUSH_VEL : -PUSH_VEL;
                        
                        // move crate
                        level->crates[i]->col_rect.x += x_vel;
                        level->crates[i]->x_vel = 0;
                        level->crates[i]->check_col(level->crates[i]->get_col_rect(), level, &repos);
                        
                        // correct dot position
                        check_collision(col_rect, level->crates[i]->get_col_rect(), &repos);
                        col_rect.x += repos.x;
                        
                        level->shiftable = false;
                        
                        // tell the crate its being pushed
                        level->crates[i]->pushed = true;
                    }
                    // land on crate
                    else if (y_vel > 0)
                    {
                        col_rect.y += repos.y;
                        y_vel = 0;
                        
                        level->shiftable = true;
                        
                        // jump! (if you want)
                        if (up && status == CHAR_ACTIVE)
                        {
                            y_vel -= JUMP_VEL;
                        }
                    }
                }
            }
        }
        else if (!black)
        {
            if (check_collision(col_rect, level->crates[i]->get_col_rect(), &repos))
            {
                
                
                if (level->crates[i]->black)
                {
                    tile_col(level->crates[i]->tileset, MAX_BORDER);
                    
                    return true;
                }
                else
                {
                    // push and move the crate
                    if (abs(repos.x) <= abs(repos.y))
                    {
                        // tell the crate its being pushed
                        level->crates[i]->pushed = true;
                        
                        // adjust dot speed
                        if (x_vel > PUSH_VEL || x_vel < -PUSH_VEL)
                            x_vel = (x_vel > 0) ? PUSH_VEL : -PUSH_VEL;
                        
                        // move crate
                        level->crates[i]->col_rect.x += x_vel;
                        level->crates[i]->x_vel = 0;
                        level->crates[i]->check_col(level->crates[i]->get_col_rect(), level, &repos);
                        
                        // correct dot position
                        check_collision(col_rect, level->crates[i]->get_col_rect(), &repos);
                        col_rect.x += repos.x;
                        
                        level->shiftable = false;
                    }
                    // land on crate
                    else if (y_vel > 0)
                    {
                        col_rect.y += repos.y;
                        y_vel = 0;
                        
                        level->shiftable = true;
                        
                        // jump! (if you want)
                        if (up && status == CHAR_ACTIVE)
                        {
                            y_vel -= JUMP_VEL;
                        }
                    }
                }
            }
        }
    }
    // if no collisions were detected, return false
    return false;
}

void dot::render(SDL_Rect* camera, SDL_Renderer* rend)
{
    SDL_Rect active_clip = {0, 0, 16, 16};
    SDL_Rect inactive_clip = {16 * 9, 0, 16, 16};
    
    switch (status)
    {
        case CHAR_ACTIVE:
            tex.render(col_rect.x, col_rect.y, &active_clip, camera, rend);
            break;
        case CHAR_INACTIVE:
            tex.render(col_rect.x, col_rect.y, &inactive_clip, camera, rend);
            break;
        case CHAR_INACTIVATE:
        {
            // next frame!
            frame++;
            
            // sprite sheet clipper
            SDL_Rect inactivate_clip = {16 * frame, 0, 16, 16};
            
            // render that mofo
            tex.render(col_rect.x, col_rect.y, &inactivate_clip, camera, rend);
            
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
            tex.render(col_rect.x, col_rect.y, &activate_clip, camera, rend);
            
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

// code for post-level animation (must be followed by level change or will mess shit up!)
void dot::completed(int width,int height, SDL_Rect* end_rect)
{
    // make them both big
    if(!CHAR_ACTIVE)
        status = CHAR_ACTIVE;
    
    // center dot on level-end object
    while (col_rect.x < end_rect->x )
        col_rect.x++;
    while (col_rect.x > end_rect->x )
        col_rect.x--;
    
    // make it jump (input actual animation here)
    black ? y_vel -= DOT_VEL : y_vel += DOT_VEL;
    
    return;
}

void dot::spring()
{
    status = CHAR_ACTIVE;
    
    black ? y_vel -= JUMP_VEL : y_vel += JUMP_VEL;
    
    x_vel += DOT_VEL;
}