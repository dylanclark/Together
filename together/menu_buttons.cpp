//
//  menu_buttons.cpp
//  together
//
//  Created by Fraser Weist on 11/30/15.
//  Copyright © 2015 together. All rights reserved.
//

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include headers
#include "menu_buttons.hpp"
#include "quitmenu_state.hpp"
#include "level1_state.hpp"
#include "level2_state.hpp"
#include "menu.hpp"
#include "mainmenu_state.hpp"
#include "optionsmenu_state.hpp"

// BUTTONS

void new_game_button::select(engine* game)
{
    game->change_state(new level1_state);
}

void continue_button::select(engine* game)
{
    game->change_state(new level1_state);
}

void options_button::select(engine* game)
{
    game->push_state(new optionsmenu_state);
}

void yes_button::select(engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->quit();
}

void no_button::select(engine* game)
{
    game->pop_state();
}

void quit_button::select(engine* game)
{
    game->push_state(new quitmenu_state);
}

void resume_button::select(engine* game)
{
    Mix_ResumeMusic();
    game->pop_state();
}

void level_select_button::select(engine* game)
{
    game->pop_state();
}

void save_and_quit_button::select(engine* game)
{
    game->change_state(new mainmenu_state);
}

void controls_button::select(engine* game)
{
    game->pop_state();
}

void back_button::select(engine* game)
{
    game->pop_state();
}

// SLIDERS


volume_slider::volume_slider(bool selected, int length, int x, int y, int w, int h) : menu_slider(selected, length, x, y, w, h)
{
    cur_frame = frames - ((float) Mix_VolumeMusic(-1) / (float) MIX_MAX_VOLUME) * ((float) length - 1) - 1;
}
void volume_slider::select(engine *game)
{
    Mix_VolumeMusic((((float) frames - (float) cur_frame - 1) / (float) frames) * (float) MIX_MAX_VOLUME);
}

sfx_slider::sfx_slider(bool selected, int length, int x, int y, int w, int h) : menu_slider(selected, length, x, y, w, h)
{
    cur_frame = frames - ((float) Mix_Volume(-1, -1) / (float) MIX_MAX_VOLUME) * ((float) length - 1) - 1;
    printf("cur sfx volume: %i, frame: %i\n", Mix_Volume(-1, -1), cur_frame);
}

void sfx_slider::select(engine *game)
{
    Mix_Volume(-1, (((float) frames - (float) cur_frame - 1) / (float) frames) * (float) MIX_MAX_VOLUME);
}
