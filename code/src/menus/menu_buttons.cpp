// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <menu.hpp>
#include <states/quitmenu_state.hpp>
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/mainmenu_state.hpp>
#include <states/optionsmenu_state.hpp>
#include <states/newgamemenu_state.hpp>
#include <states/intro_state.hpp>
#include <states/levelselectmenu_state.hpp>

// BUTTONS

void NewGameButton::select(Engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->push_state(new NewGameMenuState);
}

void ContinueButton::select(Engine* game)
{
    int level = game->read_save();
    switch (level)
    {
        case 0:
            Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
            break;
        case 1:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new Level1State);
            game->push_state(new IntroState);
            break;
        case 2:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new Level2State);
            game->push_state(new IntroState);
            break;
        case 3:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new Level3State);
            game->push_state(new IntroState);
            break;
        case 4:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new Level4State);
            game->push_state(new IntroState);
            break;
        case 5:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new Level5State);
            game->push_state(new IntroState);
            break;
        case 6:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new Level6State);
            game->push_state(new IntroState);
            break;
        case 7:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new Level7State);
            game->push_state(new IntroState);
            break;
        case 8:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            game->change_state(new Level8State);
            game->push_state(new IntroState);
            break;
        case -1:
            Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
            break;
    }
}

void OptionsButton::select(Engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->push_state(new OptionsMenuState);
}

void YesQuitButton::select(Engine* game)
{
    game->quit();
}

void YesNewGameButton::select(Engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    Mix_FadeOutMusic(200);
    game->save(0);
    game->pop_state();
    game->change_state(new Level1State);
    game->push_state(new IntroState);
}

void NoButton::select(Engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->pop_state();
}

void QuitButton::select(Engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->push_state(new QuitMenuState);
}

void ResumeButton::select(Engine* game)
{
    Mix_ResumeMusic();
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->pop_state();
}

void LevelSelectButton::select(Engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->push_state(new LevelSelectMenuState);
}

void SaveAndQuitButton::select(Engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->change_state(new MainMenuState);
}

void ControlsButton::select(Engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->pop_state();
}

void BackButton::select(Engine* game)
{
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->pop_state();
}

// SLIDERS


VolumeSlider::VolumeSlider(bool selected, int length, bool permanent, int x, int y, int w, int h) : MenuSlider(selected, length, permanent, x, y, w, h)
{
    cur_frame = frames - ((float) Mix_VolumeMusic(-1) / (float) MIX_MAX_VOLUME) * ((float) length - 1) - 1;
}

void VolumeSlider::select(Engine* game)
{
    Mix_VolumeMusic((((float) frames - (float) cur_frame - 1) / (float) frames) * (float) MIX_MAX_VOLUME);
}

SfxSlider::SfxSlider(bool selected, int length, bool permanent, int x, int y, int w, int h) : MenuSlider(selected, length, permanent, x, y, w, h)
{
    cur_frame = frames - ((float) Mix_Volume(-1, -1) / (float) MIX_MAX_VOLUME) * ((float) length - 1) - 1;
}

void SfxSlider::select(Engine* game)
{
    Mix_Volume(-1, (((float) frames - (float) cur_frame - 1) / (float) frames) * (float) MIX_MAX_VOLUME);
}

void LevelSlider::select(Engine* game)
{
    switch (cur_frame + 1)
    {
        case 1:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new Level1State);
            game->push_state(new IntroState);
            break;
        case 2:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new Level2State);
            game->push_state(new IntroState);
            break;
        case 3:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new Level3State);
            game->push_state(new IntroState);
            break;
        case 4:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new Level4State);
            game->push_state(new IntroState);
            break;
        case 5:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new Level5State);
            game->push_state(new IntroState);
            break;
        case 6:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new Level6State);
            game->push_state(new IntroState);
            break;
        case 7:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new Level7State);
            game->push_state(new IntroState);
            break;
        case 8:
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
            Mix_FadeOutMusic(200);
            game->change_state(new Level8State);
            game->push_state(new IntroState);
            break;
    }
}
