
#include <levels.hpp>
#include <states/menustate.hpp>
#include <char.hpp>
#include <utils.hpp>

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
    std::vector<Tile> tiles;
    int x = 0, y = 0;
    for (int i = 0; i < m_w * m_h; i++) {
        cur_tile = -1;

        level_file >> cur_tile;
        Tile new_tile(m_x + x, m_y + y, (TileType) cur_tile);
        tiles.push_back(new_tile);

        // iterate horizontally
        x += TILE_WIDTH;

        if (x >= m_w * TILE_WIDTH) {
            x = 0;
            y += TILE_WIDTH;
        }
    }
    tileset = new Tileset(tiles, m_x, m_y, m_w, m_h, palette);

    int num_objs;
    level_file >> num_objs;
    int obj_type;
    int obj_x, obj_y;
    int obj_w, obj_h;
    int obj_x2, obj_y2;
    int obj_pause_length, obj_move_length;
    bool obj_color;
    int obj_light_strength;
    float spring_vel;

    Spring* new_spring;
    MovingPlatform* new_platform;
    ShiftBlock* new_shiftblock;
    Crate* new_crate;
    XSpring* new_xspring;
    SmallLamp* new_smalllamp;

    for (int i = 0; i < num_objs; i++) {
        level_file >> obj_type;
        switch (obj_type)
        {
        case OBJECT_SPRING:
            level_file >> obj_x >> obj_y >> obj_color;
            level_file >> spring_vel;
            new_spring = new Spring(m_x + obj_x*TILE_WIDTH, m_y + (obj_y - (obj_color == 0))*TILE_WIDTH, obj_color, spring_vel*(obj_color - !obj_color), palette);
            objects.push_back(new_spring);
            break;
        case OBJECT_MOVING_PLATFORM:
            level_file >> obj_color >> obj_w >> obj_h >> obj_x >> obj_y >> obj_x2 >> obj_y2 >> obj_pause_length >> obj_move_length;
            new_platform = new MovingPlatform(m_x + obj_x*TILE_WIDTH, m_y + (obj_y*TILE_WIDTH), m_x + obj_x2*TILE_WIDTH, m_y + obj_y2*TILE_WIDTH, obj_w, obj_h, obj_color, true, obj_move_length, obj_pause_length, palette);
            objects.push_back(new_platform);
            break;
        case OBJECT_SHIFTBLOCK:
            level_file >> obj_x >> obj_y >> obj_w >> obj_h;
            new_shiftblock = new ShiftBlock(m_x + obj_x*TILE_WIDTH, m_y + (obj_y*TILE_WIDTH), obj_w, obj_h, palette);
            objects.push_back(new_shiftblock);
            for (int i = obj_x; i < obj_x + obj_w; i++) {
                for (int j = obj_y; j < obj_y + obj_h; j++) {
                    tileset->set_type(i + j*m_w, TILE_INVISIBLE);
                }
            }
            break;
        case OBJECT_CRATE:
            level_file >> obj_x >> obj_y >> obj_w >> obj_h >> obj_color;
            new_crate = new Crate(m_x + obj_x*TILE_WIDTH, m_y + obj_y*TILE_WIDTH, obj_w, obj_h, obj_color, palette);
            objects.push_back(new_crate);
            break;
        case OBJECT_XSPRING:
            level_file >> obj_x >> obj_y;
            new_xspring = new XSpring(m_x + obj_x*TILE_WIDTH, m_y + obj_y*TILE_WIDTH, palette);
            objects.push_back(new_xspring);
            break;
        case OBJECT_SMALL_LAMP:
            level_file >> obj_x >> obj_y >> obj_color >> obj_light_strength;
            new_smalllamp = new SmallLamp(m_x + obj_x*TILE_WIDTH, m_y + obj_y*TILE_WIDTH, obj_color, (float) obj_light_strength, palette);
            objects.push_back(new_smalllamp);
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

    load_level(zone_num, lvl_num, palette);
}

Level::~Level()
{

}

void Level::update_x(SDL_Rect black_player, SDL_Rect white_player)
{
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->update_x(black_player, white_player);
    }
}

void Level::update_y()
{
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->update_y();
    }
}

