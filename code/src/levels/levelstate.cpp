
#include <stdio.h>
#include <SDL2/SDL_mixer.h>

#include <levels.hpp>
#include <engine.hpp>
#include <states/menustate.hpp>

void Levelstate::init(Engine* game)
{
    active_color = 0;

    // get formatted level number string
    char lvl_num_cstr[3];
    snprintf(lvl_num_cstr, 3, "%02d", m_lvl_num);
    std::string lvl_num_str(lvl_num_cstr);

    // load level file
    load_level(game, lvl_num_str);

    // init camera
    camera = new class Camera(game->screen_width, game->screen_height,
                              width * TILE_WIDTH, height * TILE_WIDTH,
                              chars[active_color]->get_rect(),
                              chars[active_color]->get_dir());

    // save if necessary
    if (game->read_save() < m_lvl_num) {
        game->save(m_lvl_num);
    }
}

void Levelstate::shift()
{
    if (chars.size() == 2) {
        active_color = !active_color;
    }
}

void Levelstate::handle_events(Engine* game)
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
        chars[active_color]->handle_event(event, this, game);
    }
    shiftable = true;
}

void Levelstate::update(Engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    for (int i = 0; i < chars.size(); i++) {
        chars[i]->update(this, game);
    }
    for (int i = 0; i < crates.size(); i++) {
        crates[i]->update();
    }
    camera->update(chars[active_color]->get_rect(), chars[active_color]->get_dir());

    bool done = true;
    // if both are on level end object
    for (int i = 0; i < chars.size(); i++) {
        if (!level_ends[i]->check(chars[i]->get_rect())) {
            done = false;
        }
    }
    if (done) {
        change_state(game, new Levelstate(m_lvl_num+1));
    }
}

void Levelstate::load_level(Engine* game, std::string lvlnum)
{
    if (!load_level_from_file(game, this, "level"+lvlnum+".lvl")) {
        printf("Failed to load level %s map!\n", lvlnum.c_str());
        return;
    }
    if (!tile_tex.load_tile_sheet("tiles.png", game->rend, &palette)) {
        printf("Failed to load tile sheet texture!\n");
        return;
    }

    /*
    if (!b_button_tex.load_tile_sheet("black/button/b_button.png", game->rend)) {
        printf("Failed to load  black button texture!\n");
        return;
    }
    if (!w_button_tex.load_tile_sheet("white/button/w_button.png", game->rend)) {
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
    */
}

void Levelstate::draw(Engine* game)
{
    SDL_Rect* cam_rect = camera->get_display();

    // draw stuff to the screen!
    for (int i = 0; i < (width * height); i++) {
        tileset[i]->render(active_color, cam_rect, game, &tile_tex);
    }
    for (int i = 0; i < chars.size(); i++) {
        chars[i]->render(cam_rect, game);
    }
    for (int i = 0; i < level_ends.size(); i++) {
        level_ends[i]->render(cam_rect, game);
    }
    for (int i = 0; i < crates.size(); i++) {
        crates[i]->render(active_color, cam_rect, game, this);
    }
    SDL_RenderPresent(game->rend);
}

void Levelstate::cleanup()
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

void
Levelstate::pause(Engine* game)
{
    Mix_PauseMusic();
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->push_state(new PauseMenuState);
}
