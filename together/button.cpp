
// using SDL and standard IO
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
#include "button.hpp"

// reinitialize textures
extern texture b_button;
extern texture w_button;

// button class
button::button()
{
    // initialize collision rectangle
    col_rect.w = 16;
    col_rect.h = 6;
    col_rect.x = (SCREEN_WIDTH - col_rect.w) / 2;
    col_rect.y = (SCREEN_HEIGHT - col_rect.h) / 2;
    
}

// draw on screen
void button::render(SDL_Rect* camera, SDL_Renderer* rend)
{
    SDL_Rect active_clip = {0, 0, 16, 16};
    SDL_Rect inactive_clip;
    tex.render(col_rect.x, col_rect.y, &active_clip, camera, rend);
};

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


