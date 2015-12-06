
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

// include headers
#include "level1_state.hpp"
#include "level7_state.hpp"
#include "level8_state.hpp"
#include "level6_state.hpp"
#include "level2_state.hpp"
#include "mainmenu_state.hpp"
#include "pausemenu_state.hpp"
#include "characters.hpp"
#include "tiles.hpp"
#include "crate.hpp"
#include "level_end.hpp"
#include "gamestate.hpp"
#include "button.hpp"
#include "engine.hpp"
#include "springboard.hpp"
#include "menu.hpp"
#include "level_messages.hpp"

void level1_state::init(engine* game)
{
    // load textures
    load_textures(game);
   
    // initialize objects
    init_objects(game);
    
    if (game->read_save() < 1)
    {
        game->save(1);
    }
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
        if (!b_char.handle_event(event, this, game))
        {
            Mix_PauseMusic();
            Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
            game->push_state(new pausemenu_state);
        }
        
        w_char.handle_event(event, this, game);
        
    }
    
    shiftable = true;
}

void level1_state::update(engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);
    
    // move the square
    if (b_char.status == CHAR_ACTIVE)
        b_char.move(this);
    if (w_char.status == CHAR_ACTIVE)
        w_char.move(this);
    
    for (int i = 0; i < crates.size(); i++)
    {
        crates[i]->update();
    }
    
    // track the player
    camera->track(&b_char.col_rect, &b_char.col_rect);
    
    // move that camera!
    camera->move(width, height, game);
    
    interactions(game);
}

void level1_state::draw(engine* game)
{
    // draw stuff to the screen!
    for (int i = 0; i < (width * height); i++)
    {
        tileset[i]->render(b_char.status, &camera->display, game, &tile_tex);
    }
    
    for (int i = 0; i < crates.size(); i++)
    {
        crates[i]->render(b_char.status, &camera->display, game, this);
    }
    
    b_char.render(&camera->display, game);
    b_level_end.render(&camera->display, game);
    SDL_RenderPresent(game->rend);
}

void level1_state::cleanup()
{
    // iterate over all tiles and delete them all
    for (int i = 0; i < width * height; i++)
    {
        if (tileset[i] != NULL)
        {
            delete tileset[i];
            tileset[i] = NULL;
        }
    }
    
    for (int i = 0; i < crates.size(); i++)
    {
        if (crates[i] != NULL)
        {
            delete crates[i];
            crates.pop_back();
        }
    }
    
    // free all textures
    b_char_tex.free();
    w_char_tex.free();
    
}

void level1_state::pause()
{
    return;
}

void level1_state::resume()
{
    return;
}

void level1_state::load_textures(engine* game)
{
    // LOAD ALL TEXTURES
    if (!b_char_tex.load_tile_sheet("textures/black/b_char.png", game->rend))
    {
        printf("Failed to load black dot texture!\n");
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
    
    // initialize level
    width = 26;
    height = 17;
    
    path = "levels/level_01.csv";
    
    if (!set_tiles(tileset, path, width, height))
    {
        printf("Failed to load level 1 map!\n");
        return;
    }
}

void level1_state::init_objects(engine* game)
{
    // initialize black dot
    b_char.status = CHAR_ACTIVE;
    b_char.tex = b_char_tex;
    b_char.col_rect.x = 2 * TILE_WIDTH;
    b_char.col_rect.y = 4 * TILE_WIDTH;
    b_char.black = true;
    
    // initialize black level end
    b_level_end.tex = b_end_tex;
    b_level_end.col_rect.x = 1360;
    b_level_end.col_rect.y = 720;
    
    
    
    camera = new class camera(game->screen_width, game->screen_height);
    
}

void level1_state::interactions(engine* game)
{
    // if both are on level end object
    if(b_level_end.check(b_char.col_rect))
    {
        b_char.center(&b_level_end.col_rect);
        
        // change state to level 2
        change_state(game, new level8_state);
    }
    
}