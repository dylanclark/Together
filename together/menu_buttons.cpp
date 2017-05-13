// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include headers
#include "menu_buttons.hpp"
#include "quitmenu_state.hpp"
#include "level1_state.hpp"
#include "level2_state.hpp"
#include "level3_state.hpp"
#include "level4_state.hpp"
#include "level5_state.hpp"
#include "level6_state.hpp"
#include "level7_state.hpp"
#include "level8_state.hpp"
#include "menu.hpp"
#include "mainmenu_state.hpp"
#include "optionsmenu_state.hpp"
#include "newgamemenu_state.hpp"
#include "intro_state.hpp"
#include "levelselectmenu_state.hpp"

// BUTTONS

void new_game_button::select(engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->push_state(new newgamemenu_state);
}

void continue_button::select(engine* game)
{
    int level = game->read_save();
    switch (level)
    {
        case 0:
            Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
            break;
        case 1:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new level1_state);
            game->push_state(new intro_state);
            break;
        case 2:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new level2_state);
            game->push_state(new intro_state);
            break;
        case 3:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new level3_state);
            game->push_state(new intro_state);
            break;
        case 4:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new level4_state);
            game->push_state(new intro_state);
            break;
        case 5:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new level5_state);
            game->push_state(new intro_state);
            break;
        case 6:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new level6_state);
            game->push_state(new intro_state);
            break;
        case 7:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new level7_state);
            game->push_state(new intro_state);
            break;
        case 8:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new level8_state);
            game->push_state(new intro_state);
            break;
        case -1:
            Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
            break;
    }
}

void options_button::select(engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->push_state(new optionsmenu_state);
}

void yes_quit_button::select(engine* game)
{
    game->quit();
}

void yes_newgame_button::select(engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    Mix_FadeOutMusic(200);
    game->save(0);
    game->pop_state();
    game->change_state(new level1_state);
    game->push_state(new intro_state);
}

void no_button::select(engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->pop_state();
}

void quit_button::select(engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->push_state(new quitmenu_state);
}

void resume_button::select(engine* game)
{
    Mix_ResumeMusic();
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->pop_state();
}

void level_select_button::select(engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->push_state(new levelselectmenu_state);
}

void save_and_quit_button::select(engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->change_state(new mainmenu_state);
}

void controls_button::select(engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->pop_state();
}

void back_button::select(engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->pop_state();
}

// SLIDERS


volume_slider::volume_slider(bool selected, int length, bool permanent, int x, int y, int w, int h) : menu_slider(selected, length, permanent, x, y, w, h)
{
    cur_frame = frames - ((float) Mix_VolumeMusic(-1) / (float) MIX_MAX_VOLUME) * ((float) length - 1) - 1;
}

void volume_slider::select(engine *game)
{
    Mix_VolumeMusic((((float) frames - (float) cur_frame - 1) / (float) frames) * (float) MIX_MAX_VOLUME);
}

sfx_slider::sfx_slider(bool selected, int length, bool permanent, int x, int y, int w, int h) : menu_slider(selected, length, permanent, x, y, w, h)
{
    cur_frame = frames - ((float) Mix_Volume(-1, -1) / (float) MIX_MAX_VOLUME) * ((float) length - 1) - 1;
}

void sfx_slider::select(engine *game)
{
    Mix_Volume(-1, (((float) frames - (float) cur_frame - 1) / (float) frames) * (float) MIX_MAX_VOLUME);
}

void level_slider::select(engine *game)
{
    switch (cur_frame + 1)
    {
        case 1:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new level1_state);
            game->push_state(new intro_state);
            break;
        case 2:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new level2_state);
            game->push_state(new intro_state);
            break;
        case 3:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new level3_state);
            game->push_state(new intro_state);
            break;
        case 4:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new level4_state);
            game->push_state(new intro_state);
            break;
        case 5:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new level5_state);
            game->push_state(new intro_state);
            break;
        case 6:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new level6_state);
            game->push_state(new intro_state);
            break;
        case 7:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new level7_state);
            game->push_state(new intro_state);
            break;
        case 8:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new level8_state);
            game->push_state(new intro_state);
            break;
    }
}
