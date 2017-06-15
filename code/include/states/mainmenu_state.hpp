
#ifndef mainmenu_state_hpp
#define mainmenu_state_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <menus/menu.hpp>
#include <engine.hpp>
#include <engine.hpp>
#include <textures/textures.hpp>

class mainmenu_state : public gamestate
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

    mainmenu_state() { };

    menu* main_menu = NULL;

    texture fade_in_tex;
    texture title;
    texture new_game_button;
    texture continue_button;
    texture options_button;
    texture quit_button;
};

#endif /* mainmenu_state_hpp */
