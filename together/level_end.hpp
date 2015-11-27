
#ifndef level_end_hpp
#define level_end_hpp

#include <stdio.h>

// include header files
#include "characters.hpp"
#include "initialization.hpp"
#include "textures.hpp"
#include "collisions.hpp"
#include "tiles.hpp"
#include "camera.hpp"

class level_end
{
public:
    // initialize traits
    level_end();
    
    // render the item on the screen
    void render(SDL_Rect* camera, SDL_Renderer* rend);
    
    // collision rectangle
    SDL_Rect col_rect;
    
    // texture
    texture tex;
    
    // check for collision with dot
    bool check(SDL_Rect dot_rect);
    
};

#endif /* level_end_hpp */

