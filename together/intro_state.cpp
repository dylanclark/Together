
// using stdio and sdl
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>

// include headers
#include "intro_state.hpp"

void intro_state::update(engine* game)
{
    if (Mix_Playing(-1) == 0)
    {
        Mix_PlayChannel(-1, game->sound->menu_choose_snd, 0);
        game->pop_state();
    }
    
    SDL_RenderClear(game->rend);
}

void intro_state::handle_events(engine *game)
{
    // event handler
    SDL_Event event;
    
    // handle those events, bruh
    while (SDL_PollEvent(&event)) { };
    
}