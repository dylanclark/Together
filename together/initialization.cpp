
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include header file
#include "initialization.hpp"
#include "textures.hpp"
#include "level_draw.hpp"

// window initialization
SDL_Window* win;

// renderer initialization
SDL_Renderer* rend;

// initialize all textures
texture tile_tex;
texture b_char_tex;
texture w_char_tex;

bool init()
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
                                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
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
    
    //Initialize renderer color
    SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0xFF, 0xFF);
    
    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        success = false;
    }
    
    return success;
}

bool load_media(tile* tiles[], int level_w, int level_h)
{
    // success flag
    bool success = true;
    
    // black square texture
    if (!b_char_tex.load_tile_sheet("textures/black/b_char.png"))
    {
        printf("Failed to load black dot texture!\n");
        success = false;
    }
    if (!w_char_tex.load_tile_sheet("textures/white/w_char.png"))
    {
        printf("Failed to load white dot texture!\n");
        success = false;
    }
    if (!tile_tex.load_tile_sheet("textures/tile_sheet.png"))
    {
        printf("Failed to load tile sheet texture!\n");
        success = false;
    }
    if (!set_tiles(tiles, "levels/level_001.map", level_w, level_h))
    {
        printf("Failed to load tile texture!\n");
        success = false;
    }
    
    return success;
}