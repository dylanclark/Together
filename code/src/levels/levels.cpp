
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
    m_dir = dir;

    m_tex.load_object(m_rect.w, m_rect.h, "black/level_end/black_end.png", rend, palette);
}

// returns the number of chars on the exit
int LevelExit::check(SDL_Rect char_rect)
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
        LevelExit new_exit(m_x + exit_x, m_y + exit_y, (ExitDir) exit_dir, game->rend, &palette);
        exits.push_back(new_exit);
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
    num_chars_ready = 0;
    chosen_exit = -1;

    if (!tile_tex.load_tile_sheet("tiles.png", game->rend, &palette)) {
        printf("Failed to load tile sheet texture!\n");
    }

    load_level(game, zone_num, lvl_num, palette);
}

// this function returns the number of chars on an exit
int Level::update(Zonestate* zone, std::vector<Dot> chars)
{
    // lol update crates i guess
    for (int i = 0; i < crates.size(); i++) {
        crates[i]->update();
    }
    // check to see if the chars are on an exit
    bool ready_flag = false;
    for (int i = 0; i < chars.size(); i++) {
        // we only want to snap them into place if they haven't been snapped
        for (int j = 0; i < exits.size(); i++) {
            if (exits[j].check(chars[i].get_rect())) {
                ready_flag = true;
                // if this is the first
                if (num_chars_ready == 0) {
                    num_chars_ready++;
                    chosen_exit = j;
                } else if (num_chars_ready == 1 && chosen_exit == j) {
                    num_chars_ready++;
                }
                // if the char hasn't been snapped, snap it (and set snapped to true)
                if (!chars[i].snapped) {
                    chars[i].snap(exits[j].get_rect());
                    zone->shift();
                    break;
                }
            }
        }
        if (!ready_flag && chars[i].snapped) {
            // this means the char was snapped but is no longer colliding
            chars[i].snapped = false;
            num_chars_ready--;
        }
    }
    if (num_chars_ready == 2) {
        ExitDir dir = exits[chosen_exit].get_dir();
        int new_x, new_y;
        // move the chars in the correct direction
        if (dir == EXIT_UP) {
            new_x = 0;
            new_y = -4*TILE_WIDTH;
        } else if (dir == EXIT_DOWN) {
            new_x = 0;
            new_y = 4*TILE_WIDTH;
        } else if (dir == EXIT_LEFT) {
            new_x = -4*TILE_WIDTH;
            new_y = 0;
        } else if (dir == EXIT_RIGHT) {
            new_x = 4*TILE_WIDTH;
            new_y = 0;
        }
        for (int i = 0; i < chars.size(); i++) {
            chars[i].move(new_x, new_y);
        }
    }
    return num_chars_ready;
}

void Level::draw(Engine* game, SDL_Rect cam_rect, bool active_color)
{
    // draw stuff to the screen!
    for (int i = 0; i < m_w * m_h; i++) {
        tileset[i]->render(active_color, &cam_rect, game, &tile_tex);
    }
    for (int i = 0; i < exits.size(); i++) {
        exits[i].render(game, cam_rect);
    }
    for (int i = 0; i < crates.size(); i++) {
        // TODO: fix NULL here (crate->render)
        crates[i]->render(active_color, &cam_rect, game, NULL);
    }
}

void Level::cleanup()
{

}

/*****************/
/*   ZONESTATE   */
/*****************/

void Zonestate::init(Engine* game)
{
    // first, read the zone-file
    char zone_num_cstr[2];
    snprintf(zone_num_cstr, 2, "%d", m_zone_num);
    std::string zone_num_str(zone_num_cstr);
    std::string path = "resources/level-files/"+zone_num_str+"/zone-file";
    std::ifstream zone_file(path.c_str());

    // what's the rgb?
    zone_file >> palette.r >> palette.g >> palette.b;
    // how many lvls?
    int num_levels;
    zone_file >> num_levels;
    // where do we start?
    int first_level;
    zone_file >> first_level;
    active_level = first_level;
    int b_char_x, b_char_y;
    int w_char_x, w_char_y;
    zone_file >> b_char_x >> b_char_y;
    zone_file >> w_char_x >> w_char_y;

    // for each level, create a level object at that level's coords
    int lvl_x, lvl_y;
    for (int i = 0; i < num_levels; i++) {
        zone_file >> lvl_x >> lvl_y;
        Level new_level(game, m_zone_num, i, lvl_x, lvl_y, palette);
        levels.push_back(new_level);
    }

    // init chars
    int start_lvl_x, start_lvl_y;
    start_lvl_x = levels[active_level].get_x();
    start_lvl_y = levels[active_level].get_y();
    Dot b_char(b_char_x + start_lvl_x, b_char_y + start_lvl_y, true, game->rend, &palette);
    Dot w_char(w_char_x + start_lvl_x, w_char_y + start_lvl_y, false, game->rend, &palette);
    chars.push_back(b_char);
    chars.push_back(w_char);
    active_color = true;

    // init camera
    camera = new Camera(game->screen_width, game->screen_height, levels[active_level],
        chars[active_color].get_rect(), chars[active_color].get_dir());
}

void Zonestate::update(Engine* game)
{
    // clear the window
    SDL_RenderClear(game->rend);

    for (int i = 0; i < chars.size(); i++) {
        chars[i].update(this, game);
    }
    camera->update(chars[active_color].get_rect(), chars[active_color].get_dir());
    int lvl_x, lvl_y, lvl_w, lvl_h;
    SDL_Rect char_rect = chars[0].get_rect();
    if (levels[active_level].update(this, chars) == 2) {
        // update active level
        int lvl_x, lvl_y, lvl_w, lvl_h;
        for (int i = 0; i < levels.size(); i++) {
            lvl_x = levels[i].get_x()*TILE_WIDTH;
            lvl_y = levels[i].get_y()*TILE_WIDTH;
            lvl_w = levels[i].get_w()*TILE_WIDTH;
            lvl_h = levels[i].get_h()*TILE_WIDTH;
            if (char_rect.x >= lvl_x && char_rect.x < lvl_x + lvl_w &&
                char_rect.y >= lvl_y && char_rect.y < lvl_y + lvl_h) {
                active_level = i;
                break;
            }
        }
    }
}

void Zonestate::draw(Engine* game)
{
    SDL_Rect* cam_rect = camera->get_display();
    for (int i = 0; i < levels.size(); i++) {
        levels[i].draw(game, *cam_rect, active_color);
    }
    for (int i = 0; i < chars.size(); i++) {
        chars[i].render(cam_rect, game);
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
        chars[active_color].handle_event(event, this, game);
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

Level* Zonestate::get_active_level()
{
    Level* level = &levels[active_level];
    return level;
}
