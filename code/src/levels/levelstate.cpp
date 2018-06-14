
#include <stdio.h>
#include <SDL2/SDL_mixer.h>

#include <levels.hpp>
#include <engine.hpp>
#include <states/pausemenu-state.hpp>

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

    interactions(game);
}

void Levelstate::load_level(Engine* game, std::string lvlnum)
{
    if (!load_level_from_file(game, this, "level"+lvlnum+".lvl")) {
        printf("Failed to load level %s map!\n", path.c_str());
        return;
    }
    if (!tile_tex.load_tile_sheet("tile_sheet.png", game->rend, &palette)) {
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

void
Levelstate::pause(Engine* game)
{
    Mix_PauseMusic();
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->push_state(new PauseMenuState);
}
