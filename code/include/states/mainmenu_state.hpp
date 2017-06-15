
#ifndef MainMenuState_hpp
#define MainMenuState_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <menu.hpp>
#include <engine.hpp>
#include <textures.hpp>

class MainMenuState : public Gamestate
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

    MainMenuState() { };

    Menu* main_menu;

    Texture fade_in_tex;
    Texture title;
    Texture new_game_button;
    Texture continue_button;
    Texture OptionsButton;
    Texture quit_button;
};

#endif /* MainMenuState_hpp */
