
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// include headers
#include <states/levelstate.hpp>
#include <states/mainmenu-state.hpp>
#include <char.hpp>
#include <objects.hpp>
#include <levels.hpp>
#include <engine.hpp>
#include <states/pausemenu-state.hpp>

void Level9State::init(Engine* game)
{
    // load textures
    load_level(game, "09");

    // initialize objects
    init_objects(game);

    if (game->read_save() < 9)
    {
        game->save(9);
    }
}

void Level9State::update(Engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    // move the square
    b_char->move(this, game);
    w_char->move(this, game);

    for (int i = 0; i < crates.size(); i++) {
        crates[i]->update();
    }

    camera->update(b_char->get_rect(), w_char->get_rect());

    interactions(game);
}

void Level9State::draw(Engine* game)
{
    SDL_Rect* cam_rect = camera->get_display();

    // draw stuff to the screen!
    for (int i = 0; i < (width * height); i++) {
        tileset[i]->render(status, cam_rect, game, &tile_tex);
    }
    for (int i = 0; i < crates.size(); i++) {
        crates[i]->render(status, cam_rect, game, this);
    }

    b_char->render(cam_rect, game);
    w_char->render(cam_rect, game);
    b_level_end->render(cam_rect, game);
    w_level_end->render(cam_rect, game);
    b_button.render(cam_rect, game);
    w_button.render(cam_rect, game);
    SDL_RenderPresent(game->rend);
}

void Level9State::cleanup()
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
    w_button_tex.free();
    b_springboard_tex.free();
    w_springboard_tex.free();
    b_cross_spring_tex.free();
    w_cross_spring_tex.free();

}

void Level9State::init_objects(Engine* game)
{
    b_char = new class Dot(2, 9, true, game->rend);
    w_char = new class Dot(2, 12, false, game->rend);
    camera = new class Camera(game->screen_width, game->screen_height,
                              width * TILE_WIDTH, height * TILE_WIDTH,
                              b_char->get_rect(), w_char->get_rect());

    b_level_end = new class LevelEnd(22, 20, true, game->rend);
    w_level_end = new class LevelEnd(22, 21, false, game->rend);


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

void Level9State::interactions(Engine* game)
{

    // if both are on level end object
    if(b_level_end->check(b_char->get_rect()) && w_level_end->check(w_char->get_rect())) {
        // change state to level 5
        //change_state(game, new Level5State);
    }


    //if black button is activated
    if(b_button.check(b_char->get_rect()) && b_button.used == false) {
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
        crates.push_back(new Crate(5, 12, FOUR_BY_TWO, false, game->rend));

    }
    else {
        b_button.activated = false;

        if(b_button.status != BUTT_INACTIVE) {
            b_button.status = (b_button.status + 1) % 4;
        }
    }
    //if white button is activated
    if(w_button.check(w_char->get_rect()) && w_button.used == false) {
        // used
        w_button.used = true;

        // activate
        w_button.activated = true;

        // animate
        if(w_button.status == BUTT_INACTIVE) {
            w_button.status = (w_button.status + 1) % 4;
        }

        // init crate #2
        crates.push_back(new Crate(5, 17, FOUR_BY_TWO, false, game->rend));

    }
    else {
        w_button.activated = false;

        if(w_button.status != BUTT_INACTIVE) {
            w_button.status = (w_button.status + 1) % 4;
        }
    }


}
