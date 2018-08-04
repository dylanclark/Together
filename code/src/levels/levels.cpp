
#include <levels.hpp>
#include <states/menustate.hpp>
#include <char.hpp>

/******************/
/*   LOAD LEVEL   */
/******************/

void Level::load_level(int zone_num, int lvl_num, SDL_Color palette)
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
        Tile new_tile(m_x + x, m_y + y, (TileType) cur_tile);
        tileset.push_back(new_tile);

        // iterate horizontally
        x += TILE_WIDTH;

        if (x >= m_w * TILE_WIDTH) {
            x = 0;
            y += TILE_WIDTH;
        }
    }

    int num_objs;
    level_file >> num_objs;
    int obj_type;
    int obj_x, obj_y;
    bool obj_color;
    float spring_vel;
    Spring* new_spring;
    for (int i = 0; i < num_objs; i++) {
        level_file >> obj_type >> obj_x >> obj_y >> obj_color;
        switch (obj_type)
        {
        case OBJECT_SPRING:
            level_file >> spring_vel;
            new_spring = new Spring(m_x + obj_x*TILE_WIDTH, m_y + (obj_y - (obj_color == 0))*TILE_WIDTH, obj_color, spring_vel*(obj_color - !obj_color), palette);
            objects.push_back(new_spring);
            break;
        default:
            break;
        }
    }

    // close the file
    level_file.close();
}

/*************/
/*   LEVEL   */
/*************/

Level::Level(int zone_num, int lvl_num, int x, int y, SDL_Color palette)
{
    m_x = x*TILE_WIDTH;
    m_y = y*TILE_WIDTH;
    m_zone_num = zone_num;
    m_lvl_num = lvl_num;
    num_chars_ready = 0;

    if (!tile_tex.load_tile_sheet("tiles.png", &palette)) {
        printf("Failed to load tile sheet texture!\n");
    }

    load_level(zone_num, lvl_num, palette);
}

Level::~Level()
{

}

// this function returns true if the level has been exited
bool Level::update(Zonestate* zone, std::vector<Dot> &chars)
{
    // check to see if the chars are on an exit
    bool ready_flag;
    bool done = false;
    num_chars_ready = 0;
    for (int i = 0; i < chars.size(); i++) {
        ready_flag = false;
        // TODO check exiting
    }
    return false;
}

void Level::draw_bg(SDL_Rect cam_rect, bool active_color)
{
    // draw stuff to the screen!
    for (int i = 0; i < m_w * m_h; i++) {
        tileset[i].render_bg(active_color, &cam_rect, &tile_tex);
    }

    // draw objects
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->render(cam_rect, active_color);
    }
}

void Level::draw_fg(SDL_Rect cam_rect, bool active_color)
{
    for (int i = 0; i < m_w * m_h; i++) {
        tileset[i].render_fg(active_color, &cam_rect, &tile_tex);
    }
}

void Level::cleanup()
{

}

/*****************/
/*   ZONESTATE   */
/*****************/

void Zonestate::init()
{
    controls_frozen = false;

    // first, read the zone-file
    char zone_num_cstr[2];
    snprintf(zone_num_cstr, 2, "%d", m_zone_num);
    std::string zone_num_str(zone_num_cstr);
    std::string path = "resources/level-files/"+zone_num_str+"/zone-file";
    std::ifstream zone_file(path.c_str());

    // what's the rgb?
    int r, g, b;
    zone_file >> r >> g >> b;
    palette.r = r;
    palette.g = g;
    palette.b = b;

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
        Level* new_level = new Level(m_zone_num, i, lvl_x, lvl_y, palette);
        levels.push_back(new_level);
    }

    // init chars
    int start_lvl_x, start_lvl_y;
    start_lvl_x = levels[active_level]->get_x();
    start_lvl_y = levels[active_level]->get_y();
    Dot b_char(b_char_x + start_lvl_x/TILE_WIDTH, b_char_y + start_lvl_y/TILE_WIDTH, false, &palette);
    Dot w_char(w_char_x + start_lvl_x/TILE_WIDTH, w_char_y + start_lvl_y/TILE_WIDTH, true, &palette);
    chars.push_back(b_char);
    chars.push_back(w_char);
    active_color = false;

    bchar_lvl_x = chars[0].get_x();
    bchar_lvl_y = chars[0].get_y();
    bchar_lvl_yvel = chars[0].get_yvel();
    wchar_lvl_x = chars[1].get_x();
    wchar_lvl_y = chars[1].get_y();
    wchar_lvl_yvel = chars[1].get_yvel();

    // init camera
    camera = new Camera(game->screen_width, game->screen_height, levels[active_level],
        chars[active_color].get_rect(), chars[active_color].get_dir());
}

