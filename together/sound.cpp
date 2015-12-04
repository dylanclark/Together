
// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>

// include headers
#include "sound.hpp"

sound_player::sound_player()
{
    // initialize all music
    menu_music = Mix_LoadMUS("sound/music/menu.wav");
    
    // initialize all fx
    menu_choose_snd = Mix_LoadWAV("sound/fx/menu/choose.wav");
    if (menu_choose_snd == NULL)
    {
        printf("error loading sound: %s\n", Mix_GetError());
    }
    menu_select_snd = Mix_LoadWAV("sound/fx/menu/select.wav");
    menu_onset_snd = Mix_LoadWAV("sound/fx/menu/onset.wav");
    menu_exit_snd = Mix_LoadWAV("sound/fx/menu/exit.wav");
    level_crate_snd = Mix_LoadWAV("sound/fx/level/crate.wav");
    level_switch_snd = Mix_LoadWAV("sound/fx/level/switch2.wav");
}

sound_player::~sound_player()
{
    // free all music
    Mix_FreeMusic(menu_music);
    
    // free all fx
    Mix_FreeChunk(menu_choose_snd);
    Mix_FreeChunk(menu_select_snd);
    Mix_FreeChunk(menu_onset_snd);
    Mix_FreeChunk(menu_exit_snd);
    Mix_FreeChunk(level_crate_snd);
    Mix_FreeChunk(level_switch_snd);
}
