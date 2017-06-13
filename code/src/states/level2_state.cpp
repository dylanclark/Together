
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// include headers
<<<<<<< HEAD
<<<<<<< HEAD:together/level2_state.cpp
#include "level2_state.hpp"
#include "level3_state.hpp"
#include "mainmenu_state.hpp"
#include "characters.hpp"
#include "tiles.hpp"
#include "crate.hpp"
#include "level_end.hpp"
#include "gamestate.hpp"
#include "button.hpp"
#include "engine.hpp"
#include "springboard.hpp"
<<<<<<< HEAD
<<<<<<< HEAD
#include "pausemenu_state.hpp"
=======
=======
#include "pausemenu_state.hpp"
>>>>>>> b773edd... for the pull
#include "level_messages.hpp"
<<<<<<< HEAD
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
=======
#include "pausemenu_state.hpp"
>>>>>>> 0c27592... updated menu systems
=======
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
#include <states/level2_state.hpp>
#include <states/level3_state.hpp>
#include <states/mainmenu_state.hpp>
#include <characters/characters.hpp>
#include <levels/tiles.hpp>
#include <objects/crate.hpp>
#include <levels/level_end.hpp>
#include <engine/gamestate.hpp>
#include <objects/button.hpp>
#include <engine/engine.hpp>
#include <objects/springboard.hpp>
#include <states/pausemenu_state.hpp>
#include <levels/level_messages.hpp>
#include <states/pausemenu_state.hpp>
<<<<<<< HEAD
>>>>>>> 201bf90... gigantic makefile revamp:code/src/states/level2_state.cpp
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca

void level2_state::init(engine* game)
{
    // load textures
    load_textures(game);

    // initialize objects
    init_objects(game);

    if (game->read_save() < 2)
    {
        game->save(2);
    }
}

void level2_state::handle_events(engine *game)
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
<<<<<<< HEAD
<<<<<<< HEAD
        if (!b_char.handle_event(event))
            game->quit();
        
        if (!w_char.handle_event(event))
            game->quit();
=======
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
        if (!b_char.handle_event(event, this, game))
        {
            Mix_PauseMusic();
            Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
            game->push_state(new pausemenu_state);
        }
<<<<<<< HEAD
<<<<<<< HEAD:together/level2_state.cpp
        
<<<<<<< HEAD
        if (!w_char.handle_event(event, this, game))
            game->change_state(new mainmenu_state);
>>>>>>> a671362... Implemented main menu, pause menu, and options menu!
=======
        w_char.handle_event(event, this, game);
>>>>>>> f939236... New level files and updated menu system.
        
=======

        w_char.handle_event(event, this, game);

>>>>>>> 201bf90... gigantic makefile revamp:code/src/states/level2_state.cpp
=======

        w_char.handle_event(event, this, game);

>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
    }

    shiftable = true;
}

