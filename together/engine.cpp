// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <vector>
#include <SDL2_ttf/SDL_ttf.h>
#include <fstream>
#include <iostream>

// include headers
#include "engine.hpp"
#include "gamestate.hpp"

bool engine::init()
{
    bool success = true;
    
    // attempt initialization
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK|SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        success = false;
    }
    printf("initialization successful!\n");
    
    SDL_DisplayMode current;
    
    if (SDL_GetCurrentDisplayMode(0, &current) != 0)
    {
        printf("could not get display info: %s\n", SDL_GetError());
    }
    
    screen_width = current.w;
    screen_height = current.h;
    
    // creates a window that we can (eventually) draw into
    win = SDL_CreateWindow("together.",
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           current.w, current.h, SDL_WINDOW_RESIZABLE);
    if (!win)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        success = false;
    }
    
    // create a renderer, which sets up graphics hardward
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        success = false;
    }
    
    // initialize audio
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 256 ) == -1)
    {
        printf("error initializing audio: %s\n", SDL_GetError());
        Mix_CloseAudio();
        success = false;
    }
    
    if (SDL_ShowCursor(SDL_DISABLE) != 0)
    {
        printf("could not hide cursor: %s\n", SDL_GetError());
    }
    
    sound = new sound_player;
    
    // clear the window
    SDL_RenderClear(rend);
    
    // initialize renderer color
    SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0xFF, 0xFF);
    
    // initialize PNG loading
    int img_flags = IMG_INIT_PNG;
    if(!(IMG_Init(img_flags) & img_flags))
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        success = false;
    }
    
    
    if (save_file.fail())
    {
        printf("error opening save_file\n");
    }
    if (save_file.fail())
    {
        printf("error opening save reader\n");
    }
    
    return success;
}

void engine::cleanup()
{
    // clean up!
    free(sound);
    save_file.close();
    save_reader.close();
    Mix_CloseAudio();
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void engine::change_state(gamestate* state)
{
    if (!states.empty())
    {
        states.back()->cleanup();
        states.pop_back();
    }
    
    states.push_back(state);
    states.back()->init(this);
}

void engine::push_state(gamestate* state)
{
    if (!states.empty())
    {
        states.back()->pause();
    }
    
    states.push_back(state);
    states.back()->init(this);
}

void engine::pop_state()
{
    states.back()->cleanup();
    states.pop_back();
}

void engine::restart_state()
{
    states.back()->cleanup();
    states.back()->init(this);
}

void engine::handle_events()
{
    states.back()->handle_events(this);
}

void engine::update()
{
    states.back()->update(this);
}

void engine::draw()
{
    states.back()->draw(this);
}

bool engine::save(int level)
{
    bool success = true;
    
    save_file.open("save/save_file.txt");
    save_file << level;
    save_file.close();
    
    if (!save_file)
    {
        printf("error saving!\n");
        success = false;
    }
    
    return success;
}

int engine::read_save()
{
    int to_return = -1;
    
    save_reader.open("save/save_file.txt");
    save_reader >> to_return;
    save_reader.close();
    
    if (!save_reader)
    {
        printf("error reading save!\n");
    }
    
    return to_return;
}
