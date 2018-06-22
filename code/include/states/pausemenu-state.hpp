
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

class PauseMenuState : public Menu
{
public:
    PauseMenuState() { };

    // init and shutdown
    void init(Engine* game);
    void cleanup();
};

#endif /* pausemenu_state_hpp */
