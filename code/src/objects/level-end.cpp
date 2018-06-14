
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

// level end class
LevelEnd::LevelEnd(int x, int y, bool color, SDL_Renderer* rend, SDL_Color* palette)
{
    // initialize collision rectangle
    col_rect.w = TILE_WIDTH;
    col_rect.h = TILE_WIDTH;
    col_rect.x = x * TILE_WIDTH;
    col_rect.y = y * TILE_WIDTH;

    if (color == 0) {
        printf("hmm");
        bool success = tex.load_tile_sheet("black/level_end/black_end.png", rend, palette);
        if (!success) {
            printf("Failed to load black level end texture! SDL_Error: %s\n", SDL_GetError());
            return;
        }
        printf("hmm");
    } else {
        if (!tex.load_tile_sheet("white/level_end/white_end.png", rend, palette)) {
            printf("Failed to load white level end texture!\n");
            return;
        }
    }
    printf("initialized!");
}

void LevelEnd::move(int x, int y)
{
    col_rect.x = x * TILE_WIDTH;
    col_rect.y = y * TILE_WIDTH;
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
    if (check_collision(col_rect, dot_rect, &repos)) {
        if (abs(repos.x) > 40 && abs(repos.y) > 40) {
            return true;
        }
    }
    return false;
};

