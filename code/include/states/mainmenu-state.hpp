
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

class MainMenuState : public Menu
{
public:
    MainMenuState() { };

    // init and shutdown
    void init(Engine* game);
    void cleanup();
};

#endif /* MainMenuState_hpp */
