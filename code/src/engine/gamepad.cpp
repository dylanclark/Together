
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <engine/gamepad.hpp>

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

controller::~controller()
{
    SDL_JoystickClose(joystick);
    SDL_GameControllerClose(gamepad);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD:code/src/engine/gamepad.cpp
    joystick = NULL;
}
=======
    joystick = nullptr;
}
>>>>>>> 04da4d0... added a dependency file to help for Makefile conversion:together/gamepad.cpp
=======
    joystick = NULL;
}
>>>>>>> d9ca289... readded conflicting files
=======
    joystick = NULL;
}
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
