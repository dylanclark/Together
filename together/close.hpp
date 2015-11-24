
#ifndef close_hpp
#define close_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include header files
#include "tiles.hpp"

// close the window
bool close(SDL_Renderer* rend, SDL_Window* win, tile* tiles[], int level_w, int level_h);

#endif /* close_hpp */