void Level::switch_lights(bool active_color)
{
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->has_light()) {
            if (objects[i]->get_color() == active_color) {
                objects[i]->turn_on_light();
            } else {
                objects[i]->turn_off_light();
            }
        }
    }
}

std::vector<Light> Level::get_lights()
{
    std::vector<Light> result;
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->has_light()) {
            result.push_back(objects[i]->get_light());
        }
    }
    return result;
}

void Level::draw_bg(Camera* cam, bool active_color)
{
    // check if we should even bother
    SDL_Rect m_rect = {m_x, m_y, m_w*TILE_WIDTH, m_h*TILE_WIDTH};
    Vector repos;
    if (!check_collision(m_rect, cam->get_display(), &repos)) {
        return;
    }

    // draw stuff to the screen!
    std::vector<Light> lights = get_lights();
    tileset->render(cam, lights, active_color);

    // draw objects
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->render_bg(cam, lights, active_color);
    }
}

void Level::draw_fg(Camera* cam, bool active_color)
{
    // draw objects
    std::vector<Light> lights = get_lights();
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->render_fg(cam, lights, active_color);
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
    ripple_x = ripple_y = 0;
    ripple_start = -1000;

    // first, read the zone-file
    char zone_num_cstr[2];
    snprintf(zone_num_cstr, 2, "%d", m_zone_num);
    std::string zone_num_str(zone_num_cstr);
    std::string path = "resources/level-files/"+zone_num_str+"/zone-file";
    std::ifstream zone_file(path.c_str());

    // what's the rgb?
    int red, green, blue;
    zone_file >> red >> green >> blue;
    srand(time(NULL));
    int a, b, c;
    palette.r = 50;
    palette.g = 200;
    palette.b = 255;

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
    levels[active_level]->switch_lights(active_color);

    // init camera
    camera = new Camera(game->screen_width, game->screen_height, levels[active_level],
        chars[active_color].get_rect(), chars[active_color].get_dir());
}

void Zonestate::update()
{
    if (controls_frozen) {
        freeze_duration--;
    }
    if (freeze_duration == 0) {
        controls_frozen = false;
    }

    for (int i = 0; i < chars.size(); i++) {
        chars[i].check_for_platforms(this);
    }
    levels[active_level]->update_x(chars[0].get_rect(), chars[1].get_rect());
    for (int i = 0; i < chars.size(); i++) {
        chars[i].update_x(this, chars[!i].get_rect());
    }
    levels[active_level]->update_y();
    for (int i = 0; i < chars.size(); i++) {
        chars[i].update_y(this);
    }
    camera->update(chars[active_color].get_rect(), chars[active_color].get_dir());
}

void Zonestate::draw()
{
    m_postprocessor.begin();

    glClearColor((float) palette.r / (float) 255, (float) palette.g / (float) 255, (float) palette.b / (float) 255, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < levels.size(); i++) {
        levels[i]->draw_bg(camera, active_color);
    }
    for (int i = 0; i < chars.size(); i++) {
        chars[i].render(camera, levels[active_level]);
    }
    for (int i = 0; i < levels.size(); i++) {
        levels[i]->draw_fg(camera, active_color);
    }

    m_postprocessor.render(SDL_GetTicks() - ripple_start, ripple_x, ripple_y);
    SDL_GL_SwapWindow(game->window);
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
        // shift active color
        active_color = !active_color;

        // freeze controls for both chars so nothing sticky happens
        chars[0].halt();
        chars[1].halt();

        // ripple effect
        ripple_start = SDL_GetTicks();
        SDL_Rect char_who_shifted = chars[active_color].get_rect();
        SDL_Rect cam_rect = camera->get_display();
        float aspect_ratio = ((float) game->screen_width / (float) cam_rect.w);
        ripple_x = (char_who_shifted.x + char_who_shifted.w/2 - cam_rect.x) * aspect_ratio;
        ripple_y = (char_who_shifted.y + char_who_shifted.h/2 - cam_rect.y) * aspect_ratio;

        // switch lights
        levels[active_level]->switch_lights(active_color);
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
        chars[0].good_exit(this);
        chars[1].good_exit(this);
        chars[0].save_state();
        chars[1].save_state();
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
    camera->traumatize(.7);
    chars[0].reset(this);
    chars[1].reset(this);
}
