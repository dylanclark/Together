
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

// reinitialize textures
extern texture b_level_end;
extern texture w_level_end;

// level end class
level_end::level_end()
{
    // initialize collision rectangle
    col_rect.w = 16;
    col_rect.h = 16;
    col_rect.x = (SCREEN_WIDTH - col_rect.w) / 2;
    col_rect.y = (SCREEN_HEIGHT - col_rect.h) / 2;
    
}

// draw on screen
void level_end::render(SDL_Rect* camera, engine* game)
{
    SDL_Rect active_clip = {0, 0, 16, 16};
    tex.render(col_rect.x, col_rect.y, &active_clip, camera, game);
};

// check for level end collision
bool level_end::check(SDL_Rect dot_rect)
{
    vector repos;
    if(check_collision(col_rect, dot_rect, &repos))
    {
        return true;
    }
    return false;
};

