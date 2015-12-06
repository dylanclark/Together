
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

// include headers
#include "level1_state.hpp"
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
#include "level01_state.hpp"

void level01_state::init(engine* game)
{
    // load textures
    load_textures(game);
    
    // initialize objects
    init_objects(game);
    
}

void level01_state::handle_events(engine *game)
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

void level01_state::update(engine* game)
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
    camera->track(&b_char.col_rect, &w_char.col_rect);
    
    // move that camera!
    camera->move(width, height, game);
    
    interactions(game);
}

void level01_state::draw(engine* game)
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
    w_char.render(&camera->display, game);
    b_level_end.render(&camera->display, game);
    w_level_end.render(&camera->display, game);
    b_button.render(&camera->display, game);
    w_button.render(&camera->display, game);
    b_springboard.render(&camera->display, game);
    w_springboard.render(&camera->display, game);
    
    b_cross_spring.render(&camera->display, game);
    SDL_RenderPresent(game->rend);
}

void level01_state::cleanup()
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
    b_button_tex.free();
    w_button_tex.free();
    b_springboard_tex.free();
    w_springboard_tex.free();
    w_end_animate.free();
    b_end_animate.free();
    b_springboard_tex.free();
    w_springboard_tex.free();
    b_cross_spring_tex.free();
    w_cross_spring_tex.free();
    level1_end_tex.free();
    level1_start_tex.free();
    
}

void level01_state::pause()
{
    return;
}

void level01_state::resume()
{
    return;
}

