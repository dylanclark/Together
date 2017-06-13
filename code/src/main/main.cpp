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
<<<<<<< HEAD:together/main.cpp
#include "tiles.hpp"
<<<<<<< HEAD
#include "level_end.hpp"
=======
>>>>>>> 2e8ee63... Implemented movable crates!!!
#include "initialization.hpp"
#include "characters.hpp"
#include "collisions.hpp"
#include "level_draw.hpp"
#include "textures.hpp"
#include "camera.hpp"
#include "engine.hpp"
#include "gamestate.hpp"
#include "level1_state.hpp"
#include "button.hpp"
#include "level2_state.hpp"
#include "level3_state.hpp"
#include "springboard.hpp"
#include "mainmenu_state.hpp"
#include "level01_state.hpp"
=======
#include <engine/init.hpp>
#include <engine/engine.hpp>
#include <engine/gamestate.hpp>
#include <states/mainmenu_state.hpp>
>>>>>>> 201bf90... gigantic makefile revamp:code/src/main/main.cpp

// Let's do this.
int main(void)
{
    // create engine
    engine game;

    // initialize screen
    game.init();

    // initialize game state (to level 1, but eventually to main menu)
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