
#ifndef sound_hpp
#define sound_hpp

// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class SoundPlayer
{
public:
    SoundPlayer();
    ~SoundPlayer();

    // all music
    Mix_Music* menu_music;

    // all fx
    Mix_Chunk* menu_select_snd;
    Mix_Chunk* menu_choose_snd;
    Mix_Chunk* menu_onset_snd;
    Mix_Chunk* menu_exit_snd;
    Mix_Chunk* level_crate_snd;
    Mix_Chunk* level_switch_snd;
    Mix_Chunk* level_b_jump_snd;
    Mix_Chunk* level_w_jump_snd;
};

#endif /* sound_hpp */
