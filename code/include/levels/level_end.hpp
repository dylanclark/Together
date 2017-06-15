
#ifndef level_end_hpp
#define level_end_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
#include <textures/textures.hpp>
#include <engine.hpp>

class level_end
{
public:
    // initialize traits
    level_end();

    // render the item on the screen
    void render(SDL_Rect* camera, engine* game);

    // collision rectangle
    SDL_Rect col_rect;

    // texture
    texture tex;

    // check for collision with dot
    bool check(SDL_Rect dot_rect);
};

#endif /* level_end_hpp */

