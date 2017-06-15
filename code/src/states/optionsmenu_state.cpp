
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
#include <states/level3_state.hpp>
#include <states/level2_state.hpp>
#include <states/pausemenu_state.hpp>
#include <states/optionsmenu_state.hpp>

void optionsmenu_state::init(engine* game)
{
    if (!volume_slider.load_object(128, 16, "resources/textures/menu/volume.png", game->rend))
    {
        printf("Failed to load resume button object!\n");
        return;
    }

    if (!sfx_slider.load_object(128, 16, "resources/textures/menu/sfx.png", game->rend))
    {
        printf("Failed to load level select button texture!\n");
        return;
    }

    if (!controls_button.load_object(96, 16, "resources/textures/menu/controls.png", game->rend))
    {
        printf("Failed to load options button texture!\n");
        return;
    }

    if (!back_button.load_object(64, 16, "resources/textures/menu/back.png", game->rend))
    {
        printf("Failed to load save/quit button texture!\n");
        return;
    }


    options_menu = new class menu(game->screen_width, game->screen_height, 0);
    options_menu->size = 4;

    options_menu->sliders.push_back(new class volume_slider(true, 5, true, game->screen_width / 2, game->screen_height / 2 - 135, 640, 80));
    options_menu->sliders[0]->tex = &volume_slider;

    options_menu->sliders.push_back(new class sfx_slider(true, 5, true, game->screen_width / 2, game->screen_height / 2 - 45, 640, 80));
    options_menu->sliders[1]->tex = &sfx_slider;

    options_menu->buttons.push_back(new class controls_button(true, game->screen_width / 2, game->screen_height / 2 + 45, 480, 80));
    options_menu->buttons[0]->tex = &controls_button;

    options_menu->buttons.push_back(new class back_button(true, game->screen_width / 2, game->screen_height / 2 + 135, 320, 80));
    options_menu->buttons[1]->tex = &back_button;


}

void optionsmenu_state::cleanup()
{
    free(options_menu);

    volume_slider.free();
    sfx_slider.free();
    controls_button.free();
    back_button.free();
}

void optionsmenu_state::pause()
{

}

void optionsmenu_state::resume()
{

}

void optionsmenu_state::handle_events(engine* game)
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

        if (!options_menu->handle_event(event, game))
        {
            Mix_ResumeMusic();
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->pop_state();
        }
    }
}

void optionsmenu_state::update(engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    options_menu->update(game);


    for (int i = 0; i < options_menu->buttons.size() + options_menu->sliders.size(); i++)
    {
        if (i < options_menu->sliders.size())
        {
            if (i == options_menu->selector)
            {
                options_menu->sliders[i]->selected = true;
            }
            else
            {
                options_menu->sliders[i]->selected = false;
            }
        }
        else
        {
            if (i == options_menu->selector)
            {
                options_menu->buttons[i - (options_menu->sliders.size())]->selected = true;
            }
            else
            {
                options_menu->buttons[i - (options_menu->sliders.size())]->selected = false;
            }
        }

    }
}

void optionsmenu_state::draw(engine* game)
{
    for (int i = 0; i < options_menu->sliders.size(); i++)
    {
        options_menu->sliders[i]->render(game->rend);
    }

    for (int i = 0; i < options_menu->buttons.size(); i++)
    {
        options_menu->buttons[i]->render(game->rend);
    }

    SDL_RenderPresent(game->rend);
}