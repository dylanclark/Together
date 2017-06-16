
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// include headers
#include <states/levelstate.hpp>
#include <states/mainmenu_state.hpp>
#include <char.hpp>
#include <objects.hpp>
#include <levels.hpp>
#include <engine.hpp>
#include <states/pausemenu_state.hpp>

void Level7State::init(Engine* game)
{
    // load textures
    load_textures(game);

    // initialize objects
    init_objects(game);

    if (game->read_save() < 7)
    {
        game->save(7);
    }
}

void Level7State::handle_events(Engine* game)
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
        // quit if he pressed escape
        w_char->handle_event(event, this, game);


    }

    shiftable = true;
}

void Level7State::update(Engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    // move the square
    if (b_char->status == CHAR_ACTIVE)
        b_char->move(this, game);
    if (w_char->status == CHAR_ACTIVE)
        w_char->move(this, game);

    for (int i = 0; i < crates.size(); i++)
    {
        crates[i]->update();
    }

    camera->update(&b_char->col_rect, &w_char->col_rect, width, height, game);

    interactions(game);
}

void Level7State::draw(Engine* game)
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
    w_char->render(cam_rect, game);
    b_button.render(cam_rect, game);
    w_button.render(cam_rect, game);
    SDL_RenderPresent(game->rend);
}

void Level7State::cleanup()
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

void Level7State::load_textures(Engine* game)
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
    if (!w_crate_tex_four_by_two.load_object(TILE_WIDTH * 4, TILE_WIDTH * 2, "resources/textures/white/crates/w_crate.png", game->rend))
    {
        printf("Failed to load white crate (4x2) texture!\n");
        return;
    }
    if (!w_end_tex.load_tile_sheet("resources/textures/white/level_end/white_end.png", game->rend))
    {
        printf("Failed to load  white level end texture!\n");
        return;
    }
    if (!b_button_tex.load_tile_sheet("resources/textures/black/button/b_button.png", game-> rend))
    {
        printf("Failed to load  black button texture!\n");
        return;
    }
    if (!w_button_tex.load_tile_sheet("resources/textures/white/button/w_button.png", game-> rend))
    {
        printf("Failed to load white button texture!\n");
        return;
    }

    // initialize level
    width = 26;
    height = 28;

    path = "resources/level-files/level_07.csv";

    if (!set_tiles(tileset, path, width, height))
    {
        printf("Failed to load level 7 map!\n");
        return;
    }
}

void Level7State::init_objects(Engine* game)
{
    b_char = new class Dot(2, 9, true, &b_char_tex);
    w_char = new class Dot(2, 12, false, &w_char_tex);

    camera = new class Camera(game->screen_width, game->screen_height);

    // initialize black level end
    b_level_end.tex = b_end_tex;
    b_level_end.col_rect.x = 1300;
    b_level_end.col_rect.y = 20 * TILE_WIDTH;


    // initialize white level end
    w_level_end.tex = w_end_tex;
    w_level_end.col_rect.x = 1300;
    w_level_end.col_rect.y = 21 * TILE_WIDTH;


    // initialize black button
    b_button.tex = b_button_tex;
    b_button.col_rect.x = 1250;
    b_button.col_rect.y = 5 * TILE_WIDTH;
    b_button.single = true;
    b_button.direction = UP;

    // initialize black button
    w_button.tex = w_button_tex;
    w_button.col_rect.x = 900;
    w_button.col_rect.y = 17.5 * TILE_WIDTH;
    w_button.single = true;
    w_button.direction = LEFT;

}

void Level7State::interactions(Engine* game)
{

    // if both are on level end object
    if(b_level_end.check(b_char->col_rect) && w_level_end.check(w_char->col_rect))

    {
        // change state to level 8
        change_state(game, new Level8State);
    }


    //if black button is activated
    if(b_button.check(b_char->col_rect) && b_button.used == false)
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

        // init crate #1
        crates.push_back(new Crate(5 * TILE_WIDTH, 12 * TILE_WIDTH, FOUR_BY_TWO));
        crates.back()->tex = w_crate_tex_four_by_two;
        crates.back()->black = false;

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
    if(w_button.check(w_char->col_rect) && w_button.used == false)
    {
        // used
        w_button.used = true;

        // activate
        w_button.activated = true;

        // animate
        if(w_button.status == BUTT_INACTIVE)
        {
            w_button.status = (w_button.status + 1) % 4;
        }

        // init crate #2
        crates.push_back(new Crate(5 * TILE_WIDTH, 17 * TILE_WIDTH, FOUR_BY_TWO));
        crates.back()->tex = w_crate_tex_four_by_two;
        crates.back()->black = false;

    }
    else
    {
        w_button.activated = false;

        if(w_button.status != BUTT_INACTIVE)
        {
            w_button.status = (w_button.status + 1) % 4;
        }
    }


}