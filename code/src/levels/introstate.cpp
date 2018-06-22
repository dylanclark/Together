
// using stdio and sdl
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// include headers
#include <states/intro-state.hpp>

void IntroState::update(Engine* game)
{
    // switch to level if the sound effect is over
    if (Mix_Playing(-1) == 0) {
        game->pop_state();
        return;
    }

    // update the levelstate if the intro state is still active
    if (game->states.size() > 1) {
        game->states.rbegin()[1]->update(game);
    }

    SDL_RenderClear(game->rend);
}

void IntroState::handle_events(Engine* game)
{
    // event handler
    SDL_Event event;

    // handle those events, bruh
    while (SDL_PollEvent(&event)) { };
}