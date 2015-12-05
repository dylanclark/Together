
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
#include "level3_state.hpp"
#include "menu_buttons.hpp"

void quitmenu_state::init(engine* game)
{
    if (!quit_title.load_object(128, 16, "textures/menu/quit_title.png", game->rend))
    {
        printf("Failed to load quit title object!\n");
        return;
    }
    
    if (!yes_button.load_object(32, 16, "textures/menu/yes_quit.png", game->rend))
    {
        printf("Failed to load yes button texture!\n");
        return;
    }
    
    if (!no_button.load_object(32, 16, "textures/menu/no.png", game->rend))
    {
        printf("Failed to load no button texture!\n");
        return;
    }
    
    
    quit_menu = new class menu(game->screen_width, game->screen_height, 0);
    quit_menu->size = 2;
    
    quit_menu->menu_title = new class title (game->screen_width / 2,  game->screen_height / 3, 640, 80);
    quit_menu->menu_title->tex = &quit_title;
    
    quit_menu->buttons.push_back(new class yes_quit_button(true, game->screen_width / 2, game->screen_height / 2, 160, 80));
    quit_menu->buttons[0]->tex = &yes_button;
    
    quit_menu->buttons.push_back(new class no_button(true, game->screen_width / 2, game->screen_height / 2 + 90, 160, 80));
    quit_menu->buttons[1]->tex = &no_button;
}

void quitmenu_state::cleanup()
{
    free(quit_menu);
    
    quit_title.free();
    yes_button.free();
    no_button.free();
}

void quitmenu_state::pause()
{
    
}

void quitmenu_state::resume()
{
    
}

void quitmenu_state::handle_events(engine* game)
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

void quitmenu_state::update(engine* game)
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

void quitmenu_state::draw(engine* game)
{
    for (int i = 0; i < quit_menu->buttons.size(); i++)
    {
        quit_menu->buttons[i]->render(game->rend);
    }
    
    quit_menu->menu_title->render(game->rend);
    
    SDL_RenderPresent(game->rend);
}