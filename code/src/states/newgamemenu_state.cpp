
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// include headers
#include <states/quitmenu_state.hpp>
#include <states/mainmenu_state.hpp>
#include <engine.hpp>
#include <menu.hpp>
#include <states/level1_state.hpp>
#include <states/level2_state.hpp>
#include <states/newgamemenu_state.hpp>

void newgamemenu_state::init(engine* game)
{
    if (!newgame_title.load_object(256, 32, "resources/textures/menu/new_game_title.png", game->rend))
    {
        printf("Failed to load quit title object!\n");
        return;
    }

    if (!yes_button.load_object(32, 16, "resources/textures/menu/yes.png", game->rend))
    {
        printf("Failed to load yes button texture!\n");
        return;
    }

    if (!no_button.load_object(32, 16, "resources/textures/menu/no.png", game->rend))
    {
        printf("Failed to load no button texture!\n");
        return;
    }


    newgame_menu = new class menu(game->screen_width, game->screen_height, 0);
    newgame_menu->size = 2;

    newgame_menu->menu_title = new class title (game->screen_width / 2,  game->screen_height / 3, 1280, 160);
    newgame_menu->menu_title->tex = &newgame_title;

    newgame_menu->buttons.push_back(new class yes_newgame_button(true, game->screen_width / 2, game->screen_height / 2, 160, 80));
    newgame_menu->buttons[0]->tex = &yes_button;

    newgame_menu->buttons.push_back(new class no_button(true, game->screen_width / 2, game->screen_height / 2 + 90, 160, 80));
    newgame_menu->buttons[1]->tex = &no_button;
}

void newgamemenu_state::cleanup()
{
    free(newgame_menu);

    newgame_title.free();
    yes_button.free();
    no_button.free();
}

void newgamemenu_state::pause()
{

}

void newgamemenu_state::resume()
{

}

void newgamemenu_state::handle_events(engine* game)
{
    // event handler
    SDL_Event event;

    // handle those events, bruh
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                game->quit();
                break;
        }

        if (!newgame_menu->handle_event(event, game))
        {
            game->quit();
        }
    }
}

void newgamemenu_state::update(engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    newgame_menu->update(game);


    for (int i = 0; i < newgame_menu->buttons.size(); i++)
    {
        if (i == newgame_menu->selector)
        {
            newgame_menu->buttons[i]->selected = true;
        }
        else
        {
            newgame_menu->buttons[i]->selected = false;
        }
    }
}

void newgamemenu_state::draw(engine* game)
{
    for (int i = 0; i < newgame_menu->buttons.size(); i++)
    {
        newgame_menu->buttons[i]->render(game->rend);
    }

    newgame_menu->menu_title->render(game->rend);

    SDL_RenderPresent(game->rend);
}