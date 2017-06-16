
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <engine.hpp>

Controller::Controller()
{
    // check for joystick
    if (SDL_NumJoysticks() < 1)
    {
        return;
    }

    // init joystick
    joystick = SDL_JoystickOpen(0);
    if (joystick == NULL)
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
    if (gamepad == NULL)
    {
        printf("Warning: Unable to open controller: SDL_Error: %s\n", SDL_GetError());
    }


}

Controller::~Controller()
{
    SDL_JoystickClose(joystick);
    SDL_GameControllerClose(gamepad);
    joystick = NULL;
}