void level2_state::update(engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    // move the square
<<<<<<< HEAD
<<<<<<< HEAD
    b_char.move(tileset, width, height);
    w_char.move(tileset, width, height);
=======
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
    if (b_char.status == CHAR_ACTIVE)
        b_char.move(this, game);
    if (w_char.status == CHAR_ACTIVE)
        w_char.move(this, game);
<<<<<<< HEAD
<<<<<<< HEAD:together/level2_state.cpp
>>>>>>> b57fd21... stuff
    
=======

>>>>>>> 201bf90... gigantic makefile revamp:code/src/states/level2_state.cpp
=======

>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
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

void level2_state::draw(engine* game)
{
    // draw stuff to the screen!
    for (int i = 0; i < (width * height); i++)
    {
        tileset[i]->render(b_char.status, &camera->display, game, &tile_tex);
    }
<<<<<<< HEAD
<<<<<<< HEAD:together/level2_state.cpp
    
<<<<<<< HEAD
<<<<<<< HEAD
    for (int i = 0; i < crates.size(); i++)
    {
        crates[i]->render(b_char.status, &camera.display, game->rend, this);
    }
    
    b_char.render(&camera.display, game->rend);
    w_char.render(&camera.display, game->rend);
    b_level_end.render(&camera.display, game->rend);
    w_level_end.render(&camera.display, game->rend);
    b_button.render(&camera.display, game->rend);
    w_button.render(&camera.display, game->rend);
    b_springboard.render(&camera.display, game->rend);
=======
=======
=======

>>>>>>> 201bf90... gigantic makefile revamp:code/src/states/level2_state.cpp
=======

>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
    for (int i = 0; i < crates.size(); i++)
    {
        crates[i]->render(b_char.status, &camera->display, game, this);
    }
<<<<<<< HEAD
<<<<<<< HEAD:together/level2_state.cpp
    
<<<<<<< HEAD
>>>>>>> 71027e4... Volume / sfx slider adjustment.
    b_char.render(&camera->display, game->rend);
    w_char.render(&camera->display, game->rend);
    b_level_end.render(&camera->display, game->rend);
    w_level_end.render(&camera->display, game->rend);
<<<<<<< HEAD
>>>>>>> a671362... Implemented main menu, pause menu, and options menu!
=======
    b_button.render(&camera->display, game->rend);
    w_button.render(&camera->display, game->rend);
    b_springboard.render(&camera->display, game->rend);
    w_springboard.render(&camera->display, game->rend);
<<<<<<< HEAD
>>>>>>> 71027e4... Volume / sfx slider adjustment.
=======
    b_cross_spring.render(&camera->display, game->rend);
=======
=======

>>>>>>> 201bf90... gigantic makefile revamp:code/src/states/level2_state.cpp
    b_char.render(&camera->display, game);
    b_level_end.render(&camera->display, game);
<<<<<<< HEAD
    w_level_end.render(&camera->display, game);
    b_button.render(&camera->display, game);
    w_button.render(&camera->display, game);
    b_springboard.render(&camera->display, game);
    w_springboard.render(&camera->display, game);
    b_cross_spring.render(&camera->display, game);
>>>>>>> 4ff27ea... Finished dynamic camera! (finally)
    // level2_end.render(&camera->display, game->rend);

=======
>>>>>>> 74e2e9b... levels 1 and 2 now complete!!1

>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
=======

    b_char.render(&camera->display, game);
    b_level_end.render(&camera->display, game);

>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
    SDL_RenderPresent(game->rend);
}

void level2_state::cleanup()
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
    b_button_tex.free();
    w_button_tex.free();
    b_springboard_tex.free();
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
    w_springboard_tex.free();
    b_cross_spring_tex.free();
    w_cross_spring_tex.free();
    level2_start_tex.free();
    level2_end_tex.free();
<<<<<<< HEAD
<<<<<<< HEAD:together/level2_state.cpp
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
    
=======

>>>>>>> 201bf90... gigantic makefile revamp:code/src/states/level2_state.cpp
=======

>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
}

void level2_state::pause()
{
    return;
}

void level2_state::resume()
{
    return;
}

