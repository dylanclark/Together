
#ifndef tiles_hpp
#define tiles_hpp

#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <textures.hpp>

// tile dimensions!
static const int TILE_WIDTH = 16;
static const int TILE_WIDTH_TEX = 16;

typedef enum _tiletype {
    B_BACK = 0,
    B_FLOOR,
    B_FLOOREDGE_R,
    B_FLOOREDGE_L,
    B_CEILING,
    B_CEILINGEDGE_R,
    B_CEILINGEDGE_L,
    B_WALL_R,
    B_WALL_L,
    B_CORNER_BL,
    B_CORNER_BR,
    B_CORNER_TL,
    B_CORNER_TR,
    W_BACK,
    W_FLOOR,
    W_FLOOREDGE_R,
    W_FLOOREDGE_L,
    W_CEILING,
    W_CEILINGEDGE_R,
    W_CEILINGEDGE_L,
    W_WALL_R,
    W_WALL_L,
    W_CORNER_BL,
    W_CORNER_BR,
    W_CORNER_TL,
    W_CORNER_TR
} tiletype;

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
    Tile(int x, int y, int tile_type);

    void render(int b_status, SDL_Rect* camera, Engine* game, Texture* tile_tex);
    int get_type() { return type; }
    SDL_Rect get_col_rect() { return col_rect; }

    // edges and floors
    bool floor;
    bool wall;
    bool ceiling;

    // tile color
    bool my_color;

    // tile status
    int status;
    int frame;

private:
    SDL_Rect col_rect;
    int type;
    bool animating;
};

// close the tilemap
void close_tiles(Tile* tiles[]);

#endif /* tiles_hpp */
