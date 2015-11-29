
#ifndef button_hpp
#define button_hpp

#include <stdio.h>

// include header files
#include "characters.hpp"
#include "initialization.hpp"
#include "textures.hpp"
#include "collisions.hpp"
#include "tiles.hpp"
#include "camera.hpp"


// button status constants
const int BUTT_ACTIVE = 0;
const int BUTT_INACTIVATE = 1;
const int BUTT_INACTIVE = 2;
const int BUTT_ACTIVATE = 3;

// animation length
const int BUTT_ANIMATION_LENGTH = 8;


class button
{
public:
    // initialize traits
    button();
    
    // render the item on the screen
    void render(SDL_Rect* camera, SDL_Renderer* rend);
    
    // collision rectangle
    SDL_Rect col_rect;
    
    // texture
    texture tex;
    
    // check for collision with dot
    bool check(SDL_Rect dot_rect);
    
    // activated or not
    bool activated;
    
    // single use
    bool single;
    
    // color
    bool black;
    
    
};

#endif /* button_hpp */

