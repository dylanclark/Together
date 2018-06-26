// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header file
#include <levels.hpp>
#include <textures.hpp>

// external variables
extern Texture tile_tex;
extern SDL_Rect tile_clips[];

Tile::Tile(int x, int y, int tile_type)
{
    // set position
    col_rect.x = x;
    col_rect.y = y;

    // set collision box size
    col_rect.w = TILE_WIDTH;
    col_rect.h = TILE_WIDTH;

    // set tile type
    type = tile_type;

    // set activity
    if (type >= 0 && type <= 14)
    {
        status = TILE_ACTIVE;
        frame = 0;
    }
    else if (type >= 15 && type <= 29)
    {
        status = TILE_INACTIVE;
        frame = TILE_FRAMES - 1;
    }
    // animation DEBUG
    animating = false;

    // start with this
    floor = wall = ceiling = false;

    // define floor properties (corners to come to smooth some stuff out, biznatches)
    switch (type)
    {
        case B_FLOOR:
            floor = true;
            break;
        case B_FLOOREDGE_L:
        case B_FLOOREDGE_R:
            floor = true;
            wall = true;
            break;
        case B_WALL_L:
        case B_WALL_R:
            wall = true;
            break;
        case B_CEILING:
            ceiling = true;
            break;
        case B_CEILINGEDGE_L:
        case B_CEILINGEDGE_R:
            ceiling = true;
            wall = true;
            break;
        case W_FLOOR:
            floor = true;
            break;
        case W_FLOOREDGE_L:
        case W_FLOOREDGE_R:
            ceiling = true;
            wall = true;
            break;
        case W_WALL_L:
        case W_WALL_R:
            wall = true;
            break;
        case W_CEILING:
            floor = true;
            break;
        case W_CEILINGEDGE_L:
        case W_CEILINGEDGE_R:
            floor = true;
            wall = true;
            break;
    }
    my_color = (type >= W_BACK);
}

// render tile
void Tile::render(int active_color, SDL_Rect* camera, Engine* game, Texture* tile_tex)
{
    status = (my_color == active_color) ? TILE_ACTIVE : TILE_INACTIVE;

    // activity clippers
    SDL_Rect active_clip = {(2 * my_color + (my_color != active_color)) * TILE_WIDTH_TEX, 0, TILE_WIDTH_TEX, TILE_WIDTH_TEX};
    tile_tex->render_tile(col_rect.x, col_rect.y, &active_clip, camera, game);
}
