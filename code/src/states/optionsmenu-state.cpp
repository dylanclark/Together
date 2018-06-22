
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
#include <states/optionsmenu-state.hpp>

void OptionsMenuState::init(Engine* game)
{
    items.push_back(new class VolumeSlider(5, true, game->screen_width / 2, game->screen_height / 2 - 135, 640, 80, game->rend));

    items.push_back(new class SfxSlider(5, true, game->screen_width / 2, game->screen_height / 2 - 45, 640, 80, game->rend));

    items.push_back(new class ControlsButton(game->screen_width / 2, game->screen_height / 2 + 45, 480, 80, game->rend));

    items.push_back(new class BackButton(game->screen_width / 2, game->screen_height / 2 + 135, 320, 80, game->rend));
}

void OptionsMenuState::cleanup()
{

}
