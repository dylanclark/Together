// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header file
#include <levels.hpp>
#include <textures.hpp>
#include <tiles.hpp>

Tile::Tile(int x, int y, TileType tile_type)
{
    // set position
    col_rect.x = x;
    col_rect.y = y;

    // set collision box size
    col_rect.w = TILE_WIDTH;
    col_rect.h = TILE_WIDTH;

    // set tile type
    m_type = tile_type;
}

// render tile
void Tile::render_bg(int active_color, SDL_Rect* camera, Engine* game, Texture* tile_tex)
{
    // activity clippers
    int clip_x;
    if (m_type == BLACK_SOLID || m_type == WHITE_SOLID) {
        clip_x = (2 * m_type + active_color) * TILE_WIDTH_TEX;
    } else if (m_type == GLASS) {
        clip_x = (2 - active_color) * TILE_WIDTH_TEX;
    }
    SDL_Rect active_clip = {clip_x, 0, TILE_WIDTH_TEX, TILE_WIDTH_TEX};
    tile_tex->render_tile(col_rect.x, col_rect.y, &active_clip, camera, game);
}

void Tile::render_fg(int active_color, SDL_Rect* camera, Engine* game, Texture* tile_tex)
{
    if (m_type == GLASS) {
        SDL_Rect active_clip = {(2*m_type + active_color) * TILE_WIDTH_TEX, 0, TILE_WIDTH_TEX, TILE_WIDTH_TEX};
        tile_tex->render_tile(col_rect.x, col_rect.y, &active_clip, camera, game, 90);
    }
}
