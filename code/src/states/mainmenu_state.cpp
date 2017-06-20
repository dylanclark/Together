
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// include headers
#include <states/mainmenu_state.hpp>
#include <engine.hpp>
#include <menu.hpp>
#include <states/levelstate.hpp>
#include <states/quitmenu_state.hpp>
#include <states/levelstate.hpp>

void MainMenuState::init(Engine* game)
{
    if (!fade_in_tex.load_object(16, 16, "white/background/white_back.png", game->rend))
    {
        printf("Failed to load fade-in object!\n");
        return;
    }
    else
    {
        fade_in_tex.set_blend_mode(SDL_BLENDMODE_BLEND);
    }

    if (!title.load_object(128, 16, "menu/title.png", game->rend))
    {
        printf("Failed to load title texture!\n");
        return;
    }

    if (!new_game_button.load_object(96, 16, "menu/new_game.png", game->rend))
    {
        printf("Failed to load new game button texture!\n");
        return;
    }
    if (!continue_button.load_object(96, 16, "menu/continue.png", game->rend))
    {
        printf("Failed to load continue button texture!\n");
        return;
    }
    if (!OptionsButton.load_object(64, 16, "menu/options.png", game->rend))
    {
        printf("Failed to load options button texture!\n");
        return;
    }
    if (!quit_button.load_object(32, 16, "menu/quit.png", game->rend))
    {
        printf("Failed to load quit button texture!\n");
        return;
    }


    main_menu = new class Menu(game->screen_width, game->screen_height, 100);
    main_menu->size = 4;

    main_menu->menu_title = new class Title (game->screen_width / 2,  game->screen_height / 4, 1600, 200);
    main_menu->menu_title->tex = &title;

    main_menu->buttons.push_back(new class NewGameButton(true, game->screen_width / 2, game->screen_height / 2 - 40, 480, 80));
    main_menu->buttons[0]->tex = &new_game_button;

    main_menu->buttons.push_back(new class ContinueButton(true, game->screen_width / 2, game->screen_height / 2 + 50, 480, 80));
    main_menu->buttons[1]->tex = &continue_button;

    main_menu->buttons.push_back(new class OptionsButton(true, game->screen_width / 2, game->screen_height / 2 + 140, 320, 80));
    main_menu->buttons[2]->tex = &OptionsButton;

    main_menu->buttons.push_back(new class QuitButton(true, game->screen_width / 2, game->screen_height / 2 + 230, 160, 80));
    main_menu->buttons[3]->tex = &quit_button;

    main_menu->fade_in->tex = &fade_in_tex;

    // start bg music
    Mix_PlayMusic(game->sound->menu_music, -1);

}

void MainMenuState::cleanup()
{
    free(main_menu);

    delete main_menu->fade_in;
    main_menu->fade_in = NULL;

    new_game_button.free();
    continue_button.free();
    OptionsButton.free();
    quit_button.free();
}

void MainMenuState::pause()
{
    printf("paused menu\n");
    main_menu->fade_in->alpha = 0;
}

void MainMenuState::resume()
{
    printf("paused menu\n");
}

void MainMenuState::handle_events(Engine* game)
{

    // event handler
    SDL_Event event;

    // handle those events, bruh
    while (SDL_PollEvent(&event))
    {
        if (main_menu->fade_in->timer > 0)
            continue;

        switch (event.type)
        {
            case SDL_QUIT:
                game->quit();
                break;
        }

        if (!main_menu->handle_event(event, game))
        {
            game->quit();
        }
    }
}

void MainMenuState::update(Engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    main_menu->fade_in->update();
    main_menu->update(game);


    for (int i = 0; i < main_menu->buttons.size(); i++)
    {
        if (i == main_menu->selector)
        {
            main_menu->buttons[i]->selected = true;
        }
        else
        {
            main_menu->buttons[i]->selected = false;
        }
    }
}

void MainMenuState::draw(Engine* game)
{
    for (int i = 0; i < main_menu->buttons.size(); i++)
    {
        main_menu->buttons[i]->render(game->rend);
    }

    main_menu->menu_title->render(game->rend);
    main_menu->fade_in->render(game->rend);

    SDL_RenderPresent(game->rend);
}