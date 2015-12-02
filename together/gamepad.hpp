
#ifndef gamepad_hpp
#define gamepad_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

class controller
{
public:
    // init
    controller();
    
    // dealloc
    ~controller();
    
    // joystick
    SDL_Joystick* joystick;
    
    // gamepad
    SDL_GameController* gamepad;
    
    // dead zone constant
    const int DEAD_ZONE = 8000;
};

#endif /* gamepad_hpp */
