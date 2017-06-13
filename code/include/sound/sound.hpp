
#ifndef sound_hpp
#define sound_hpp

// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

class sound_player
{
public:
    // init
    sound_player();

    // dealloc
    ~sound_player();

    // all music
    Mix_Music* menu_music = NULL;

    // all fx
    Mix_Chunk* menu_select_snd = NULL;
    Mix_Chunk* menu_choose_snd = NULL;
    Mix_Chunk* menu_onset_snd = NULL;
    Mix_Chunk* menu_exit_snd = NULL;
    Mix_Chunk* level_crate_snd = NULL;
    Mix_Chunk* level_switch_snd = NULL;
    Mix_Chunk* level_b_jump_snd = NULL;
    Mix_Chunk* level_w_jump_snd = NULL;
};

#endif /* sound_hpp */
