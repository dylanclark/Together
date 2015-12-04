
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>

// include headers
#include "quitmenu_state.hpp"
#include "mainmenu_state.hpp"
#include "engine.hpp"
#include "menu.hpp"
#include "initialization.hpp"
#include "level1_state.hpp"
#include "level01_state.hpp"
#include "level2_state.hpp"
#include "menu_buttons.hpp"
#include "pausemenu_state.hpp"
#include "optionsmenu_state.hpp"
#include "levelselectmenu_state.hpp"

void levelselectmenu_state::init(engine* game)
{
    if (!level_slider.load_object(96, 16, "textures/menu/level_slider.png", game->rend))
    {
        printf("Failed to load resume button object!\n");
        return;
    }
    
    if (!back_button.load_object(64, 16, "textures/menu/back.png", game->rend))
    {
        printf("Failed to load save/quit button texture!\n");
        return;
    }
    
    
    levelselect_menu = new class menu(game->screen_width, game->screen_height, 0);
    levelselect_menu->size = 4;
    
    levelselect_menu->sliders.push_back(new class level_slider(true, game->read_save(), false, game->screen_width / 2, game->screen_height / 2 - 45, 480, 80));
    levelselect_menu->sliders[0]->tex = &level_slider;
    
    levelselect_menu->buttons.push_back(new class back_button(true, game->screen_width / 2, game->screen_height / 2 + 45, 320, 80));
    levelselect_menu->buttons[0]->tex = &back_button;
    
    
}

void levelselectmenu_state::cleanup()
{
    free(levelselect_menu);
    
    level_slider.free();
    back_button.free();
}

void levelselectmenu_state::pause()
{
    
}

void levelselectmenu_state::resume()
{
    
}

void levelselectmenu_state::handle_events(engine* game)
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
        
        if (!levelselect_menu->handle_event(event, game))
        {
            Mix_ResumeMusic();
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->pop_state();
        }
    }
}

void levelselectmenu_state::update(engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);
    
    levelselect_menu->update(game);
    
    
    for (int i = 0; i < levelselect_menu->buttons.size() + levelselect_menu->sliders.size(); i++)
    {
        if (i < levelselect_menu->sliders.size())
        {
            if (i == levelselect_menu->selector)
            {
                levelselect_menu->sliders[i]->selected = true;
            }
            else
            {
                levelselect_menu->sliders[i]->selected = false;
            }
        }
        else
        {
            if (i == levelselect_menu->selector)
            {
                levelselect_menu->buttons[i - (levelselect_menu->sliders.size())]->selected = true;
            }
            else
            {
                levelselect_menu->buttons[i - (levelselect_menu->sliders.size())]->selected = false;
            }
        }
        
    }
}

void levelselectmenu_state::draw(engine* game)
{
    for (int i = 0; i < levelselect_menu->sliders.size(); i++)
    {
        levelselect_menu->sliders[i]->render(game->rend);
    }
    
    for (int i = 0; i < levelselect_menu->buttons.size(); i++)
    {
        levelselect_menu->buttons[i]->render(game->rend);
    }
    
    SDL_RenderPresent(game->rend);
}