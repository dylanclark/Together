
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

void QuitMenuState::init(Engine* game)
{
    title = new class Title (game->screen_width / 2,  game->screen_height / 3, 640, 80);
    if (!title->tex.load_object(128, 16, "menu/quit_title.png", game->rend)) {
        printf("Failed to load quit title object!\n");
    }

    items.push_back(new class YesQuitButton(game->screen_width / 2, game->screen_height / 2, 160, 80, game->rend));
    items.push_back(new class NoButton(game->screen_width / 2, game->screen_height / 2 + 90, 160, 80, game->rend));
}

void QuitMenuState::cleanup()
{

}
