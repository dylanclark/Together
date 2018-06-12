
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// include headers
#include <states/levelstate.hpp>
#include <states/mainmenu-state.hpp>
#include <states/pausemenu-state.hpp>
#include <char.hpp>
#include <levels.hpp>
#include <engine.hpp>
#include <objects.hpp>
#include <menu.hpp>

void Level1State::init(Engine* game)
{
    status = 0;
    palette.r = 200;
    palette.g = 100;
    palette.b = 255;

    // load textures
    load_tiles(game, "01");

    // initialize objects
    init_objects(game);

    if (game->read_save() < 1) {
        game->save(1);
    }
}

void Level1State::draw(Engine* game)
{
    SDL_Rect* cam_rect = camera->get_display();

    // draw stuff to the screen!
    for (int i = 0; i < (width * height); i++) {
        tileset[i]->render(status, cam_rect, game, &tile_tex);
    }

    for (int i = 0; i < crates.size(); i++) {
        crates[i]->render(status, cam_rect, game, this);
    }

    for (int i = 0; i < chars.size(); i++) {
        chars[i]->render(cam_rect, game);
    }

    for (int i = 0; i < level_ends.size(); i++) {
        level_ends[i]->render(cam_rect, game);
    }
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

    for (int i = 0; i < chars.size(); i++) {
        if (chars[i] != NULL) {
            delete chars[i];
            chars.pop_back();
        }
    }
    delete camera;
    tile_tex.free();
}

void Level1State::init_objects(Engine* game)
{
    chars.push_back(new Dot(2, 3, true, game->rend, &palette));
    camera = new class Camera(game->screen_width, game->screen_height,
                              width * TILE_WIDTH, height * TILE_WIDTH,
                              chars[0]->get_rect(), chars[0]->get_rect());
    level_ends.push_back(new LevelEnd(5, 5, true, game->rend));
}

void Level1State::interactions(Engine* game)
{
    // if both are on level end object
    for (int i = 0; i < chars.size(); i++) {
        if (!level_ends[i]->check(chars[0]->get_rect())) {
            return;
        }
    }

    change_state(game, new Level2State);
}
