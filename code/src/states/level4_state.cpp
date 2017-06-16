
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// include headers
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/mainmenu_state.hpp>
#include <char.hpp>
#include <levels.hpp>
#include <objects.hpp>
#include <engine.hpp>
#include <states/pausemenu_state.hpp>

void Level4State::init(Engine* game)
{
    // load textures
    load_textures(game);

    b_char = NULL;
    w_char = NULL;

    // initialize objects
    init_objects(game);

    if (game->read_save() < 4)
    {
        game->save(4);
    }
}

void Level4State::handle_events(Engine* game)
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
        if (!b_char->handle_event(event, this, game))
        {
            Mix_PauseMusic();
            Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
            game->push_state(new PauseMenuState);
        }
        if (w_char != NULL) {
            w_char->handle_event(event, this, game);
        }
    }

    shiftable = true;
}

void Level4State::update(Engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    // move the square
    if (b_char->status == CHAR_ACTIVE)
        b_char->move(this, game);
    if (w_char != NULL) {
        if (w_char->status == CHAR_ACTIVE) {
            w_char->move(this, game);
        }
    }

    for (int i = 0; i < crates.size(); i++)
    {
        crates[i]->update();
    }

    // if white is active
    if(w_char != NULL) {
        camera->update(&b_char->col_rect, &w_char->col_rect);
    }
    else {
        camera->update(&b_char->col_rect, &b_char->col_rect);
    }

    interactions(game);
}

void Level4State::draw(Engine* game)
{
    SDL_Rect* cam_rect = camera->get_display();

    // draw stuff to the screen!
    for (int i = 0; i < (width * height); i++)
    {
        tileset[i]->render(b_char->status, cam_rect, game, &tile_tex);
    }

    for (int i = 0; i < crates.size(); i++)
    {
        crates[i]->render(b_char->status, cam_rect, game, this);
    }

    b_char->render(cam_rect, game);
    b_level_end.render(cam_rect, game);
    w_level_end.render(cam_rect, game);
    if (w_char != NULL) {
        w_char->render(cam_rect, game);
    }

    SDL_RenderPresent(game->rend);
}

void Level4State::cleanup()
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

void Level4State::load_textures(Engine* game)
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
    width = 32;
    height = 20;

    path = "resources/level-files/level_04.csv";

    if (!set_tiles(tileset, path, width, height))
    {
        printf("Failed to load level 4 map!\n");
        return;
    }
}

void Level4State::init_objects(Engine* game)
{
    // initialize black dot
    b_char = new class Dot(2, 8, true, &b_char_tex);
    camera = new class Camera(game->screen_width, game->screen_height, width * TILE_WIDTH, height * TILE_WIDTH);

    // initialize black level end
    b_level_end.tex = b_end_tex;
    b_level_end.col_rect.x = 920;
    b_level_end.col_rect.y = 8 * TILE_WIDTH;

}

void Level4State::interactions(Engine* game)
{
    // if both are on level end object (making sure it hasnt been activate
    // to change level ends already
    if(b_level_end.check(b_char->col_rect) && (w_level_end.col_rect.x != 1700))
    {
        //b_char->center(&b_level_end.col_rect);

        // initialize white dot
        w_char = new class Dot(15, 9, false, &w_char_tex);

        b_level_end.col_rect.x = 1700;
        b_level_end.col_rect.y = 13 * TILE_WIDTH;

        // initialize white level end
        w_level_end.tex = w_end_tex;
        w_level_end.col_rect.x = 1700;
        w_level_end.col_rect.y = 14 * TILE_WIDTH;
    }

    if (w_char != NULL) {
        if(b_level_end.check(b_char->col_rect) && w_level_end.check(w_char->col_rect)) {
            change_state(game, new Level5State);
        }
    }
}