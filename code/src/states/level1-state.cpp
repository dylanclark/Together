
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// include headers
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/levelstate.hpp>
#include <states/mainmenu_state.hpp>
#include <states/pausemenu_state.hpp>
#include <char.hpp>
#include <levels.hpp>
#include <engine.hpp>
#include <objects.hpp>
#include <menu.hpp>

void Level1State::init(Engine* game)
{
    // load textures
    load_tiles(game);

    // initialize objects
    init_objects(game);

    if (game->read_save() < 1) {
        game->save(1);
    }
}

void Level1State::handle_events(Engine* game)
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
    }

    shiftable = true;
}

void Level1State::update(Engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    // move the square
    if (b_char->status == CHAR_ACTIVE) {
        b_char->move(this, game);
    }

    for (int i = 0; i < crates.size(); i++) {
        crates[i]->update();
    }

    camera->update(b_char->get_rect(), b_char->get_rect());

    interactions(game);
}

void Level1State::draw(Engine* game)
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
    SDL_RenderPresent(game->rend);
}

void Level1State::cleanup()
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
    delete camera;
    delete b_level_end;
    tile_tex.free();
}

void Level1State::load_tiles(Engine* game)
{
    if (!tile_tex.load_tile_sheet("tile_sheet.png", game->rend)) {
        printf("Failed to load tile sheet texture!\n");
        return;
    }
    if (!set_tiles(this, tileset, "level01.lvl")) {
        printf("Failed to load level 1 map!\n");
        return;
    }
}

void Level1State::init_objects(Engine* game)
{
    b_char = new class Dot(2, 7, true, game->rend);
    camera = new class Camera(game->screen_width, game->screen_height,
                              width * TILE_WIDTH, height * TILE_WIDTH,
                              b_char->get_rect(), b_char->get_rect());
    b_level_end = new class LevelEnd(23, 12, true, game->rend);
}

void Level1State::interactions(Engine* game)
{
    // if both are on level end object
    if(b_level_end->check(b_char->get_rect())) {
        change_state(game, new Level2State);
    }

}