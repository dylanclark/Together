
#ifndef tiles_hpp
#define tiles_hpp

#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <textures.hpp>

// tile dimensions!
static const int TILE_WIDTH = 8;
static const int TILE_WIDTH_TEX = 8;

typedef enum _TileType {
    TILE_BLACK = 0,
    TILE_WHITE,
    TILE_GLASS,
    TILE_BRICK
} TileType;

// class definition
class Tile
{
public:
    Tile(int x, int y, TileType tile_type);

    void render_bg(int active_color, SDL_Rect* camera, Engine* game, Texture* tile_tex);
    void render_fg(int active_color, SDL_Rect* camera, Engine* game, Texture* tile_tex);
    TileType get_type() { return m_type; }
    SDL_Rect get_col_rect() { return col_rect; }

private:
    SDL_Rect col_rect;
    TileType m_type;
};

#endif /* tiles_hpp */
