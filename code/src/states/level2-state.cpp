
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
#include <engine.hpp>
#include <objects.hpp>
#include <states/pausemenu-state.hpp>

void Level2State::init(Engine* game)
{
    load_tiles(game, "02");
    init_objects(game);

    if (game->read_save() < 2) {
        game->save(2);
    }
}

void Level2State::update(Engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    // move the square
    b_char->move(this, game);

    for (int i = 0; i < crates.size(); i++)
    {
        crates[i]->update();
    }

    interactions(game);

    camera->update(b_char->get_rect(), b_char->get_rect());
}

void Level2State::draw(Engine* game)
{
    SDL_Rect* cam_rect = camera->get_display();

    // draw stuff to the screen!
    for (int i = 0; i < (width * height); i++)
    {
        tileset[i]->render(status, cam_rect, game, &tile_tex);
    }

    for (int i = 0; i < crates.size(); i++)
    {
        crates[i]->render(status, cam_rect, game, this);
    }

    b_char->render(cam_rect, game);
    b_level_end->render(cam_rect, game);

    SDL_RenderPresent(game->rend);
}

void Level2State::cleanup()
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

void Level2State::init_objects(Engine* game)
{
    // initialize black dot
    b_char = new class Dot(2, 11, true, game->rend);
    camera = new class Camera(game->screen_width, game->screen_height,
                              width * TILE_WIDTH, height * TILE_WIDTH,
                              b_char->get_rect(), b_char->get_rect());

    b_level_end = new class LevelEnd(25, 11, true, game->rend);
}

void Level2State::interactions(Engine* game)
{
    // if both are on level end object
    if (b_level_end->check(b_char->get_rect())) {
        change_state(game, new Level3State);
    }
}