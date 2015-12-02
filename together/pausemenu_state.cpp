
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
#include "level2_state.hpp"
#include "menu_buttons.hpp"
#include "pausemenu_state.hpp"

void pausemenu_state::init(engine* game)
{
    if (!resume_button.load_object(64, 16, "textures/menu/resume.png", game->rend))
    {
        printf("Failed to load resume button object!\n");
        return;
    }
    
    if (!level_select_button.load_object(128, 16, "textures/menu/level_select.png", game->rend))
    {
        printf("Failed to load level select button texture!\n");
        return;
    }
    
    if (!options_button.load_object(64, 16, "textures/menu/options.png", game->rend))
    {
        printf("Failed to load options button texture!\n");
        return;
    }
    
    if (!save_and_quit_button.load_object(128, 16, "textures/menu/save_and_quit.png", game->rend))
    {
        printf("Failed to load save/quit button texture!\n");
        return;
    }
    
    
    pause_menu = new class menu(game->screen_width, game->screen_height, 0);
    pause_menu->size = 4;
    
    pause_menu->buttons.push_back(new class resume_button(true, game->screen_width / 2, game->screen_height / 2 - 120, 300, 60));
    pause_menu->buttons[0]->tex = &resume_button;
    
    pause_menu->buttons.push_back(new class level_select_button(true, game->screen_width / 2, game->screen_height / 2 - 40, 600, 60));
    pause_menu->buttons[1]->tex = &level_select_button;
    
    pause_menu->buttons.push_back(new class options_button(true, game->screen_width / 2, game->screen_height / 2 + 40, 300, 60));
    pause_menu->buttons[2]->tex = &options_button;
    
    pause_menu->buttons.push_back(new class save_and_quit_button(true, game->screen_width / 2, game->screen_height / 2 + 120, 600, 60));
    pause_menu->buttons[3]->tex = &save_and_quit_button;
    
    
}

void pausemenu_state::cleanup()
{
    free(pause_menu);
    
    resume_button.free();
    level_select_button.free();
    options_button.free();
    save_and_quit_button.free();
}

void pausemenu_state::pause()
{
    
}

void pausemenu_state::resume()
{
    
}

void pausemenu_state::handle_events(engine* game)
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

void pausemenu_state::update(engine* game)
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

void pausemenu_state::draw(engine* game)
{
    for (int i = 0; i < pause_menu->buttons.size(); i++)
    {
        pause_menu->buttons[i]->render(game->rend);
    }
    
    SDL_RenderPresent(game->rend);
}