
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include headers
#include "level1_state.hpp"
#include "level2_state.hpp"
#include "characters.hpp"
#include "tiles.hpp"
#include "level_end.hpp"
#include "gamestate.hpp"
#include "button.hpp"
#include "engine.hpp"

level1_state level1_state::level1_state_inst;

void level1_state::init(engine* game)
{
    // LOAD ALL TEXTURES
    if (!b_char_tex.load_tile_sheet("textures/black/b_char.png", game->rend))
    {
        printf("Failed to load black dot texture!\n");
        return;
    }
    if (!w_char_tex.load_tile_sheet("textures/white/w_char.png", game->rend))
    {
        printf("Failed to load white dot texture!\n");
        return;
    }
    if (!tile_tex.load_tile_sheet("textures/tile_sheet.png", game->rend))
    {
        printf("Failed to load tile sheet texture!\n");
        return;
    }
    if (!b_end_tex.load_tile_sheet("textures/black/level_end/black_end.png", game->rend))
    {
        printf("Failed to load black level end texter!\n");
        return;
    }
    if (!w_end_tex.load_tile_sheet("textures/white/level_end/white_end.png", game->rend))
    {
        printf("Failed to load  white level end texture!\n");
        return;
    }
    if(!b_button_tex.load_tile_sheet("textures/black/button/b_button.png", game->rend))
    {
        printf("Failed to load black button texture!\n");
        return;
    }
    if(!w_button_tex.load_tile_sheet("textures/white/button/w_button.png", game->rend))
    {
        printf("Failed to load white button texture!\n");
        return;
    }
    
    // initialize level
    width = 44;
    height = 18;
    
    path = "levels/level_001.map";
    
    if (!set_tiles(tileset, path, width, height))
    {
        printf("Failed to load level 1 map!\n");
        return;
    }
    
    // initialize black dot
    b_char.status = CHAR_ACTIVE;
    b_char.tex = b_char_tex;
    b_char.col_rect.x = 120;
    b_char.col_rect.y = 200;
    b_char.black = true;
    
    // initialize white dot
    w_char.status = CHAR_INACTIVE;
    w_char.tex = w_char_tex;
    w_char.col_rect.x = 120;
    w_char.col_rect.y = 400;
    w_char.black = false;
    
    // initialize black level end
    b_level_end.tex = b_end_tex;
    b_level_end.col_rect.x = 1000;
    b_level_end.col_rect.y = 360;
    
    // initialize black level end
    w_level_end.tex = w_end_tex;
    w_level_end.col_rect.x = 1000;
    w_level_end.col_rect.y = 420;
    
    // initialize black button
    //b_button.tex = b_button_tex;
    //b_button.col_rect.x = 1200;
    //b_button.col_rect.y = 360;
    //b_button.activated = false;
    ///b_button.single = false;
//b_button.black = true;

    // initialize white button
    //w_button.tex = w_button_tex;
    //w_button.col_rect.x = 1300;
    //w_button.col_rect.y = 420;
    //b_button.activated = false;
    //b_button.single = false;
   // b_button.black = false;
}

void level1_state::handle_events(engine *game)
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

void level1_state::update(engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);
    
    // move the square
    b_char.move(tileset, width, height);
    w_char.move(tileset, width, height);
    
    // track the player
    camera.track(&b_char.col_rect, &w_char.col_rect);
    
    // move that camera!
    camera.move(width, height);
    
    
    // if both are on level end object
    if(b_level_end.check(b_char.col_rect) && w_level_end.check(w_char.col_rect))
    {
        // do end animation
        b_char.completed(width, height, &b_level_end.col_rect);
        w_char.completed(width, height, &w_level_end.col_rect);
        
        // change state to level 2
        change_state(game, new level2_state);
    }
    
    // if black button is activated
    //if(b_button.check(b_char.col_rect))
    //{
        // activate
        //b_button.activated = true;
        
        //printf("Active");
    //}
    // if white button is actited
    //if(w_button.check(w_char.col_rect))
    //{
        // activate
       // w_button.activated = true;
    //}
    
}

void level1_state::draw(engine* game)
{
    // draw stuff to the screen!
    for (int i = 0; i < (width * height); i++)
    {
        tileset[i]->render(b_char.status, &camera.display, game->rend, &tile_tex);
    }
    
    b_char.render(&camera.display, game->rend);
    w_char.render(&camera.display, game->rend);
    b_level_end.render(&camera.display, game->rend);
    w_level_end.render(&camera.display, game->rend);
    //b_button.render(&camera.display, game->rend);
//w_button.render(&camera.display, game->rend);
    SDL_RenderPresent(game->rend);
}

void level1_state::cleanup()
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

void level1_state::pause()
{
    return;
}

void level1_state::resume()
{
    return;
}