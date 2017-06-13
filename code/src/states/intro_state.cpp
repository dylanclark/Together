
// using stdio and sdl
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// include headers
#include <states/intro_state.hpp>

void intro_state::update(engine* game)
{
    // switch to level if the sound effect is over
    if (Mix_Playing(-1) == 0)
    {
        game->pop_state();
        return;
    }

    // update the levelstate if the intro state is still active
    if (game->states.size() > 1)
    {
        game->states.rbegin()[1]->update(game);
    }

    SDL_RenderClear(game->rend);
}

void intro_state::handle_events(engine *game)
{
    // event handler
    SDL_Event event;

    // handle those events, bruh
    while (SDL_PollEvent(&event)) { };
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD:code/src/states/intro_state.cpp

}
=======
    
}
>>>>>>> 04da4d0... added a dependency file to help for Makefile conversion:together/intro_state.cpp
=======

}
>>>>>>> d9ca289... readded conflicting files
=======

}
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
