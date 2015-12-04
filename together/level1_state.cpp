
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

// include headers
#include "level1_state.hpp"
#include "level01_state.hpp"
#include "level2_state.hpp"
#include "mainmenu_state.hpp"
#include "pausemenu_state.hpp"
#include "characters.hpp"
#include "tiles.hpp"
<<<<<<< HEAD
#include "level_end.hpp"
#include "gamestate.hpp"
#include "button.hpp"
#include "engine.hpp"
#include "springboard.hpp"
#include "menu.hpp"
#include "level_messages.hpp"

level1_state level1_state::level1_state_inst;
=======
#include "crate.hpp"
>>>>>>> 2e8ee63... Implemented movable crates!!!

void level1_state::init(engine* game)
{
<<<<<<< HEAD
<<<<<<< HEAD
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
<<<<<<< HEAD
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
=======
    if (!crate_tex_four_by_two.load_object(TILE_WIDTH * 4, TILE_WIDTH * 2, "textures/black/crates/b_crate.png", game->rend))
    {
        printf("Failed to load crate (4x2) texture!\n");
>>>>>>> 2e8ee63... Implemented movable crates!!!
        return;
    }
    if(!b_springboard_tex.load_tile_sheet("textures/black/spring/b_spring.png", game->rend))
    {
        printf("Failed to load black springboard texture!\n");
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
    
    camera = new class camera(game->screen_width, game->screen_height);
    
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
    
<<<<<<< HEAD
    // initialize black level end
    b_level_end.tex = b_end_tex;
    b_level_end.col_rect.x = 1000;
    b_level_end.col_rect.y = 360;
    
    // initialize black level end
    w_level_end.tex = w_end_tex;
    w_level_end.col_rect.x = 1000;
    w_level_end.col_rect.y = 420;
    
    // initialize black button
    b_button.tex = b_button_tex;
    b_button.col_rect.x = 960;
    b_button.col_rect.y = 480;
    b_button.single = true;
    b_button.direction = LEFT;
    
    // initialize white button
<<<<<<< HEAD
    //w_button.tex = w_button_tex;
    //w_button.col_rect.x = 1300;
    //w_button.col_rect.y = 420;
    //b_button.activated = false;
    //b_button.single = false;
   // b_button.black = false;
=======
    // init crate #1
    crates.push_back(new crate(5 * TILE_WIDTH, 7  * TILE_WIDTH, FOUR_BY_TWO));
    crates.back()->tex = crate_tex_four_by_two;
    crates.back()->black = true;
    
>>>>>>> 2e8ee63... Implemented movable crates!!!
=======
    w_button.tex = w_button_tex;
    w_button.col_rect.x = 200;
    w_button.col_rect.y = 540;
<<<<<<< HEAD
    w_button.activated = false;
    w_button.single = false;
    w_button.used = false;
    w_button.status = 0;
>>>>>>> 1aa9403... buttons!
=======
    
=======
=======
>>>>>>> 71027e4... Volume / sfx slider adjustment.
    // load textures
    load_textures(game);
    
    // initialize objects
    init_objects(game);
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> ffa65bf... add springs and abstract level loading, put in some SDL_ttf headers
=======
>>>>>>> 71027e4... Volume / sfx slider adjustment.

>>>>>>> c8cecea... springs!
=======
    
<<<<<<< HEAD
    if (game->read_save() < 1)
    {
        game->save(1);
    }
>>>>>>> f939236... New level files and updated menu system.
=======
>>>>>>> 74e2e9b... levels 1 and 2 now complete!!1
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
            Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
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
    
<<<<<<< HEAD
    b_char.render(&camera->display, game->rend);
    w_char.render(&camera->display, game->rend);
    b_level_end.render(&camera->display, game->rend);
    w_level_end.render(&camera->display, game->rend);
<<<<<<< HEAD
<<<<<<< HEAD
    //b_button.render(&camera.display, game->rend);
    //w_button.render(&camera.display, game->rend);
    //b_springboard.render(&camera.display, game->rend);
=======
>>>>>>> 71027e4... Volume / sfx slider adjustment.
=======
    b_cross_spring.render(&camera->display, game->rend);
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
=======
    b_char.render(&camera->display, game);
    b_level_end.render(&camera->display, game);
<<<<<<< HEAD
    w_level_end.render(&camera->display, game);
    b_cross_spring.render(&camera->display, game);
>>>>>>> 4ff27ea... Finished dynamic camera! (finally)
=======
>>>>>>> 74e2e9b... levels 1 and 2 now complete!!1
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
<<<<<<< HEAD
    b_button_tex.free();
    w_button_tex.free();
    b_springboard_tex.free();
<<<<<<< HEAD
=======
    w_springboard_tex.free();
<<<<<<< HEAD
    b_springboard_tex.free();
    w_springboard_tex.free();
>>>>>>> 71027e4... Volume / sfx slider adjustment.
=======
    w_end_animate.free();
    b_end_animate.free();
    b_springboard_tex.free();
    w_springboard_tex.free();
<<<<<<< HEAD
>>>>>>> b25d2b2... woo
=======
    b_cross_spring_tex.free();
    w_cross_spring_tex.free();
    level1_end_tex.free();
    level1_start_tex.free();
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
=======
>>>>>>> 74e2e9b... levels 1 and 2 now complete!!1
    
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
<<<<<<< HEAD
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
<<<<<<< HEAD
=======
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
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
=======
>>>>>>> 74e2e9b... levels 1 and 2 now complete!!1
    
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
    b_level_end.col_rect.x = 1200;
    b_level_end.col_rect.y = 720;
    
    
    
    camera = new class camera(game->screen_width, game->screen_height);
    
<<<<<<< HEAD
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
=======
>>>>>>> 74e2e9b... levels 1 and 2 now complete!!1
}

void level1_state::interactions(engine* game)
{
    // if both are on level end object
<<<<<<< HEAD
    if ((b_level_end.check(b_char.col_rect)) && (w_level_end.check(w_char.col_rect)))
=======
    if(b_level_end.check(b_char.col_rect))
>>>>>>> 74e2e9b... levels 1 and 2 now complete!!1
    {
<<<<<<< HEAD
<<<<<<< HEAD
        // do end animation
        b_char.completed(width, height, &b_level_end.col_rect);
        w_char.completed(width, height, &w_level_end.col_rect);
=======
        b_char.center(&b_level_end.col_rect);
        
=======
>>>>>>> 4754007... Updated level-end detection.
        // change state to level 2
        change_state(game, new level2_state);
    }
    
<<<<<<< HEAD
    // if black cross spring is activated
    if(b_cross_spring.check(w_char.col_rect) && b_cross_spring.check(b_char.col_rect) && b_cross_spring.show)
    {
<<<<<<< HEAD
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
        
=======
>>>>>>> 4754007... Updated level-end detection.
        b_cross_spring.cross_spring(&w_char, &b_char, LOCATION);
        // activate
    }
    else
    {
        b_cross_spring.activated = false;
        
        while (b_cross_spring.status != BOARD_INACTIVE)
        {
            b_cross_spring.status = BOARD_INACTIVE;
        }
    }
=======
>>>>>>> 74e2e9b... levels 1 and 2 now complete!!1
}