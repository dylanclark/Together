
#ifndef Level1State_hpp
#define Level1State_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// include headers
#include <engine.hpp>
#include <char.hpp>
#include <levels.hpp>

class Level1State : public Levelstate
{
public:

private:
    // init and shutdown
    void init(Engine* game);
    void cleanup();

    // handling control of screen
    void handle_events(Engine* games);
    void update(Engine* game);
    void draw(Engine* game);

    // load textures
    void load_textures(Engine* game);

    // init objects
    void init_objects(Engine* game);

    // object interactions
    void interactions(Engine* game);
};

#endif /* Level1State_hpp */
