
#ifndef tile_def_hpp
#define tile_def_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include headers
#include "textures.hpp"
#include "engine.hpp"

// tile dimensions!
const int TILE_WIDTH = 60;
const int TILE_WIDTH_TEX = 16;

// initialize all tiles
const int B_BACK = 0;
const int B_FLOOR1 = 1;
const int B_FLOOR2 = 2;
const int B_FLOOR3 = 3;
const int B_FLOOREDGE_R = 4;
const int B_FLOOREDGE_L = 5;
const int B_CEILING = 6;
const int B_CEILINGEDGE_R = 7;
const int B_CEILINGEDGE_L = 8;
const int B_WALL_R = 9;
const int B_WALL_L = 10;
const int B_CORNER_BL = 11;
const int B_CORNER_BR = 12;
const int B_CORNER_TL = 13;
const int B_CORNER_TR = 14;
const int W_BACK = 15;
const int W_FLOOR1 = 16;
const int W_FLOOR2 = 17;
const int W_FLOOR3 = 18;
const int W_FLOOREDGE_R = 19;
const int W_FLOOREDGE_L = 20;
const int W_CEILING = 21;
const int W_CEILINGEDGE_R = 22;
const int W_CEILINGEDGE_L = 23;
const int W_WALL_R = 24;
const int W_WALL_L = 25;
const int W_CORNER_BL = 26;
const int W_CORNER_BR = 27;
const int W_CORNER_TL = 28;
const int W_CORNER_TR = 29;

// tile status constants
const int TILE_ACTIVE = 0;
const int TILE_INACTIVATE = 1;
const int TILE_INACTIVE = 2;
const int TILE_ACTIVATE = 3;

// total number of tiles
const int TOTAL_TILES = 30;

// animation length
const int TILE_FRAMES = 10;

// class definition
class tile
{
public:
    // initialize the tile
    tile(int x, int y, int tile_type);
    
    // render the tile on the screen
    void render(int b_status, SDL_Rect* camera, engine* game, texture* tile_tex);
    
    // get the tile type
    int get_type();
    
    // get the collision rectangle
    SDL_Rect get_col_rect();
    
    // edges and floors
    bool floor_b, floor_w;
    bool wall_b, wall_w;
    bool ceiling_b, ceiling_w;
    
    // tile status
    int status;
    int frame;
    
private:
    // collision rectangle
    SDL_Rect col_rect;
    
    // tile type
    int type;
    
    // animation bool
    bool animating;
};

#endif /* tile_def_hpp */
