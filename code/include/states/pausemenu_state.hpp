
#ifndef pausemenu_state_hpp
#define pausemenu_state_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <menu.hpp>
#include <engine.hpp>
#include <textures.hpp>

class PauseMenuState : public Gamestate
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

    PauseMenuState() { };

    Menu* pause_menu = NULL;

    Texture ResumeButton;
    Texture LevelSelectButton;
    Texture OptionsButton;
    Texture SaveAndQuitButton;
};

#endif /* pausemenu_state_hpp */
