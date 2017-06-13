
#ifndef level_end_hpp
#define level_end_hpp

#include <stdio.h>

// include header files
#include <characters/characters.hpp>
#include <engine/init.hpp>
#include <textures/textures.hpp>
#include <characters/collisions.hpp>
#include <levels/tiles.hpp>
#include <levels/camera.hpp>
#include <engine/engine.hpp>

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

