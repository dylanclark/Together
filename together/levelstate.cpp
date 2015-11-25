
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include headers
#include "gamestate.hpp"
#include "levelstate.hpp"



/*void levelstate::handle_events(engine *game)
{
    // event handler
    SDL_Event event;
    
    // handle those events, bruh
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                game->quit();
                break;
        }
        
        // quit if he pressed escape
        if (!b_char.handle_event(event))
            game->quit();
        
        if (!w_char.handle_event(event))
            game->quit();
        
    }
}

void levelstate::update(engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);
    
    // move the square
    b_char.move(tileset, width, height);
    w_char.move(tileset, width, height);
    
    // track the player
    camera.track(&b_char.col_rect, &w_char.col_rect);
    
    // move that camera!
    camera.move(this->width, this->height);
}

void levelstate::draw(engine* game)
{
    // draw stuff to the screen!
    for (int i = 0; i < (width * height); i++)
    {
        tileset[i]->render(b_char.status, &camera.display, game->rend);
    }
    
    b_char.render(&camera.display, game->rend);
    w_char.render(&camera.display, game->rend);
    SDL_RenderPresent(game->rend);
}

void levelstate::cleanup()
{
    // iterate over all tiles and delete them all
    for (int i = 0; i < width * height; i++)
    {
        if (tileset[i] == NULL)
        {
            delete tileset[i];
            tileset[i] = NULL;
        }
    }
}

void levelstate::pause()
{
    return;
}

void levelstate::resume()
{
    return;
}*/