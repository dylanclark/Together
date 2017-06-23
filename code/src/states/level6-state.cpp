
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// include headers
#include <states/levelstate.hpp>
#include <states/mainmenu-state.hpp>
#include <char.hpp>
#include <levels.hpp>
#include <objects.hpp>
#include <engine.hpp>
#include <states/pausemenu-state.hpp>

void Level6State::init(Engine* game)
{
    // load textures
    load_tiles(game);

    // initialize objects
    init_objects(game);

    if (game->read_save() < 6)
    {
        game->save(6);
    }
}

void Level6State::handle_events(Engine* game)
{
    // event handler
    SDL_Event event;

    // handle those events, bruh
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game->quit();
                break;
        }

        // quit if he pressed escape
        if (!b_char->handle_event(event, this, game)) {
            Mix_PauseMusic();
            Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
            game->push_state(new PauseMenuState);
        }

        w_char->handle_event(event, this, game);
    }

    shiftable = true;
}

void Level6State::update(Engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    // move the square
    if (b_char->status == CHAR_ACTIVE) {
        b_char->move(this, game);
    }
    if (w_char->status == CHAR_ACTIVE) {
        w_char->move(this, game);
    }

    for (int i = 0; i < crates.size(); i++) {
        crates[i]->update();
    }

    camera->update(b_char->get_rect(), w_char->get_rect());

    interactions(game);
}

void Level6State::draw(Engine* game)
{
    SDL_Rect* cam_rect = camera->get_display();

    // draw stuff to the screen!
    for (int i = 0; i < (width * height); i++) {
        tileset[i]->render(b_char->status, cam_rect, game, &tile_tex);
    }
    for (int i = 0; i < crates.size(); i++) {
        crates[i]->render(b_char->status, cam_rect, game, this);
    }

    b_char->render(cam_rect, game);
    b_level_end->render(cam_rect, game);
    w_level_end->render(cam_rect, game);
    w_char->render(cam_rect, game);
    b_button.render(cam_rect, game);
    SDL_RenderPresent(game->rend);
}

void Level6State::cleanup()
{
    // iterate over all tiles and delete them all
    for (int i = 0; i < width * height; i++) {
        if (tileset[i] != NULL) {
            delete tileset[i];
            tileset[i] = NULL;
        }
    }

    for (int i = 0; i < crates.size(); i++) {
        if (crates[i] != NULL) {
            delete crates[i];
            crates.pop_back();
        }
    }

    delete b_char;
    delete w_char;
    delete camera;
    delete b_level_end;
    delete w_level_end;

    tile_tex.free();
    b_button_tex.free();
}

void Level6State::load_tiles(Engine* game)
{
    if (!tile_tex.load_tile_sheet("tile_sheet.png", game->rend)) {
        printf("Failed to load tile sheet texture!\n");
        return;
    }
    if (!b_button_tex.load_tile_sheet("black/button/b_button.png", game-> rend)) {
        printf("Failed to load  black button texture!\n");
        return;
    }
    if (!set_tiles(this, tileset, "level06.lvl")) {
        printf("Failed to load level 6 map!\n");
        return;
    }
}

void Level6State::init_objects(Engine* game)
{
    b_char = new class Dot(2, 8, true, game->rend);
    w_char = new class Dot(2, 9, false, game->rend);
    camera = new class Camera(game->screen_width, game->screen_height,
                              width * TILE_WIDTH, height * TILE_WIDTH,
                              b_char->get_rect(), w_char->get_rect());

    b_level_end = new class LevelEnd(19, 8, true, game->rend);
    w_level_end = new class LevelEnd(19, 9, false, game->rend);

    // initialize black button
    b_button.tex = b_button_tex;
    b_button.col_rect.x = 800;
    b_button.col_rect.y = 8 * TILE_WIDTH;
    b_button.single = true;
    b_button.direction = UP;
    b_button.used = false;

}

void Level6State::interactions(Engine* game)
{
    if(b_level_end->check(b_char->get_rect()) && w_level_end->check(w_char->get_rect())) {
        change_state(game, new Level7State);
    }


    //if black button is activated
    if(b_button.check(b_char->get_rect()) && b_button.used == false) {
        b_button.used = true;
        b_button.activated = true;

        // animate
        if(b_button.status == BUTT_INACTIVE) {
            b_button.status = (b_button.status + 1) % 4;
        }

        // init crate #1
        crates.push_back(new Crate(5, 7, FOUR_BY_TWO, true, game->rend));

    }
    else {
        b_button.activated = false;
        if(b_button.status != BUTT_INACTIVE) {
            b_button.status = (b_button.status + 1) % 4;
        }
    }
}