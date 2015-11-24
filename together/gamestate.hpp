
#ifndef scenes_hpp
#define scenes_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

const int MAIN_MENU = 0;
const int SETTINGS = 1;
const int PAUSE_MENU = 2;
const int COLLECTION = 3;
const int QUIT = 4;
const int NEW_GAME = 5;
const int LOAD_GAME = 6;

class scene
{
public:
    // init
    scene();
    
    // dealloc
    ~scene();
    
    // render function
    int render(int id);
};

#endif /* scenes_hpp */
