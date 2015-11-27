/*
 *
 *      "together" is a game by Fraser Weist and Dylan Clark.
 *
 *      They're both pretty lame.
 *      But rock socks. 
 *      Doubt it.
 *
 */

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include all headers
#include "tiles.hpp"
#include "level_end.hpp"
#include "initialization.hpp"
#include "characters.hpp"
#include "collisions.hpp"
#include "level_draw.hpp"
#include "textures.hpp"
#include "camera.hpp"
#include "engine.hpp"
#include "gamestate.hpp"
#include "level1_state.hpp"
#include "level2_state.hpp"

// Let's do this.
int main(void)
{
    // create engine
    engine game;
    
    // initialize screen
    game.init(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // initialize game state (to level 1, but eventually to main menu)
    game.change_state(new level1_state);
    
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