
#ifndef level_messages_hpp
#define level_messages_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
#include <engine.hpp>
#include <textures/textures.hpp>

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

