
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// include headers
#include <states/level4_state.hpp>
#include <states/level6_state.hpp>
#include <states/level5_state.hpp>
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

void level5_state::init(engine* game)
{
    // load textures
    load_textures(game);

    // initialize objects
    init_objects(game);

    if (game->read_save() < 5)
    {
        game->save(5);
    }
}

void level5_state::handle_events(engine *game)
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
        // quit if he pressed escape
        w_char.handle_event(event, this, game);


    }

    shiftable = true;
}

void level5_state::update(engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    // move the square
    if (b_char.status == CHAR_ACTIVE)
        b_char.move(this, game);
    if (w_char.status == CHAR_ACTIVE)
        w_char.move(this, game);

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

void level5_state::draw(engine* game)
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
    w_level_end.render(&camera->display, game);
    w_char.render(&camera->display, game);

    SDL_RenderPresent(game->rend);
}

void level5_state::cleanup()
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
    w_springboard_tex.free();
    b_cross_spring_tex.free();
    w_cross_spring_tex.free();

}

void level5_state::pause()
{
    return;
}

void level5_state::resume()
{
    return;
}

void level5_state::load_textures(engine* game)
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


    // initialize level
    width = 29;
    height = 21;

    path = "resources/level-files/level_05.csv";

    if (!set_tiles(tileset, path, width, height))
    {
        printf("Failed to load level 5 map!\n");
        return;
    }
}

void level5_state::init_objects(engine* game)
{
    // initialize black dot
    b_char.status = CHAR_ACTIVE;
    b_char.tex = b_char_tex;
    b_char.col_rect.x = 2 * TILE_WIDTH;
    b_char.col_rect.y = 8 * TILE_WIDTH;
    b_char.black = true;


    camera = new class camera(game->screen_width, game->screen_height);

    // initialize black level end
    b_level_end.tex = b_end_tex;
    b_level_end.col_rect.x = 1600;
    b_level_end.col_rect.y = 8 * TILE_WIDTH;

    // initialize white dot
    w_char.status = CHAR_INACTIVE;
    w_char.tex = w_char_tex;
    w_char.col_rect.x = 2 * TILE_WIDTH;
    w_char.col_rect.y = 9 * TILE_WIDTH;
    w_char.black = false;

    // initialize white level end
    w_level_end.tex = w_end_tex;
    w_level_end.col_rect.x = 1600;
    w_level_end.col_rect.y = 9 * TILE_WIDTH;



}

void level5_state::interactions(engine* game)
{

    // if both are on level end object
    if(b_level_end.check(b_char.col_rect) && w_level_end.check(w_char.col_rect))

    {
        // change state to level 5
        change_state(game, new level6_state);
    }
}