
#ifndef level_messages_hpp
#define level_messages_hpp

#include <stdio.h>

// include header files
#include <characters/characters.hpp>
#include <engine/init.hpp>
#include <textures/textures.hpp>
#include <characters/collisions.hpp>
#include <levels/tiles.hpp>
#include <levels/camera.hpp>

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

