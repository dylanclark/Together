
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
    printf("created char\n");

    if (game->read_save() < 1) {
        game->save(1);
    }
}

void Level1State::update(Engine* game)
{
    printf("new update\n");
    // clear the window
    SDL_RenderClear(game->rend);

    for (int i = 0; i < chars.size(); i++) {
        printf("about to move char %d\n", i);
        chars[i]->move(this, game);
        printf("moved!\n");
    }

    for (int i = 0; i < crates.size(); i++) {
        crates[i]->update();
    }
    printf("about to check size...\n");
    if (chars.size() == 1) {
        printf("about to update camera\n");
        camera->update(chars[0]->get_rect(), chars[0]->get_rect());
        printf("success!\n");
    } else {
        camera->update(chars[0]->get_rect(), chars[1]->get_rect());
    }
    interactions(game);
}

void Level1State::draw(Engine* game)
{
    printf("draw begin\n");
    SDL_Rect* cam_rect = camera->get_display();
    printf("got cam\n");

    // draw stuff to the screen!
    for (int i = 0; i < (width * height); i++) {
        tileset[i]->render(status, cam_rect, game, &tile_tex);
    }
    printf("drew tiles\n");

    for (int i = 0; i < crates.size(); i++) {
        crates[i]->render(status, cam_rect, game, this);
    }

    printf("about to check size in draw...\n");
    for (int i = 0; i < chars.size(); i++) {
        printf("about to draw char\n");
        chars[i]->render(cam_rect, game);
        printf("nailed it!\n");
    }

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
    delete camera;
    delete b_level_end;
    tile_tex.free();
}

void Level1State::init_objects(Engine* game)
{
    printf("check");
    chars.push_back(new Dot(2, 3, true, game->rend, &palette));
    camera = new class Camera(game->screen_width, game->screen_height,
                              width * TILE_WIDTH, height * TILE_WIDTH,
                              chars[0]->get_rect(), chars[0]->get_rect());
    b_level_end = new class LevelEnd(23, 12, true, game->rend);
}

void Level1State::interactions(Engine* game)
{
    printf("interactions begin\n");
    // if both are on level end object
    if(b_level_end->check(chars[0]->get_rect())) {
        change_state(game, new Level2State);
    }
    printf("interactions end\n");

}
