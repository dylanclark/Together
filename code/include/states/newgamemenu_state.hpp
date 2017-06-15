
#ifndef newgamemenu_state_hpp
#define newgamemenu_state_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <menu.hpp>
#include <engine.hpp>
#include <textures.hpp>

class NewGameMenuState : public Gamestate
{
public:
    // init and shutdown
    void init(Engine* game);
    void cleanup();

    // pause and resume
    void pause();
    void resume();

    // handling control of screen
    void handle_events(Engine* game);
    void update(Engine* game);
    void draw(Engine* game);

    NewGameMenuState() { };

    Menu* newgame_menu = NULL;

    Texture newgame_title;
    Texture yes_button;
    Texture no_button;
};

#endif /* newgamemenu_state_hpp */
