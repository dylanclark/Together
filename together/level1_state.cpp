
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include headers
#include "level1_state.hpp"
#include "characters.hpp"
#include "tiles.hpp"
#include "crate.hpp"

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
    if (!crate_tex_four_by_two.load_object(TILE_WIDTH * 4, TILE_WIDTH * 2, "textures/black/crates/b_crate.png", game->rend))
    {
        printf("Failed to load crate (4x2) texture!\n");
        return;
    }
    
    // initialize level
    width = 27;
    height = 19;
    
    path = "levels/level_001.csv";
    
    if (!set_tiles(tileset, path, width, height))
    {
        printf("Failed to load level 1 map!\n");
        return;
    }
    
    // initialize black dot
    b_char.status = CHAR_ACTIVE;
    b_char.tex = b_char_tex;
    b_char.col_rect.x = 2 * TILE_WIDTH;
    b_char.col_rect.y = 8 * TILE_WIDTH;
    b_char.black = true;
    
    // initialize white dot
    w_char.status = CHAR_INACTIVE;
    w_char.tex = w_char_tex;
    w_char.col_rect.x = 2 * TILE_WIDTH;
    w_char.col_rect.y = 9 * TILE_WIDTH;
    w_char.black = false;
    
    // init crate #1
    crates.push_back(new crate(5 * TILE_WIDTH, 7  * TILE_WIDTH, FOUR_BY_TWO));
    crates.back()->tex = crate_tex_four_by_two;
    crates.back()->black = true;
    
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
            game->quit();
        
        if (!w_char.handle_event(event, this, game))
            game->quit();
        
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
    camera.track(&b_char.col_rect, &w_char.col_rect);
    
    // move that camera!
    camera.move(width, height);
}

void level1_state::draw(engine* game)
{
    // draw stuff to the screen!
    for (int i = 0; i < (width * height); i++)
    {
        tileset[i]->render(b_char.status, &camera.display, game->rend, &tile_tex);
    }
    
    for (int i = 0; i < crates.size(); i++)
    {
        crates[i]->render(b_char.status, &camera.display, game->rend, this);
    }
    
    b_char.render(&camera.display, game->rend);
    w_char.render(&camera.display, game->rend);
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
    tile_tex.free();
    crate_tex_four_by_two.free();
    
}

void level1_state::pause()
{
    return;
}

void level1_state::resume()
{
    return;
}