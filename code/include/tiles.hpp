
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

typedef enum _tilestatus {
    TILE_ACTIVE = 0,
    TILE_INACTIVATE = 1,
    TILE_INACTIVE = 2,
    TILE_ACTIVATE = 3
} tilestatus;

#define TOTAL_TILES 30
#define TILE_FRAMES 10

// class definition
class Tile
{
public:
    Tile(int x, int y, TileType tile_type);

    void render_bg(int active_color, SDL_Rect* camera, Engine* game, Texture* tile_tex);
    void render_fg(int active_color, SDL_Rect* camera, Engine* game, Texture* tile_tex);
    int get_type() { return m_type; }
    SDL_Rect get_col_rect() { return col_rect; }

    // edges and floors
    bool floor;
    bool wall;
    bool ceiling;

    int frame;
    int m_type;

private:
    SDL_Rect col_rect;
    bool animating;
};

// close the tilemap
void close_tiles(Tile* tiles[]);

#endif /* tiles_hpp */
