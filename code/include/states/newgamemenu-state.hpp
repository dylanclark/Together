
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

class NewGameMenuState : public Menu
{
public:
    NewGameMenuState() { };

    // init and shutdown
    void init(Engine* game);
    void cleanup();
};

#endif /* newgamemenu_state_hpp */
