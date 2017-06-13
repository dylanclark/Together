
#ifndef gamestate_hpp
#define gamestate_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

class engine;

// include engine header
#include <engine/engine.hpp>

class gamestate
{
public:
    // init and cleanup
    virtual void init(engine* game) = 0;
    virtual void cleanup() = 0;

    // pause and resume
    virtual void pause() = 0;
    virtual void resume() = 0;

    // handling control of screen
    virtual void handle_events(engine* game) = 0;
    virtual void update(engine* game) = 0;
    virtual void draw(engine* game) = 0;

    // changing states
    void change_state(engine* game, gamestate* state)
    {
        game->change_state(state);
    }

    // create class
    gamestate() { };
};

#endif /* gamestate_hpp */
