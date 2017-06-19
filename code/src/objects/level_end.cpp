
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header
#include <levels.hpp>
#include <engine.hpp>
#include <textures.hpp>
#include <char.hpp>
#include <levels.hpp>

// reinitialize textures
extern Texture b_level_end;
extern Texture w_level_end;

// level end class
LevelEnd::LevelEnd()
{
    // initialize collision rectangle
    col_rect.w = TILE_WIDTH;
    col_rect.h = TILE_WIDTH;
    col_rect.x = 0;
    col_rect.y = 0;

}

// draw on screen
void LevelEnd::render(SDL_Rect* camera, Engine* game)
{
    SDL_Rect active_clip = {0, 0, 16, 16};
    tex.render(col_rect.x, col_rect.y, &active_clip, camera, game);
};

// check for level end collision
bool LevelEnd::check(SDL_Rect dot_rect)
{
    Vector repos;
    if(check_collision(col_rect, dot_rect, &repos))
    {
        if (abs(repos.x) > 40 && abs(repos.y) > 40)
        {
            return true;
        }
    }
    return false;
};

