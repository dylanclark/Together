
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

class Light;

bool tiletype_isslope(TileType type);

// class definition
class Tile
{
public:
    Tile(int x, int y, TileType tile_type);

    TileType get_type() { return m_type; }
    SDL_Rect get_rect() { return m_rect; }
    void set_type(TileType type) { m_type = type; }

private:
    SDL_Rect m_rect;
    TileType m_type;
};

class Tileset
{
public:
    Tileset(std::vector<Tile> &tiles, int x, int y, int w, int h, SDL_Color palette);
    void render(Camera* cam, std::vector<Light> lights, bool active_color);
    std::vector<Tile> get_tiles() { return m_tiles; }
    void set_type(int idx, TileType type) { m_tiles[idx].set_type(type); }

private:
    int m_x, m_y;
    int m_w, m_h;
    int tex_width, tex_height;
    std::vector<Tile> m_tiles;

    SDL_Color m_palette;

    GLuint m_tex;
    GLuint m_normalmap;
    GLuint m_vao;
};

#endif /* tiles_hpp */
