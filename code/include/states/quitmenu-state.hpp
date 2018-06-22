
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

class QuitMenuState : public Menu
{
public:
    QuitMenuState() { };

    // init and shutdown
    void init(Engine* game);
    void cleanup();
};

#endif /* MainMenuState_hpp */
