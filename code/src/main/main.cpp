/*
 *      "together" is a game by Fraser Weist and Dylan Clark.
 *
 *      They're both pretty lame.
 *      But rock socks.
 *      Doubt it.
 */

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include all headers
#include <engine.hpp>
#include <states/mainmenu_state.hpp>

// Let's do this.
int main(void)
{
    // create engine
    engine game;

    // initialize screen
    game.init();

    // initialize game state to main menu
    game.change_state(new mainmenu_state);

    // run game
    while (game.running())
    {
        game.handle_events();
        game.update();
        game.draw();
    }

    // shutdown
    game.cleanup();
}