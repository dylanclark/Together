
#ifndef level1_state_hpp
#define level1_state_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

// include headers
#include "gamestate.hpp"
#include "characters.hpp"
#include "tiles.hpp"
#include "levelstate.hpp"

class level1_state : public gamestate, public levelstate
{
public:
    // init and shutdown
    void init(engine* game);
    void cleanup();
    
    // pause and resume
    void pause();
    void resume();
    
    // handling control of screen
    void handle_events(engine* games);
    void update(engine* game);
    void draw(engine* game);
    
    level1_state() { };
    
    // load textures
    void load_textures(engine* game);
    
    // init objects
    void init_objects(engine* game);
    
    // object interactions
    void interactions(engine* game);
};

#endif /* level1_state_hpp */
