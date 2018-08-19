
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
    TILE_BLACK_SOLID = 0,
    TILE_WHITE_SOLID,
    TILE_CLEAR,
    TILE_SOLID,
    TILE_BLACK_PLATFORM,
    TILE_WHITE_PLATFORM,
    TILE_SPIKES_FLOOR,
    TILE_SPIKES_CEILING,
    TILE_SPIKES_LEFT,
    TILE_SPIKES_RIGHT,
    TILE_SLOPE_2_UP_A,
    TILE_SLOPE_2_UP_B,
    TILE_SLOPE_2_DOWN_A,
    TILE_SLOPE_2_DOWN_B,
    TILE_SLOPE_PAD_BLACK,
    TILE_SLOPE_PAD_WHITE,
    TILE_INVISIBLE,
} TileType;

bool tiletype_isslope(TileType type);

// class definition
class Tile
{
public:
    Tile(int x, int y, TileType tile_type);

    void render_bg(Camera* cam, int active_color, Texture* tile_tex);
    void render_fg(Camera* cam, int active_color, Texture* tile_tex);
    TileType get_type() { return m_type; }
    void set_type(TileType type) { m_type = type; }
    SDL_Rect get_col_rect() { return col_rect; }

private:
    SDL_Rect col_rect;
    TileType m_type;
};

#endif /* tiles_hpp */
