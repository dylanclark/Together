
#ifndef newgamemenu_state_hpp
#define newgamemenu_state_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <menus/menu.hpp>
#include <engine/engine.hpp>
#include <engine/gamestate.hpp>
#include <textures/textures.hpp>

class newgamemenu_state : public gamestate
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

    newgamemenu_state() { };

    menu* newgame_menu = NULL;

    texture newgame_title;
    texture yes_button;
    texture no_button;
};

#endif /* newgamemenu_state_hpp */