void Zonestate::update()
{
    // clear the window
    SDL_RenderClear(game->rend);

    if (controls_frozen) {
        freeze_duration--;
    }
    if (freeze_duration == 0) {
        controls_frozen = false;
    }

    for (int i = 0; i < chars.size(); i++) {
        chars[i].update(this);
    }
    camera->update(chars[active_color].get_rect(), chars[active_color].get_dir());

    /*
    int lvl_x, lvl_y, lvl_w, lvl_h;
    if (levels[active_level]->update(this, chars)) {
        SDL_Rect char_rect = chars[0].get_rect();
        // update active level
        int lvl_x, lvl_y, lvl_w, lvl_h;
        for (int i = 0; i < levels.size(); i++) {
            if (i == active_level) {
                continue;
            }
            lvl_x = levels[i]->get_x();
            lvl_y = levels[i]->get_y();
            lvl_w = levels[i]->get_w()*TILE_WIDTH;
            lvl_h = levels[i]->get_h()*TILE_WIDTH;
            if (char_rect.x >= lvl_x && char_rect.x < lvl_x + lvl_w &&
                char_rect.y >= lvl_y && char_rect.y < lvl_y + lvl_h) {
                active_level = i;
                levels[active_level]->just_exited = true;
                camera->set_level(levels[active_level]);
                break;
            }
        }
    }
    */
}

void Zonestate::draw()
{
    SDL_Rect* cam_rect = camera->get_display();
    for (int i = 0; i < levels.size(); i++) {
        levels[i]->draw_bg(*cam_rect, active_color);
    }
    for (int i = 0; i < chars.size(); i++) {
        chars[i].render(cam_rect, levels[active_level]);
    }
    for (int i = 0; i < levels.size(); i++) {
        levels[i]->draw_fg(*cam_rect, active_color);
    }

    SDL_RenderPresent(game->rend);
}

void Zonestate::handle_events()
{
    if (controls_frozen) {
        return;
    }

    // event handler
    SDL_Event e;

    // handle those events, bruh
    while (SDL_PollEvent(&e)) {
        switch (e.type)
        {
        case SDL_QUIT:
            game->quit();
            break;
        }
        chars[active_color].handle_event(e, this);
    }
    shiftable = true;
}

void Zonestate::cleanup()
{

}

void Zonestate::pause()
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
    Level* level = levels[active_level];
    return level;
}

void Zonestate::check_exit()
{
    int result = -1;
    // check that chars exited same way
    if (chars[0].get_exit_dir() == chars[1].get_exit_dir()) {
        // determine new active level
        for (int i = 0; i < levels.size(); i++) {
            if (i == active_level) {
                continue;
            }
            if (chars[0].in_level(levels[i])) {
                result = i;
                break;
            }
        }
    }
    if (result == -1) {
        shift();
    } else {
        chars[0].good_exit();
        chars[1].good_exit();
        bchar_lvl_x = chars[0].get_x();
        bchar_lvl_y = chars[0].get_y();
        bchar_lvl_yvel = chars[0].get_yvel();
        wchar_lvl_x = chars[1].get_x();
        wchar_lvl_y = chars[1].get_y();
        wchar_lvl_yvel = chars[1].get_yvel();
        active_level = result;
        int transition_duration = 25;
        SDL_Rect char_rect = chars[active_color].get_rect();
        int char_dir = chars[active_color].get_dir();
        camera->set_level(levels[active_level], char_rect, char_dir, transition_duration);
        freeze_controls(transition_duration);
    }
}

void Zonestate::freeze_controls(int duration)
{
    controls_frozen = true;
    freeze_duration = duration;
}

void Zonestate::reset_level()
{
    int transition_duration = 10;
    freeze_controls(transition_duration);
    chars[0].reset(bchar_lvl_x, bchar_lvl_y, bchar_lvl_yvel);
    chars[1].reset(wchar_lvl_x, wchar_lvl_y, wchar_lvl_yvel);
}