void level2_state::load_textures(engine* game)
{
    // LOAD ALL TEXTURES
    if (!b_char_tex.load_tile_sheet("resources/textures/black/b_char.png", game->rend))
    {
        printf("Failed to load black dot texture!\n");
        return;
    }
    if (!w_char_tex.load_tile_sheet("resources/textures/white/w_char.png", game->rend))
    {
        printf("Failed to load white dot texture!\n");
        return;
    }
    if (!tile_tex.load_tile_sheet("resources/textures/tile_sheet.png", game->rend))
    {
        printf("Failed to load tile sheet texture!\n");
        return;
    }
    if (!b_end_tex.load_tile_sheet("resources/textures/black/level_end/black_end.png", game->rend))
    {
        printf("Failed to load black level end texter!\n");
        return;
    }
    if (!crate_tex_four_by_two.load_object(TILE_WIDTH * 4, TILE_WIDTH * 2, "resources/textures/black/crates/b_crate.png", game->rend))
    {
        printf("Failed to load crate (4x2) texture!\n");
        return;
    }
    if (!w_end_tex.load_tile_sheet("resources/textures/white/level_end/white_end.png", game->rend))
    {
        printf("Failed to load  white level end texture!\n");
        return;
    }
<<<<<<< HEAD
<<<<<<< HEAD
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
    if(!w_springboard_tex.load_tile_sheet("textures/white/spring/w_spring.png", game->rend))
    {
        printf("Failed to load white springboard texture!\n");
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
    //if (!level2_end_tex.load_message(128, 16, "textures/levels/level_2_done.png", game->rend))
    {
     //   printf("Failed to load level 2 end texture!\n");
     //   return;
    }
=======
>>>>>>> 74e2e9b... levels 1 and 2 now complete!!1

<<<<<<< HEAD:together/level2_state.cpp
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
    
=======

>>>>>>> 201bf90... gigantic makefile revamp:code/src/states/level2_state.cpp
=======


>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
    // initialize level
    width = 27;
    height = 18;

    path = "resources/level-files/level_02.csv";

    if (!set_tiles(tileset, path, width, height))
    {
        printf("Failed to load level 2 map!\n");
        return;
    }
}

void level2_state::init_objects(engine* game)
{
    // initialize black dot
    b_char.status = CHAR_ACTIVE;
    b_char.tex = b_char_tex;
    b_char.col_rect.x = 2 * TILE_WIDTH;
    b_char.col_rect.y = 8 * TILE_WIDTH;
    b_char.black = true;

    camera = new class camera(game->screen_width, game->screen_height);
<<<<<<< HEAD
<<<<<<< HEAD:together/level2_state.cpp
    
<<<<<<< HEAD
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
<<<<<<< HEAD
=======
    w_button.direction = UP;
    w_button.status = BUTT_INACTIVE;
    
    // initialize black springboard
    b_springboard.tex = b_springboard_tex;
    b_springboard.col_rect.x = 920;
    b_springboard.col_rect.y = 480;
    b_springboard.show = true;
    b_springboard.direction = FLIP_RIGHT;
    b_springboard.x_spring = 0;
    b_springboard.y_spring = 10;
    
    // initialize white springboard
    w_springboard.tex = w_springboard_tex;
    w_springboard.col_rect.x = 920;
    w_springboard.col_rect.y = 540;
    w_springboard.show = true;
    w_springboard.direction = FLIP_RIGHT;
    w_springboard.x_spring = 0;
    w_springboard.y_spring = 10;
    
    //level2_end.tex = level2_end_tex;
    //level2_end.send_message(END, game);
    
    camera = new class camera(game->screen_width, game->screen_height);
    

>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
=======
=======

>>>>>>> 201bf90... gigantic makefile revamp:code/src/states/level2_state.cpp
=======

>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
    // initialize black level end
    b_level_end.tex = b_end_tex;
    b_level_end.col_rect.x = 1500;
    b_level_end.col_rect.y = 11 * TILE_WIDTH;



<<<<<<< HEAD
>>>>>>> 74e2e9b... levels 1 and 2 now complete!!1
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
}

void level2_state::interactions(engine* game)
{
<<<<<<< HEAD
<<<<<<< HEAD:together/level2_state.cpp
=======

>>>>>>> 201bf90... gigantic makefile revamp:code/src/states/level2_state.cpp
    // if both are on level end object
    if(b_level_end.check(b_char.col_rect))
    {
<<<<<<< HEAD
        // do end animation
        b_char.completed(width, height, &b_level_end.col_rect);
        w_char.completed(width, height, &w_level_end.col_rect);
        
        
=======
        b_char.center(&b_level_end.col_rect);
<<<<<<< HEAD:together/level2_state.cpp
<<<<<<< HEAD
        w_char.center(&w_level_end.col_rect);
    
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
=======
        
>>>>>>> d0f13a4... leves 3 and 4!
        // change state to level 3
        change_state(game, new level3_state);
    }
    
<<<<<<< HEAD
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
<<<<<<< HEAD
    if(w_button.check(w_char.col_rect) && !(w_button.single && w_button.used))
=======
    if(w_button.check(w_char.col_rect))
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
    {
        // used
        w_button.used = true;
        
        // activate
        w_button.activated = true;
        
        if(w_button.status == BUTT_INACTIVE)
        {
            w_button.status = (w_button.status + 1) % 4;
        }
        
        // initialize black springboard
        b_springboard.tex = b_springboard_tex;
        b_springboard.col_rect.x = 920;
        b_springboard.col_rect.y = 480;
        b_springboard.direction = RIGHT;
        
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
    if(b_springboard.check(b_char.col_rect))
    {
        if(b_char.col_rect.y < b_springboard.col_rect.y + 20)
        {
            // activate
        b_springboard.activated = true;
        
        if(b_springboard.status == BOARD_INACTIVE)
        {
            b_springboard.status = (b_springboard.status + 1) % 4;
        }
        
<<<<<<< HEAD
        b_char.spring();
        
        
        // initialize black level end
        b_level_end.tex = b_end_tex;
        b_level_end.col_rect.x = 1500;
        b_level_end.col_rect.y = 480;
        
        // initialize black level end
        w_level_end.tex = w_end_tex;
        w_level_end.col_rect.x = 1500;
        w_level_end.col_rect.y = 540;
        
=======
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
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
        
        
    }
    else
    {
        b_springboard.activated = false;
        
<<<<<<< HEAD
        if(b_springboard.status != BUTT_INACTIVE)
=======
        if(w_springboard.status != BOARD_INACTIVE)
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
        {
            b_springboard.status = (b_springboard.status + 1) % 4;
        }
    }
=======
>>>>>>> 74e2e9b... levels 1 and 2 now complete!!1
=======
=======

    // if both are on level end object
    if(b_level_end.check(b_char.col_rect))
    {
        b_char.center(&b_level_end.col_rect);
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca

        // change state to level 3
        change_state(game, new level3_state);
    }

<<<<<<< HEAD
>>>>>>> 201bf90... gigantic makefile revamp:code/src/states/level2_state.cpp
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
}