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
#include <states/menustate.hpp>
#include <levels.hpp>
#include <editor.hpp>

// Let's do this.
int main(void)
{
    game = new Engine();

    // initialize screen
    game->init();

    // initialize game state to main menu
    game->change_state(new Zonestate(0));

    // run game
    while (game->running())
    {
        game->handle_events();
        game->update();
        game->draw();
    }

    // shutdown
    game->cleanup();
}
