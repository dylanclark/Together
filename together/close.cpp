
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include header file
#include "close.hpp"
#include "textures.hpp"

// reinitialize textures
extern texture b_char_tex;
extern texture w_char_tex;
extern texture tile_tex;

// close the window
bool close(SDL_Renderer* rend, SDL_Window* win, tile* tiles[], int level_w, int level_h)
{
    // iterate over all tiles and delete them all
    for (int i = 0; i < level_h * level_w; i++)
    {
        if (tiles[i] == NULL)
        {
            delete tiles[i];
            tiles[i] = NULL;
        }
    }
    
    // free textures
    b_char_tex.free();
    w_char_tex.free();
    tile_tex.free();
    
    // clean up!
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return true;
}
