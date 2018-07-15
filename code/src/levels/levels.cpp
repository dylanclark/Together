
#include <levels.hpp>
#include <states/menustate.hpp>

/******************/
/*   LEVEL EXIT   */
/******************/

LevelExit::LevelExit(int x, int y, ExitDir dir, SDL_Renderer* rend, SDL_Color* palette)
{
    m_rect.x = x;
    m_rect.y = y;
    m_rect.w = (dir == EXIT_LEFT || dir == EXIT_RIGHT) ? TILE_WIDTH*2 : TILE_WIDTH;
    m_rect.h = (dir == EXIT_UP || dir == EXIT_DOWN) ? TILE_WIDTH*2 : TILE_WIDTH;

    m_tex.load_object(m_rect.w, m_rect.h, "", rend, palette);
}

bool LevelExit::check(std::vector<Dot*> chars)
{
    // TODO
    return false;
}

void LevelExit::render(Engine* game, SDL_Rect camera)
{
    m_tex.render(m_rect.x, m_rect.y, NULL, &camera, game);
}


/******************/
/*   LOAD LEVEL   */
/******************/

void Level::load_level(Engine* game, int zone_num, int lvl_num, SDL_Color palette)
{
    char lvl_cstr[5];
    snprintf(lvl_cstr, 5, "%d-%02d", m_zone_num, m_lvl_num);
    std::string lvl_str(lvl_cstr);
    lvl_cstr[1] = '\0';
    std::string zone_str(lvl_cstr);
    std::string path = "resources/level-files/"+zone_str+"/level"+lvl_str+".lvl";
    std::ifstream level_file(path.c_str());

    level_file >> m_w;
    level_file >> m_h;

    int cur_tile;
    int x = 0, y = 0;
    for (int i = 0; i < m_w * m_h; i++) {
        cur_tile = -1;

        level_file >> cur_tile;

        tileset[i] = new Tile(m_x + x, m_y + y, cur_tile);

        // iterate horizontally
        x += TILE_WIDTH;

        if (x >= m_w * TILE_WIDTH) {
            x = 0;
            y += TILE_WIDTH;
        }
    }

    int num_exits;
    level_file >> num_exits;
    int exit_x, exit_y, exit_dir;
    for (int i = 0; i < num_exits; i++) {
        level_file >> exit_x;
        level_file >> exit_y;
        level_file >> exit_dir;
        exits.push_back(new LevelExit(m_x + exit_x, m_y + exit_y, (ExitDir) exit_dir, game->rend, &palette));
    }

    // close the file
    level_file.close();
}

/*************/
/*   LEVEL   */
/*************/

Level::Level(Engine* game, int zone_num, int lvl_num, int x, int y, SDL_Color palette)
{
    m_x = x;
    m_y = y;
    m_zone_num = zone_num;
    m_lvl_num = lvl_num;

    if (!tile_tex.load_tile_sheet("tiles.png", game->rend, &palette)) {
        printf("Failed to load tile sheet texture!\n");
    }

    load_level(game, zone_num, lvl_num, palette);
}

void Level::update(std::vector<Dot*> chars)
{
    // lol update crates i guess
    for (int i = 0; i < crates.size(); i++) {
        crates[i]->update();
    }
    // check to see if the chars are on an exit
    for (int i = 0; i < exits.size(); i++) {
        if (exits[i]->check(chars)) {
            // TODO: move the chars to the proper place
            break;
        }
    }
}

void Level::draw(Engine* game, SDL_Rect cam_rect, bool active_color)
{
    // draw stuff to the screen!
    for (int i = 0; i < m_w * m_h; i++) {
        tileset[i]->render(active_color, &cam_rect, game, &tile_tex);
    }
    for (int i = 0; i < exits.size(); i++) {
        exits[i]->render(game, cam_rect);
    }
    for (int i = 0; i < crates.size(); i++) {
        // TODO: fix NULL here (crate->render)
        crates[i]->render(active_color, &cam_rect, game, NULL);
    }
}

void Level::cleanup()
{
    // TODO will we ever do this?
}

/*****************/
/*   ZONESTATE   */
/*****************/

void Zonestate::init(Engine* game)
{
    // TODO lots
    // first, read the zone-file
    char zone_num_cstr[2];
    snprintf(zone_num_cstr, 2, "%d", m_zone_num);
    std::string zone_num_str(zone_num_cstr);
    std::string path = "resources/level-files/"+zone_num_str+"/zone-file";
    std::ifstream zone_file(path.c_str());

    // find out how many levels we got
    zone_file >> palette.r >> palette.g >> palette.b;

    int num_levels;
    zone_file >> num_levels;

    // for each level, create a level object at that level's coords
    int lvl_x, lvl_y;
    for (int i = 0; i < num_levels; i++) {
        zone_file >> lvl_x >> lvl_y;
        levels.push_back(new Level(game, m_zone_num, i, lvl_x, lvl_y, palette));
    }
    // finally figure out where our chars start
}

void Zonestate::update(Engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    for (int i = 0; i < chars.size(); i++) {
        // TODO: fix this NULL (chars->update)
        chars[i]->update(NULL, game);
    }
    camera->update(chars[active_color]->get_rect(), chars[active_color]->get_dir());

    for (int i = 0; i < levels.size(); i++) {
        levels[i]->update(chars);
    }
}

void Zonestate::draw(Engine* game)
{
    SDL_Rect* cam_rect = camera->get_display();
    for (int i = 0; i < levels.size(); i++) {
        levels[i]->draw(game, *cam_rect, active_color);
    }
    for (int i = 0; i < chars.size(); i++) {
        chars[i]->render(cam_rect, game);
    }

    SDL_RenderPresent(game->rend);
}

void Zonestate::handle_events(Engine* game)
{
    // event handler
    SDL_Event event;

    // handle those events, bruh
    while (SDL_PollEvent(&event)) {
        switch (event.type)
        {
        case SDL_QUIT:
            game->quit();
            break;
        }
        // TODO: fix chars->handle_event
        chars[active_color]->handle_event(event, NULL, game);
    }
    shiftable = true;
}

void Zonestate::cleanup()
{

}

void Zonestate::pause(Engine* game)
{
    Mix_PauseMusic();
    Mix_PlayChannel(-1, game->sound->menu_exit_snd, 0);
    game->push_state(new PauseMenuState);
}

void Zonestate::shift()
{
    if (chars.size() == 2) {
        active_color = !active_color;
    }
}
