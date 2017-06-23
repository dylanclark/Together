
#ifndef OptionsMenuState_hpp
#define OptionsMenuState_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <menu.hpp>
#include <engine.hpp>
#include <textures.hpp>

class OptionsMenuState : public Gamestate
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

    OptionsMenuState() { };

    Menu* options_menu;

    Texture volume_slider;
    Texture sfx_slider;
    Texture controls_button;
    Texture back_button;
};

#endif /* OptionsMenuState_hpp */
