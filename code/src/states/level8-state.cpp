
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

void Level8State::init(Engine* game)
{
    // load textures
    load_tiles(game);

    // initialize objects
    init_objects(game);

    if (game->read_save() < 8)
    {
        game->save(8);
    }
}

void Level8State::handle_events(Engine* game)
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

void Level8State::update(Engine* game)
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

    camera->update(b_char->get_rect(), w_char->get_rect());

    interactions(game);
}

void Level8State::draw(Engine* game)
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
    b_level_end->render(cam_rect, game);
    w_level_end->render(cam_rect, game);
    w_char->render(cam_rect, game);
    b_button.render(cam_rect, game);
    w_button.render(cam_rect, game);
    b_springboard.render(cam_rect, game);
    w_springboard.render(cam_rect, game);
    SDL_RenderPresent(game->rend);
}

void Level8State::cleanup()
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

void Level8State::load_tiles(Engine* game)
{
    if (!tile_tex.load_tile_sheet("tile_sheet.png", game->rend)) {
        printf("Failed to load tile sheet texture!\n");
        return;
    }
    if (!b_button_tex.load_tile_sheet("black/button/b_button.png", game-> rend)) {
        printf("Failed to load  black button texture!\n");
        return;
    }
    if (!w_button_tex.load_tile_sheet("white/button/w_button.png", game-> rend)) {
        printf("Failed to load white button texture!\n");
        return;
    }
    if (!b_springboard_tex.load_tile_sheet("black/spring/b_spring.png", game-> rend)) {
        printf("Failed to load black springboard texture!\n");
        return;
    }
    if (!w_springboard_tex.load_tile_sheet("white/spring/w_spring.png", game-> rend)) {
        printf("Failed to load white springboard texture!\n");
        return;
    }
    if (!set_tiles(this, tileset, "level08.lvl")) {
        printf("Failed to load level 8 map!\n");
        return;
    }
}

void Level8State::init_objects(Engine* game)
{

    b_char = new class Dot(2, 9, true, game->rend);
    w_char = new class Dot(2, 11, false, game->rend);
    camera = new class Camera(game->screen_width, game->screen_height,
                              width * TILE_WIDTH, height * TILE_WIDTH,
                              b_char->get_rect(), w_char->get_rect());

    // initialize black level end
    b_level_end = new class LevelEnd(22, 16, true, game->rend);
    w_level_end = new class LevelEnd(22, 17, false, game->rend);


    // initialize black button
    b_button.tex = b_button_tex;
    b_button.col_rect.x = 1080;
    b_button.col_rect.y = 2 * TILE_WIDTH;
    b_button.single = true;
    b_button.used = false;
    b_button.direction = UP;

    // initialize white button
    w_button.tex = w_button_tex;
    w_button.col_rect.x = 1240;
    w_button.col_rect.y = 8 * TILE_WIDTH;
    w_button.single = true;
    w_button.used = false;
    w_button.direction = UP;


}

void Level8State::interactions(Engine* game)
{

    // if both are on level end object
    if(b_level_end->check(b_char->get_rect()) && w_level_end->check(w_char->get_rect())) {
        change_state(game, new MainMenuState);
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
        crates.push_back(new Crate(10, 11, FOUR_BY_TWO, false, game->rend));

        // initialize black springboard
        w_springboard.tex = w_springboard_tex;
        w_springboard.col_rect.x = 2 * TILE_WIDTH;
        w_springboard.col_rect.y = 11 * TILE_WIDTH;
        w_springboard.show = true;
        w_springboard.direction = FLIP_LEFT;
        w_springboard.x_spring = 4;
        w_springboard.y_spring = 12;
    }
    else {
        b_button.activated = false;

        if(b_button.status != BUTT_INACTIVE) {
            b_button.status = (b_button.status + 1) % 4;
        }
    }
    //if white button is activated
    if(w_button.check(w_char->get_rect()) && w_button.used == false) {
        w_button.used = true;
        w_button.activated = true;

        if(w_button.status == BUTT_INACTIVE) {
            w_button.status = (w_button.status + 1) % 4;
        }

        // initialize black springboard
        b_springboard.tex = b_springboard_tex;
        b_springboard.col_rect.x = 1280;
        b_springboard.col_rect.y = 7 * TILE_WIDTH;
        b_springboard.show = true;
        b_springboard.direction = FLIP_LEFT;
        b_springboard.x_spring = 0;
        b_springboard.y_spring = 10;

        // init crate #2
        crates.push_back(new Crate(5, 17, FOUR_BY_TWO, false, game->rend));
    }
    else {
        w_button.activated = false;

        if(w_button.status != BUTT_INACTIVE) {
            w_button.status = (w_button.status + 1) % 4;
        }
    }

    //if black springboard is activated
    if(b_springboard.check(b_char->get_rect()) && b_springboard.show)
    {
        //if(b_char->col_rect.y < b_springboard.col_rect.y + 20)
        {
            // activate
            b_springboard.activated = true;

            if(b_springboard.status == BOARD_INACTIVE) {
                b_springboard.status = (b_springboard.status + 1) % 4;
            }

            b_char->spring(b_springboard.x_spring, b_springboard.y_spring, b_springboard.direction);
        }
    }
    else {
        b_springboard.activated = false;

        if(b_springboard.status != BOARD_INACTIVE) {
            b_springboard.status = (b_springboard.status + 1) % 4;
        }
    }

    //if black springboard is activated
    if(w_springboard.check(w_char->get_rect()) && w_springboard.show) {
        //if(b_char->col_rect.y < b_springboard.col_rect.y + 20)
        {
            // activate
            w_springboard.activated = true;

            if(w_springboard.status == BOARD_INACTIVE)
            {
                w_springboard.status = (w_springboard.status + 1) % 4;
            }

            w_char->spring(w_springboard.x_spring, w_springboard.y_spring, w_springboard.direction);
        }
    }
    else {
        w_springboard.activated = false;

        if(w_springboard.status != BOARD_INACTIVE)
        {
            w_springboard.status = (w_springboard.status + 1) % 4;
        }
    }

}