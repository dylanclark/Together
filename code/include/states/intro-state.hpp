
#ifndef IntroState_hpp
#define IntroState_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <menu.hpp>
#include <engine.hpp>
#include <textures.hpp>

class IntroState : public Gamestate
{
public:
    // init and shutdown
    void init(Engine* game) { Mix_PlayChannel(-1, game->sound->menu_onset_snd, 0); };
    void cleanup() { };

    // pause and resume
    void pause() { };
    void resume() { };

    // handling control of screen
    void handle_events(Engine* game);
    void update(Engine* game);
    void draw(Engine* game) { SDL_RenderPresent(game->rend); } ;
};

#endif /* IntroState_hpp */
