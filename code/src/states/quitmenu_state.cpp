
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
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>

void QuitMenuState::init(Engine* game)
{
    if (!quit_title.load_object(128, 16, "resources/textures/menu/quit_title.png", game->rend))
    {
        printf("Failed to load quit title object!\n");
        return;
    }

    if (!yes_button.load_object(32, 16, "resources/textures/menu/yes_quit.png", game->rend))
    {
        printf("Failed to load yes button texture!\n");
        return;
    }

    if (!no_button.load_object(32, 16, "resources/textures/menu/no.png", game->rend))
    {
        printf("Failed to load no button texture!\n");
        return;
    }


    quit_menu = new class Menu(game->screen_width, game->screen_height, 0);
    quit_menu->size = 2;

    quit_menu->menu_title = new class Title (game->screen_width / 2,  game->screen_height / 3, 640, 80);
    quit_menu->menu_title->tex = &quit_title;

    quit_menu->buttons.push_back(new class YesQuitButton(true, game->screen_width / 2, game->screen_height / 2, 160, 80));
    quit_menu->buttons[0]->tex = &yes_button;

    quit_menu->buttons.push_back(new class NoButton(true, game->screen_width / 2, game->screen_height / 2 + 90, 160, 80));
    quit_menu->buttons[1]->tex = &no_button;
}

void QuitMenuState::cleanup()
{
    free(quit_menu);

    quit_title.free();
    yes_button.free();
    no_button.free();
}

void QuitMenuState::pause()
{

}

void QuitMenuState::resume()
{

}

void QuitMenuState::handle_events(Engine* game)
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

        if (!quit_menu->handle_event(event, game))
        {
            game->quit();
        }
    }
}

void QuitMenuState::update(Engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    quit_menu->update(game);


    for (int i = 0; i < quit_menu->buttons.size(); i++)
    {
        if (i == quit_menu->selector)
        {
            quit_menu->buttons[i]->selected = true;
        }
        else
        {
            quit_menu->buttons[i]->selected = false;
        }
    }
}

void QuitMenuState::draw(Engine* game)
{
    for (int i = 0; i < quit_menu->buttons.size(); i++)
    {
        quit_menu->buttons[i]->render(game->rend);
    }

    quit_menu->menu_title->render(game->rend);

    SDL_RenderPresent(game->rend);
}