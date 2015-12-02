
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
#include "gamepad.hpp"
#include "springboard.hpp"


// reinitialize character textures
extern texture b_char_tex;
extern texture w_char_tex;
extern texture b_end_animate;
extern texture w_end_animate;

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
    
    // initiliaze gamepad
    controller = new class controller;
    
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
        case SDL_JOYAXISMOTION:
            if (e.jaxis.which == 0)
            {
                switch (e.jaxis.axis)
                {
                    case 0:
                        if (e.jaxis.value > controller->DEAD_ZONE)
                        {
                            right = true;
                            left = false;
                            break;
                        }
                        else if (e.jaxis.value < -(controller->DEAD_ZONE))
                        {
                            left = true;
                            right = false;
                            break;
                        }
                        else
                        {
                            left = false;
                            right = false;
                            break;
                        }
                        break;
                    case 1:
                        if (e.jaxis.value > controller->DEAD_ZONE)
                        {
                            down = true;
                            break;
                        }
                        else if (e.jaxis.value < -(controller->DEAD_ZONE))
                        {
                            // up = true;
                            break;
                        }
                        else
                        {
                            // up = false;
                            down = false;
                            break;
                        }
                        break;
                }
            }
            break;
        case SDL_CONTROLLERBUTTONDOWN:
            switch (e.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_A:
                    up = true;
                    break;
                case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                case SDL_CONTROLLER_BUTTON_X:
                    if (level->shiftable)
                        status = (status + 1) % 4;
                    break;
                case SDL_CONTROLLER_BUTTON_Y:
                    game->restart_state();
                    break;
                case SDL_CONTROLLER_BUTTON_START:
                    return false;
                    break;
            }
            break;
        case SDL_CONTROLLERBUTTONUP:
            switch (e.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_A:
                    up = false;
                    break;
        }
            
                    
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
    if (col_rect.x + col_rect.w > level->width * TILE_WIDTH)
    {
        col_rect.x = level->width * TILE_WIDTH - col_rect.w;
        x_vel = 0;
    }
    if (col_rect.y + col_rect.h > level->height * TILE_WIDTH)
    {
        col_rect.y = level->height * TILE_WIDTH - col_rect.h;
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
                    if (col_rect.y + col_rect.h / 2 >= tileset[i]->get_col_rect().y + tileset[i]->get_col_rect().h / 2)
                        continue;
                    
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
                        if (col_rect.y + col_rect.h / 2 >= tileset[i]->get_col_rect().y + tileset[i]->get_col_rect().h / 2)
                            continue;
                        
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
                    if (col_rect.y + col_rect.h / 2 <= tileset[i]->get_col_rect().y + tileset[i]->get_col_rect().h / 2)
                        continue;
                    
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
                        if (col_rect.y + col_rect.h / 2 <= tileset[i]->get_col_rect().y + tileset[i]->get_col_rect().h / 2)
                            continue;
                        
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
                        level->crates[i]->check_col(level->crates[i]->get_col_rect(), level, &repos);
                        level->crates[i]->x_vel = 0;
                        
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

// code for post-level animation
void dot::completed(int width,int height, int frame)
{
    // make them both big
    if(!CHAR_ACTIVE)
        status = CHAR_ACTIVE;

    if (frame < 10 || (40 <= frame && frame < 50) || (80 <= frame && frame < 90) || (130 <= frame && frame < 140))
    {
        black ? col_rect.x-- : col_rect.x++;
    }
    if ((20 <= frame && frame < 30) || (60 <= frame && frame < 70) || (100 <= frame && frame < 120)) {
        black ? col_rect.x++ : col_rect.x--;
    }
    if ((frame < 15 || (60 <= frame && frame < 75) || (120 <= frame && frame < 135))) {
        black ? col_rect.y-- : col_rect.y++;
    }
    if ((30 <= frame && frame < 45) || (90 <= frame && frame < 105) || (150 <= frame && frame < 160)) {
        black ? col_rect.y++ : col_rect.y--;
    }
    if (frame > 160 && frame <= 190)
    {
        black ? col_rect.y++ : col_rect.y--;
    }
    if (frame > 190)
        col_rect.y--;
    if (frame > 220)
        col_rect.x += 2;
    
    return;
}

void dot::spring(int x, int y, int direction)
{
    status = CHAR_ACTIVE;
    
    black ? y_vel -= y : y_vel += y;
    
    if(direction == FLIP_RIGHT)
        x_vel += x;
    else
        x_vel -= x;
}

bool dot::center(SDL_Rect* end_rect)
{
    // center dot on level-end object
    while (col_rect.x < end_rect->x )
        col_rect.x++;
    while (col_rect.x > end_rect->x )
        col_rect.x--;
    while (col_rect.y < end_rect->y )
        col_rect.y++;
    while (col_rect.y > end_rect->y )
        col_rect.y--;
    
    return true;
}