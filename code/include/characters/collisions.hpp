
#ifndef collisions_hpp
#define collisions_hpp

// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
#include <levels/tiles.hpp>

class vector
{
public:
    // init
    vector();

    // coords
    int x;
    int y;
};

bool check_collision(SDL_Rect a, SDL_Rect b, vector* overlap);

#endif /* collisions_hpp */
