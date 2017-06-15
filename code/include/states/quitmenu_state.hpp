
#ifndef quitmenu_state_hpp
#define quitmenu_state_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <menu.hpp>
#include <engine.hpp>
#include <textures.hpp>

class QuitMenuState : public Gamestate
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

    QuitMenuState() { };

    Menu* quit_menu = NULL;

    Texture quit_title;
    Texture yes_button;
    Texture no_button;
};

#endif /* MainMenuState_hpp */
