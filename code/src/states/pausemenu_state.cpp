
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
#include <states/level3_state.hpp>
#include <states/level1_state.hpp>
#include <states/level2_state.hpp>
#include <states/pausemenu_state.hpp>

void PauseMenuState::init(Engine* game)
{
    if (!ResumeButton.load_object(64, 16, "resources/textures/menu/resume.png", game->rend))
    {
        printf("Failed to load resume button object!\n");
        return;
    }

    if (!LevelSelectButton.load_object(128, 16, "resources/textures/menu/level_select.png", game->rend))
    {
        printf("Failed to load level select button texture!\n");
        return;
    }

    if (!OptionsButton.load_object(64, 16, "resources/textures/menu/options.png", game->rend))
    {
        printf("Failed to load options button texture!\n");
        return;
    }

    if (!SaveAndQuitButton.load_object(128, 16, "resources/textures/menu/save_and_quit.png", game->rend))
    {
        printf("Failed to load save/quit button texture!\n");
        return;
    }


    pause_menu = new class Menu(game->screen_width, game->screen_height, 0);
    pause_menu->size = 4;

    pause_menu->buttons.push_back(new class ResumeButton(true, game->screen_width / 2, game->screen_height / 2 - 135, 320, 80));
    pause_menu->buttons[0]->tex = &ResumeButton;

    pause_menu->buttons.push_back(new class LevelSelectButton(true, game->screen_width / 2, game->screen_height / 2 - 45, 640, 80));
    pause_menu->buttons[1]->tex = &LevelSelectButton;

    pause_menu->buttons.push_back(new class OptionsButton(true, game->screen_width / 2, game->screen_height / 2 + 45, 320, 80));
    pause_menu->buttons[2]->tex = &OptionsButton;

    pause_menu->buttons.push_back(new class SaveAndQuitButton(true, game->screen_width / 2, game->screen_height / 2 + 135, 640, 80));
    pause_menu->buttons[3]->tex = &SaveAndQuitButton;

}

void PauseMenuState::cleanup()
{
    free(pause_menu);

    ResumeButton.free();
    LevelSelectButton.free();
    OptionsButton.free();
    SaveAndQuitButton.free();
}

void PauseMenuState::pause()
{

}

void PauseMenuState::resume()
{

}

void PauseMenuState::handle_events(Engine* game)
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

        if (!pause_menu->handle_event(event, game))
        {
            Mix_ResumeMusic();
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->pop_state();
        }
    }
}

void PauseMenuState::update(Engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    pause_menu->update(game);


    for (int i = 0; i < pause_menu->buttons.size(); i++)
    {
        if (i == pause_menu->selector)
        {
            pause_menu->buttons[i]->selected = true;
        }
        else
        {
            pause_menu->buttons[i]->selected = false;
        }
    }
}

void PauseMenuState::draw(Engine* game)
{
    for (int i = 0; i < pause_menu->buttons.size(); i++)
    {
        pause_menu->buttons[i]->render(game->rend);
    }

    SDL_RenderPresent(game->rend);
}