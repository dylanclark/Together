
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// include headers
#include <states/quitmenu-state.hpp>
#include <states/mainmenu-state.hpp>
#include <engine.hpp>
#include <menu.hpp>
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/pausemenu-state.hpp>

void PauseMenuState::init(Engine* game)
{
    items.push_back(new class ResumeButton(game->screen_width / 2, game->screen_height / 2 - 135, 320, 80, game->rend));
    items.push_back(new class LevelSelectButton(game->screen_width / 2, game->screen_height / 2 - 45, 640, 80, game->rend));
    items.push_back(new class OptionsButton(game->screen_width / 2, game->screen_height / 2 + 45, 320, 80, game->rend));
    items.push_back(new class SaveAndQuitButton(game->screen_width / 2, game->screen_height / 2 + 135, 640, 80, game->rend));

}

void PauseMenuState::cleanup()
{

}
