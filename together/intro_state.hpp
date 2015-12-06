
#ifndef intro_state_hpp
#define intro_state_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include headers
#include "menu.hpp"
#include "engine.hpp"
#include "gamestate.hpp"
#include "textures.hpp"

class intro_state : public gamestate
{
public:
    // init and shutdown
    void init(engine* game) { Mix_PlayChannel(-1, game->sound->menu_onset_snd, 0); };
    void cleanup() { };
    
    // pause and resume
    void pause() { };
    void resume() { };
    
    // handling control of screen
    void handle_events(engine* game);
    void update(engine* game);
    void draw(engine* game) { SDL_RenderPresent(game->rend); } ;
};

#endif /* intro_state_hpp */
