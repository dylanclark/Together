
#ifndef levelselectmenu_state_hpp
#define levelselectmenu_state_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <menu.hpp>
#include <engine.hpp>
#include <textures.hpp>

class LevelSelectMenuState : public Gamestate
{
public:
    // init and shutdown
    void init(Engine* game);
    void cleanup();

    // handling control of screen
    void handle_events(Engine* game);
    void update(Engine* game);
    void draw(Engine* game);

    LevelSelectMenuState() { };

    Menu* levelselect_menu;

    Texture level_slider;
    Texture back_button;
};


#endif /* levelselectmenu_state_hpp */
