
#ifndef level2_state_hpp
#define level2_state_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include headers
#include "gamestate.hpp"
#include "characters.hpp"
#include "tiles.hpp"
#include "levelstate.hpp"

class level2_state : public gamestate, public levelstate
{
public:
    // init and shutdown
    void init(engine* game);
    void cleanup();
    
    // pause and resume
    void pause();
    void resume();
    
    // handling control of screen
    void handle_events(engine* game);
    void update(engine* game);
    void draw(engine* game);
    
    static level2_state* instance()
    {
        return &level2_state_inst;
    }
    
    level2_state() { };
    
private:
    static level2_state level2_state_inst;
};

#endif /* level2_state_hpp */
