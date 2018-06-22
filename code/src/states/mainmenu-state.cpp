
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// include headers
#include <states/mainmenu-state.hpp>
#include <engine.hpp>
#include <menu.hpp>
#include <states/levelstate.hpp>
#include <states/quitmenu-state.hpp>
#include <states/levelstate.hpp>

void MainMenuState::init(Engine* game)
{
    // create title
    title = new class Title(game->screen_width / 2,  game->screen_height / 4, 1600, 200);
    if (!title->tex.load_object(128, 16, "menu/title.png", game->rend))
    {
        printf("failed to load title texture!\n");
        return;
    }

    fade_in = new class FadeIn(game, 100);

    // create buttons
    items.push_back(new class NewGameButton(game->screen_width / 2, game->screen_height / 2 - 40, 480, 80, game->rend));
    items.push_back(new class ContinueButton(game->screen_width / 2, game->screen_height / 2 + 50, 480, 80, game->rend));
    items.push_back(new class OptionsButton(game->screen_width / 2, game->screen_height / 2 + 140, 320, 80, game->rend));
    items.push_back(new class QuitButton(game->screen_width / 2, game->screen_height / 2 + 230, 160, 80, game->rend));

    // start bg music
    Mix_PlayMusic(game->sound->menu_music, -1);
}

void MainMenuState::cleanup()
{

}
