
#ifndef level_messages_hpp
#define level_messages_hpp

#include <stdio.h>

// include header files
#include "characters.hpp"
#include "initialization.hpp"
#include "textures.hpp"
#include "collisions.hpp"
#include "tiles.hpp"
#include "camera.hpp"

class level_message
{
public:
    
    level_message();
    
    // initialize traits
    void send_message(int type, engine* game);
    
    // render the item on the screen
    void render(SDL_Rect* camera, SDL_Renderer* rend);
    
    // collision rectangle
    SDL_Rect col_rect;
    
    // texture
    texture tex;
    
    int type;
};

#endif /* level_messages_hpp */

