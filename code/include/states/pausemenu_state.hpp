
#ifndef pausemenu_state_hpp
#define pausemenu_state_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <menus/menu.hpp>
#include <engine.hpp>
#include <engine.hpp>
#include <textures/textures.hpp>

class pausemenu_state : public gamestate
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

    pausemenu_state() { };

    menu* pause_menu = NULL;

    texture resume_button;
    texture level_select_button;
    texture options_button;
    texture save_and_quit_button;
};

#endif /* pausemenu_state_hpp */
