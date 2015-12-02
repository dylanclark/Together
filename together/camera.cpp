
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include header files
#include "camera.hpp"
#include "characters.hpp"
#include "initialization.hpp"
#include "level_draw.hpp"
#include "gamestate.hpp"
#include "levelstate.hpp"

camera::camera(int w, int h)
{
    // initialize velocities
    x_vel = 0;
    y_vel = 0;
    w_vel = 0;
    h_vel = 0;
    
    // initialize rectangle
    display = {0, 0, w, h};
};

void camera::move(int level_w, int level_h)
{
    // update positions
    location.x += x_vel;
    location.y += y_vel;
    location.w += w_vel;
    location.h += h_vel;
    
    // don't shrink too small
    if (location.w < SCREEN_WIDTH)
    {
        location.w = SCREEN_WIDTH;
        w_vel = 0;
    }
    if (location.h < SCREEN_HEIGHT)
    {
        location.h = SCREEN_HEIGHT;
        h_vel = 0;
    }
    
    // don't get too big (max dimensions)
    if (location.w > SCREEN_WIDTH * 2 || location.h > SCREEN_HEIGHT * 2)
    {
        location.w = SCREEN_WIDTH * 2;
        location.h = SCREEN_HEIGHT * 2;
    }
    
    // don't get too big (level dimensions)
    if (location.w > level_w * TILE_WIDTH)
    {
        location.w = level_w * TILE_WIDTH;
        location.x = location.w / 2;
        w_vel = 0;
        h_vel = 0;
        x_vel = 0;
    }
    if (location.h > level_h * TILE_WIDTH)
    {
        location.h = level_h * TILE_WIDTH;
        location.y = location.h / 2;
        h_vel = 0;
        w_vel = 0;
        y_vel = 0;
    }
    
    // keep correct aspect ratio
    if ((float) location.w / (float) SCREEN_WIDTH > (float) location.h / (float) SCREEN_WIDTH)
    {
        location.h = ((float) location.w / (float) SCREEN_WIDTH) * SCREEN_HEIGHT;
    }
    if ((float) location.w / (float) SCREEN_WIDTH < (float) location.h / (float) SCREEN_WIDTH)
    {
        location.w = ((float) location.h / (float) SCREEN_HEIGHT) * SCREEN_WIDTH;
    }
    
    
    if (location.x - location.w / 2 < 0)
    {
        location.x = location.w / 2;
        x_vel = 0;
    }
    if (location.x + location.w / 2 > level_w * TILE_WIDTH)
    {
        location.x = level_w * TILE_WIDTH - location.w / 2;
        x_vel = 0;
    }
    if (location.y - location.h / 2 < 0)
    {
        location.y = location.h / 2;
        y_vel = 0;
    }
    if (location.y + location.h / 2 > level_h * TILE_WIDTH)
    {
        location.y = level_h * TILE_WIDTH - location.h / 2;
        y_vel = 0;
    }
    
    
    

    // change the actual positions based on this origin
    update();
}

void camera::update()
{
    display.x = location.x - location.w / 2;
    display.y = location.y - location.h / 2;
    display.w = location.w;
    display.h = location.h;
}

void camera::track(SDL_Rect* b_char, SDL_Rect* w_char)
{
    // these will be for our ideal camera position (x and y are centered)
    int min_x;
    int max_x;
    int min_y;
    int max_y;
    
    // x bounds
    if (b_char->x >= w_char->x)
    {
        max_x = b_char->x + b_char->w / 2;
        min_x = w_char->x + w_char->w / 2;
    }
    else if (b_char->x < w_char->x)
    {
        max_x = w_char->x + w_char->w / 2;
        min_x = b_char->x + b_char->w / 2;
    }
    // y bounds
    if (b_char->y >= w_char->y)
    {
        max_y = b_char->y + b_char->h / 2;
        min_y = w_char->y + w_char->h / 2;
    }
    else if (b_char->y < w_char->y)
    {
        max_y = w_char->y + w_char->h / 2;
        min_y = b_char->y + b_char->h / 2;
    }
    
    // ideal camera position
    SDL_Rect target_rect;
    target_rect.x = (min_x + max_x) / 2;
    target_rect.y = (min_y + max_y) / 2;
    target_rect.w = max_x - min_x + BUFFER * 2;
    target_rect.h = max_y - min_y + BUFFER * 2;
    
    // update speeds!
    x_vel = CAM_ACC * ((float) target_rect.x - (float) location.x) / 20;
    y_vel = CAM_ACC * ((float) target_rect.y - (float) location.y) / 20;
    w_vel = CAM_ACC * ((float) target_rect.w - (float) location.w) / 20;
    h_vel = CAM_ACC * ((float) target_rect.h - (float) location.h) / 20;
}


