
#include <stdio.h>
#include <SDL2/SDL_mixer.h>

#include <levels.hpp>
#include <engine.hpp>
#include <states/pausemenu-state.hpp>

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
        // quit if he pressed escape
        for (int i = 0; i < chars.size(); i++) {
            chars[i]->handle_event(event, this, game);
        }
    }
    shiftable = true;
}

void Levelstate::update(Engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    for (int i = 0; i < chars.size(); i++) {
        chars[i]->move(this, game);
    }
    for (int i = 0; i < crates.size(); i++) {
        crates[i]->update();
    }
    if (chars.size() == 1) {
        camera->update(chars[0]->get_rect(), chars[0]->get_rect());
    } else {
        camera->update(chars[0]->get_rect(), chars[1]->get_rect());
    }
    interactions(game);
}

void Levelstate::load_tiles(Engine* game, std::string lvlnum)
{
    if (!tile_tex.load_tile_sheet("tile_sheet.png", game->rend, &palette)) {
        printf("Failed to load tile sheet texture!\n");
        return;
    }
    if (!set_tiles(this, tileset, "level"+lvlnum+".lvl")) {
        printf("Failed to load level %s map!\n", path.c_str());
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

void
Levelstate::pause(Engine* game)
{
    Mix_PauseMusic();
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->push_state(new PauseMenuState);
}
