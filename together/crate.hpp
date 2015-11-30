
#ifndef crate_hpp
#define crate_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include headers
#include "textures.hpp"
#include "tiles.hpp"
#include "levelstate.hpp"
#include "collisions.hpp"

// crate types
const int FOUR_BY_TWO = 0;
const int FOUR_BY_ONE = 1;
const int TWO_BY_TWO = 2;
const int THREE_BY_TWO = 3;

// max number of tiles that this crate will need to render
const int MAX_BORDER = 12;

class crate
{
public:
    // velocity
    float x_vel;
    
    // for collision detection
    SDL_Rect get_col_rect() { return col_rect; };
    
    // check for wall collisions
    bool check_col(SDL_Rect crate, levelstate* level, vector* repos);
    
    // update function (aligns crate with tile-grid)
    void update();
    
    // render function
    void render(int b_status, SDL_Rect* camera, SDL_Renderer* rend, levelstate* level);
    
    // crate texture
    texture tex;
    
    // type
    int crate_type;
    bool black;
    
    // pushed flag
    bool pushed;
    
    // initializer
    crate(int x, int y, int type);
    
    // tileset
    tile* tileset[MAX_BORDER];
    void create_tiles(int b_status, levelstate* level);
    bool generating;
    
    // determine which tiletype to render
    int tile_type_top(int type);
    int tile_type_bottom(int type);
    int tile_type_left(int type);
    int tile_type_right(int type);
    
    // collision rectangle
    SDL_Rect col_rect;
};

#endif /* crate_hpp */
