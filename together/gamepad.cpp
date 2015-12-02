
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include headers
#include "gamepad.hpp"

controller::controller()
{
    // check for joystick
    if (SDL_NumJoysticks() < 1)
    {
        printf("warning: no joysticks connected!\n");
        return;
    }
    
    // init joystick
    joystick = SDL_JoystickOpen(0);
    if (joystick == nullptr)
    {
        printf("Warning: Unable to open controller! SDL_Error: %s\n", SDL_GetError());
    }
    
    // init gamepad
    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        if (SDL_IsGameController(i))
        {
            gamepad = SDL_GameControllerOpen(i);
            break;
        }
    }
    gamepad = SDL_GameControllerOpen(0);
    if (gamepad == nullptr)
    {
        printf("Warning: Unable to open controller: SDL_Error: %s\n", SDL_GetError());
    }
    
    
}

controller::~controller()
{
    SDL_JoystickClose(joystick);
    SDL_GameControllerClose(gamepad);
    joystick = nullptr;
}