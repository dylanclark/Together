
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
#include <states/levelselectmenu-state.hpp>

void LevelSelectMenuState::init(Engine* game)
{
    items.push_back(new class LevelSlider(game->read_save(), false, game->screen_width / 2, game->screen_height / 2 - 45, 480, 80, game->rend));

    items.push_back(new class BackButton(game->screen_width / 2, game->screen_height / 2 + 45, 320, 80, game->rend));


}

void LevelSelectMenuState::cleanup()
{

}

