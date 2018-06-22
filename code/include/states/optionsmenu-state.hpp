
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

class OptionsMenuState : public Menu
{
public:
    OptionsMenuState() { };

    // init and shutdown
    void init(Engine* game);
    void cleanup();
};

#endif /* OptionsMenuState_hpp */
