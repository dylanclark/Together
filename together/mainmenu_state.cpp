 
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>

// include headers
#include "mainmenu_state.hpp"
#include "engine.hpp"
#include "menu.hpp"
#include "initialization.hpp"
#include "level1_state.hpp"
#include "level2_state.hpp"
#include "quitmenu_state.hpp"
#include "menu_buttons.hpp"

void mainmenu_state::init(engine* game)
{
    if (!fade_in_tex.load_object(16, 16, "textures/white/background/white_back.png", game->rend))
    {
        printf("Failed to load fade-in object!\n");
        return;
    }
    else
    {
        fade_in_tex.set_blend_mode(SDL_BLENDMODE_BLEND);
    }
    
    if (!title.load_object(128, 16, "textures/menu/title.png", game->rend))
    {
        printf("Failed to load title texture!\n");
        return;
    }
    
    if (!new_game_button.load_object(96, 16, "textures/menu/new_game.png", game->rend))
    {
        printf("Failed to load new game button texture!\n");
        return;
    }
    if (!continue_button.load_object(64, 16, "textures/menu/continue.png", game->rend))
    {
        printf("Failed to load continue button texture!\n");
        return;
    }
    if (!options_button.load_object(64, 16, "textures/menu/options.png", game->rend))
    {
        printf("Failed to load options button texture!\n");
        return;
    }
    if (!quit_button.load_object(32, 16, "textures/menu/quit.png", game->rend))
    {
        printf("Failed to load quit button texture!\n");
        return;
    }

    
    main_menu = new class menu(game->screen_width, game->screen_height, 100);
    main_menu->size = 4;
    
    main_menu->menu_title = new class title (game->screen_width / 2,  game->screen_height / 4, 1600, 200);
    main_menu->menu_title->tex = &title;
    
    main_menu->buttons.push_back(new class new_game_button(true, game->screen_width / 2, game->screen_height / 2, 400, 60));
    main_menu->buttons[0]->tex = &new_game_button;
    
    main_menu->buttons.push_back(new class continue_button(true, game->screen_width / 2, game->screen_height / 2 + 75, 300, 60));
    main_menu->buttons[1]->tex = &continue_button;
    
    main_menu->buttons.push_back(new class options_button(true, game->screen_width / 2, game->screen_height / 2 + 150, 300, 60));
    main_menu->buttons[2]->tex = &options_button;
    
    main_menu->buttons.push_back(new class quit_button(true, game->screen_width / 2, game->screen_height / 2 + 225, 150, 60));
    main_menu->buttons[3]->tex = &quit_button;
    
    main_menu->fade_in->tex = &fade_in_tex;
    
    // start bg music
    Mix_PlayMusic(game->sound->menu_music, -1);
    
}

void mainmenu_state::cleanup()
{
    free(main_menu);
    
    delete main_menu->fade_in;
    main_menu->fade_in = nullptr;
    
    new_game_button.free();
    continue_button.free();
    options_button.free();
    quit_button.free();
}

void mainmenu_state::pause()
{
    printf("paused menu\n");
    main_menu->fade_in->alpha = 0;
}

void mainmenu_state::resume()
{
    printf("paused menu\n");
}

void mainmenu_state::handle_events(engine* game)
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
        
        if (!main_menu->handle_event(event, game))
        {
            game->quit();
        }
    }
}

void mainmenu_state::update(engine* game)
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

void mainmenu_state::draw(engine* game)
{
    for (int i = 0; i < main_menu->buttons.size(); i++)
    {
        main_menu->buttons[i]->render(game->rend);
    }

    main_menu->menu_title->render(game->rend);
    main_menu->fade_in->render(game->rend);
    
    SDL_RenderPresent(game->rend);
}