void level01_state::load_textures(engine* game)
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
    if (!crate_tex_four_by_two.load_object(TILE_WIDTH * 4, TILE_WIDTH * 2, "textures/black/crates/b_crate.png", game->rend))
    {
        printf("Failed to load crate (4x2) texture!\n");
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
    if(!b_springboard_tex.load_tile_sheet("textures/black/spring/b_spring.png", game->rend))
    {
        printf("Failed to load black springboard texture!\n");
        return;
    }
    if(!b_end_animate.load_tile_sheet("textures/black/end_char/b_end_animate.png", game->rend))
    {
        printf("Failed to load black animating texture!\n");
        return;
    }
    if(!w_end_animate.load_tile_sheet("textures/white/end_char/w_end_animate.png", game->rend))
    {
        printf("Failed to load black animating texture!\n");
        return;
    }
    if(!w_cross_spring_tex.load_tile_sheet("textures/white/cross_layer/w_cross.png", game->rend))
    {
        printf("Failed to load white cross layer spring texture!\n");
        return;
    }
    if(!b_cross_spring_tex.load_tile_sheet("textures/black/cross_layer/b_cross.png", game->rend))
    {
        printf("Failed to black white cross layer spring texture!\n");
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
}

void level01_state::init_objects(engine* game)
{
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
    
    
    // initialize black cross - layer spring, a variant of springboard
    b_cross_spring.tex = b_cross_spring_tex;
    b_cross_spring.status = BOARD_INACTIVE;
    b_cross_spring.col_rect.x = 880;
    b_cross_spring.col_rect.y = 490;
    b_cross_spring.col_rect.h = 120;
    b_cross_spring.show = true;
    b_cross_spring.y_spring = 6;
    b_cross_spring.direction = FLIP_RIGHT;
    
    camera = new class camera(game->screen_width, game->screen_height);
    
    // init crate #1
    crates.push_back(new crate(5 * TILE_WIDTH, 7  * TILE_WIDTH, FOUR_BY_TWO));
    crates.back()->tex = crate_tex_four_by_two;
    crates.back()->black = true;
    
    // initialize black button
    b_button.tex = b_button_tex;
    b_button.col_rect.x = 1200;
    b_button.col_rect.y = 480;
    b_button.single = true;
    b_button.direction = RIGHT;
    
    // initialize white button
    w_button.tex = w_button_tex;
    w_button.col_rect.x = 200;
    w_button.col_rect.y = 540;
    w_button.direction = UP;
    w_button.status = BUTT_INACTIVE;
    
    // initialize black springboard
    b_springboard.tex = b_springboard_tex;
    b_springboard.col_rect.x = 1120;
    b_springboard.col_rect.y = 480;
    b_springboard.show = true;
    b_springboard.direction = FLIP_RIGHT;
    b_springboard.x_spring = 0;
    b_springboard.y_spring = 10;
    
    // initialize white springboard
    w_springboard.tex = w_springboard_tex;
    w_springboard.col_rect.x = 1160;
    w_springboard.col_rect.y = 540;
    w_springboard.show = true;
    w_springboard.direction = FLIP_RIGHT;
    w_springboard.x_spring = 0;
    w_springboard.y_spring = 10;

    
    
}

void level01_state::interactions(engine* game)
{
    // if both are on level end object
    if(b_level_end.check(b_char.col_rect) && w_level_end.check(w_char.col_rect))
    {
        b_char.center(&b_level_end.col_rect);
        w_char.center(&w_level_end.col_rect);
        
        // change state to level 2
        change_state(game, new level01_state);
    }
    
    //if black cross spring is activated
    if(b_cross_spring.check(w_char.col_rect) && b_cross_spring.check(b_char.col_rect) && b_cross_spring.show)
    {
        
        b_cross_spring.cross_spring(&w_char, &b_char, LOCATION);
        // activate
    }
    else
    {
        b_cross_spring.activated = false;
        
        while(b_cross_spring.status != BOARD_INACTIVE)
        {
            b_cross_spring.status = BOARD_INACTIVE;
        }
    }
    
    //if black button is activated
    if(b_button.check(b_char.col_rect))
    {
        // used
        b_button.used = true;
        
        // activate
        b_button.activated = true;
        
        // animate
        if(b_button.status == BUTT_INACTIVE)
        {
            b_button.status = (b_button.status + 1) % 4;
        }
        
        // initialize black level end
        b_level_end.tex = b_end_tex;
        b_level_end.col_rect.x = 1500;
        b_level_end.col_rect.y = 480;
        
        // initialize black level end
        w_level_end.tex = w_end_tex;
        w_level_end.col_rect.x = 1500;
        w_level_end.col_rect.y = 540;
        
        
    }
    else
    {
        b_button.activated = false;
        
        if(b_button.status != BUTT_INACTIVE)
        {
            b_button.status = (b_button.status + 1) % 4;
        }
    }
    
    //if white button is activated
    if(w_button.check(w_char.col_rect))
    {
        // used
        w_button.used = true;
        
        // activate
        w_button.activated = true;
        
        if(w_button.status == BUTT_INACTIVE)
        {
            w_button.status = (w_button.status + 1) % 4;
        }
    }
    else
    {
        w_button.activated = false;
        
        if(w_button.status == BUTT_ACTIVE)
        {
            w_button.status = (w_button.status + 1) % 4;
        }
        //if(w_button.status != BUTT_INACTIVE)
        {
            //w_button.status = (w_button.status + 1) % 4;
        }
    }
    
    //if black springboard is activated
    if(b_springboard.check(b_char.col_rect) && b_springboard.show)
    {
        if(b_char.col_rect.y < b_springboard.col_rect.y + 20)
        {
            // activate
            b_springboard.activated = true;
            
            if(b_springboard.status == BOARD_INACTIVE)
            {
                b_springboard.status = (b_springboard.status + 1) % 4;
            }
            
            b_char.spring(b_springboard.x_spring, b_springboard.y_spring, b_springboard.direction);
        }
    }
    else
    {
        b_springboard.activated = false;
        
        if(b_springboard.status != BOARD_INACTIVE)
        {
            b_springboard.status = (b_springboard.status + 1) % 4;
        }
    }
    
    //if white springboard is activated
    if(w_springboard.check(w_char.col_rect) && w_springboard.show)
    {
        // activate
        w_springboard.activated = true;
        
        if(w_springboard.status == BOARD_INACTIVE)
        {
            w_springboard.status = (w_springboard.status + 1) % 4;
        }
        
        w_char.spring(w_springboard.x_spring, w_springboard.y_spring, w_springboard.direction);
        
    }
    else
    {
        w_springboard.activated = false;
        
        if(w_springboard.status != BOARD_INACTIVE)
        {
            w_springboard.status = (w_springboard.status + 1) % 4;
        }
    }

}