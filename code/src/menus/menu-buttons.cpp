// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <menu.hpp>
#include <states/intro-state.hpp>
#include <levels.hpp>
#include <states/menustate.hpp>

// BUTTONS

NewGameButton::NewGameButton(int x, int y, int w, int h) : MenuButton(x,y,w,h)
{
    if (!tex.load_object(96, 16, "menu/new_game.png")) {
        printf("failed to load new game button texture!\n");
    }
}

void NewGameButton::select()
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->push_state(new NewGameMenuState);
}

ContinueButton::ContinueButton(int x, int y, int w, int h) : MenuButton(x,y,w,h)
{
    if (!tex.load_object(96, 16, "menu/continue.png")) {
        printf("failed to load continue button texture!\n");
    }
}

void ContinueButton::select()
{
    int level = game->read_save();
    if (level == -1 || level == 0) {
        Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    } else {
        Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
        // TODO select zone
    }
}

OptionsButton::OptionsButton(int x, int y, int w, int h) : MenuButton(x,y,w,h)
{
    if (!tex.load_object(64, 16, "menu/options.png")) {
        printf("failed to load options button texture!\n");
    }
}

void OptionsButton::select()
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->push_state(new OptionsMenuState);
}

QuitButton::QuitButton(int x, int y, int w, int h) : MenuButton(x,y,w,h)
{
    if (!tex.load_object(32, 16, "menu/quit.png")) {
        printf("failed to load quit button texture!\n");
    }
}

void QuitButton::select()
{
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->push_state(new QuitMenuState);
}

YesQuitButton::YesQuitButton(int x, int y, int w, int h) : MenuButton(x,y,w,h)
{
    if (!tex.load_object(32, 16, "menu/yes_quit.png"))
    {
        printf("Failed to load yes button texture!\n");
        return;
    }
}

void YesQuitButton::select()
{
    game->quit();
}

YesNewGameButton::YesNewGameButton(int x, int y, int w, int h) : MenuButton(x,y,w,h)
{
    if (!tex.load_object(32, 16, "menu/yes.png")) {
        printf("Failed to load yes button texture!\n");
    }
}

void YesNewGameButton::select()
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    Mix_FadeOutMusic(200);
    game->save(0);
    game->pop_state();
    game->change_state(new Zonestate(0));
}

NoButton::NoButton(int x, int y, int w, int h) : MenuButton(x,y,w,h)
{
    if (!tex.load_object(32, 16, "menu/no.png")) {
        printf("Failed to load no button texture!\n");
    }
}

void NoButton::select()
{
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->pop_state();
}

ResumeButton::ResumeButton(int x, int y, int w, int h) : MenuButton(x,y,w,h)
{
    if (!tex.load_object(64, 16, "menu/resume.png")) {
        printf("Failed to load resume button object!\n");
    }
}

void ResumeButton::select()
{
    Mix_ResumeMusic();
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->pop_state();
}

LevelSelectButton::LevelSelectButton(int x, int y, int w, int h) : MenuButton(x,y,w,h)
{
    if (!tex.load_object(128, 16, "menu/level_select.png")) {
        printf("Failed to load level select button texture!\n");
    }
}

void LevelSelectButton::select()
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->push_state(new LevelSelectMenuState);
}

SaveAndQuitButton::SaveAndQuitButton(int x, int y, int w, int h) : MenuButton(x,y,w,h)
{
    if (!tex.load_object(128, 16, "menu/save_and_quit.png")) {
        printf("Failed to load save/quit button texture!\n");
    }
}

void SaveAndQuitButton::select()
{
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->change_state(new MainMenuState);
}

ControlsButton::ControlsButton(int x, int y, int w, int h) : MenuButton(x,y,w,h)
{
    if (!tex.load_object(96, 16, "menu/controls.png")) {
        printf("Failed to load controls button texture!\n");
    }
}

void ControlsButton::select()
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    game->pop_state();
}

BackButton::BackButton(int x, int y, int w, int h) : MenuButton(x,y,w,h)
{
    if (!tex.load_object(64, 16, "menu/back.png")) {
        printf("Failed to load back button texture!\n");
    }
}

void BackButton::select()
{
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->pop_state();
}

// SLIDERS

VolumeSlider::VolumeSlider(int length, bool permanent, int x, int y, int w, int h) : MenuSlider(length, permanent, x, y, w, h)
{
    if (!tex.load_object(128, 16, "menu/volume.png")) {
        printf("Failed to load volume slider object!\n");
    }
    cur_frame = frames - ((float) Mix_VolumeMusic(-1) / (float) MIX_MAX_VOLUME) * ((float) length - 1) - 1;
}

void VolumeSlider::select()
{
    Mix_VolumeMusic((((float) frames - (float) cur_frame - 1) / (float) frames) * (float) MIX_MAX_VOLUME);
}

SfxSlider::SfxSlider(int length, bool permanent, int x, int y, int w, int h) : MenuSlider(length, permanent, x, y, w, h)
{
    if (!tex.load_object(128, 16, "menu/sfx.png")) {
        printf("Failed to load level select button texture!\n");
    }
    cur_frame = frames - ((float) Mix_Volume(-1, -1) / (float) MIX_MAX_VOLUME) * ((float) length - 1) - 1;
}

void SfxSlider::select()
{
    Mix_Volume(-1, (((float) frames - (float) cur_frame - 1) / (float) frames) * (float) MIX_MAX_VOLUME);
}

LevelSlider::LevelSlider(int length, bool permanent, int x, int y, int w, int h) : MenuSlider(length, permanent, x, y, w, h)
{
    if (!tex.load_object(96, 16, "menu/level_slider.png")) {
        printf("Failed to load level slider object!\n");
    }
}

void LevelSlider::select()
{
    Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
    Mix_FadeOutMusic(200);
    game->change_state(new Zonestate(cur_frame + 1));
}
