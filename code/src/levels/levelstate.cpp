
#include <stdio.h>
#include <SDL2/SDL_mixer.h>

#include <levels.hpp>
#include <engine.hpp>
#include <states/pausemenu-state.hpp>

void
Levelstate::pause(Engine* game)
{
    Mix_PauseMusic();
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->push_state(new PauseMenuState);
}