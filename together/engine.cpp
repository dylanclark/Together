
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <vector>
#include <SDL2_ttf/SDL_ttf.h>

// include headers
#include "engine.hpp"
#include "gamestate.hpp"

bool engine::init(int width, int height)
{
    bool success = true;
    
    // attempt initialization
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        success = false;
    }
    printf("initialization successful!\n");
    
    // creates a window that we can (eventually) draw into
    win = SDL_CreateWindow("together.",
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           width, height, SDL_WINDOW_RESIZABLE);
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
    
    return success;
}

void engine::cleanup()
{
    // clean up!
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