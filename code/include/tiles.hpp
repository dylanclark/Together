
#ifndef tiles_hpp
#define tiles_hpp

#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// tile dimensions!
static const int TILE_WIDTH = 60;
static const int TILE_WIDTH_TEX = 16;

typedef enum _tiletype {
    B_BACK = 0,
    B_FLOOR1 = 1,
    B_FLOOR2 = 2,
    B_FLOOR3 = 3,
    B_FLOOREDGE_R = 4,
    B_FLOOREDGE_L = 5,
    B_CEILING = 6,
    B_CEILINGEDGE_R = 7,
    B_CEILINGEDGE_L = 8,
    B_WALL_R = 9,
    B_WALL_L = 10,
    B_CORNER_BL = 11,
    B_CORNER_BR = 12,
    B_CORNER_TL = 13,
    B_CORNER_TR = 14,
    W_BACK = 15,
    W_FLOOR1 = 16,
    W_FLOOR2 = 17,
    W_FLOOR3 = 18,
    W_FLOOREDGE_R = 19,
    W_FLOOREDGE_L = 20,
    W_CEILING = 21,
    W_CEILINGEDGE_R = 22,
    W_CEILINGEDGE_L = 23,
    W_WALL_R = 24,
    W_WALL_L = 25,
    W_CORNER_BL = 26,
    W_CORNER_BR = 27,
    W_CORNER_TL = 28,
    W_CORNER_TR = 29
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
    int get_type();
    SDL_Rect get_col_rect();

    // edges and floors
    bool floor_b, floor_w;
    bool wall_b, wall_w;
    bool ceiling_b, ceiling_w;

    // tile status
    int status;
    int frame;

private:
    SDL_Rect col_rect;
    int type;
    bool animating;
};

class Levelstate;

// draw the tilemap
bool set_tiles(Levelstate* lvl, Tile* tiles[], std::string filename);

// close the tilemap
void close_tiles(Tile* tiles[]);

#endif /* tiles_hpp */