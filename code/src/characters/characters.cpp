
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header file
#include <char.hpp>
#include <utils.hpp>
#include <engine.hpp>
#include <textures.hpp>
#include <levels.hpp>
#include <objects.hpp>
#include <tiles.hpp>
#include <editor.hpp>

static const int DEAD_ZONE = 8000;

static const int DOT_VEL = 3;
static const int JUMP_VEL = 6;
static const float DOT_ACC = .5;
static const int PUSH_VEL = 2;

Dot::Dot(int x, int y, bool color, SDL_Color* palette)
{
    // initialize velocity
    m_xvel = m_yvel = 0;
    on_moving_platform = false;
    on_moving_platform_idx = 0;
    in_moving_platform = false;
    in_moving_platform_idx = 0;
    in_crate = false;
    in_crate_idx = 0;

    // initialize controllers
    up = false;
    down = false;
    right = false;
    left = false;
    dir = DIR_RIGHT;
    exiting = exited = entering = false;

    // initiliaze gamepad
    controller = new class Controller;

    m_color = color;
    Texture m_tex;
    if (m_color == 0) {
        m_status = CHAR_IDLE;
        m_tex = ResourceManager::get_texture("black_player");
    } else {
        m_status = CHAR_INACTIVE;
        m_tex = ResourceManager::get_texture("white_player");
    }
    m_sprite = Sprite(m_tex, 16, 16, palette);

    // initialize collision rectangle
    col_rect.w = 12;
    col_rect.h = 15;
    col_rect.x = x*TILE_WIDTH;
    col_rect.y = y*TILE_WIDTH - (m_color == 0)*(col_rect.h-TILE_WIDTH);;
    true_y = col_rect.y;

    save_state();
}

bool Dot::handle_event(SDL_Event &e, Zonestate* zone)
{
    if (exiting || entering) {
        return true;
    }
    // handle those events, duder
    switch (e.type)
    {
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode)
            {
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_SPACE:
                    up = true;
                    break;
                case SDL_SCANCODE_DOWN:
                case SDL_SCANCODE_S:
                    down = true;
                    break;
                case SDL_SCANCODE_LEFT:
                case SDL_SCANCODE_A:
                    left = true;
                    break;
                case SDL_SCANCODE_RIGHT:
                case SDL_SCANCODE_D:
                    right = true;
                    break;
                case SDL_SCANCODE_Q:
                case SDL_SCANCODE_LSHIFT:
                case SDL_SCANCODE_RSHIFT:
                    if (zone->shiftable) {
                        up = left = down = right = 0;
                        m_xvel = 0;
                        zone->shift();
                    }
                    break;
                case SDL_SCANCODE_R:
                    game->restart_state();
                    break;
                case SDL_SCANCODE_E:
                    game->change_state(new ZoneEditor());
                    break;
                case SDL_SCANCODE_ESCAPE:
                    zone->pause();
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (e.key.keysym.scancode)
            {
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_SPACE:
                    up = false;
                    break;
                case SDL_SCANCODE_DOWN:
                case SDL_SCANCODE_S:
                    down = false;
                    break;
                case SDL_SCANCODE_LEFT:
                case SDL_SCANCODE_A:
                    left = false;
                    break;
                case SDL_SCANCODE_RIGHT:
                case SDL_SCANCODE_D:
                    right = false;
                    break;
                default:
                    break;
            }
            break;
        case SDL_JOYAXISMOTION:
            if (e.jaxis.which == 0)
            {
                switch (e.jaxis.axis)
                {
                    case 0:
                        if (e.jaxis.value > DEAD_ZONE)
                        {
                            right = true;
                            left = false;
                            break;
                        }
                        else if (e.jaxis.value < -(DEAD_ZONE))
                        {
                            left = true;
                            right = false;
                            break;
                        }
                        else
                        {
                            left = false;
                            right = false;
                            break;
                        }
                        break;
                    case 1:
                        if (e.jaxis.value > DEAD_ZONE)
                        {
                            down = true;
                            break;
                        }
                        else if (e.jaxis.value < -(DEAD_ZONE))
                        {
                            // up = true;
                            break;
                        }
                        else
                        {
                            // up = false;
                            down = false;
                            break;
                        }
                        break;
                }
            }
            break;
        case SDL_CONTROLLERBUTTONDOWN:
            switch (e.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_A:
                    up = true;
                    break;
                case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                case SDL_CONTROLLER_BUTTON_X:
                    if (zone->shiftable)
                    {
                        Mix_PlayChannel(-1, game->sound->level_switch_snd, 0);
                        up = left = down = right = 0;
                        zone->shift();
                    }
                    break;
                case SDL_CONTROLLER_BUTTON_Y:
                    game->restart_state();
                    break;
                case SDL_CONTROLLER_BUTTON_START:
                    return false;
                    break;
            }
            break;
        case SDL_CONTROLLERBUTTONUP:
            switch (e.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_A:
                    up = false;
                    break;
            }
        default:
            break;
    }
    // success! (no quitting)
    return true;
}

void Dot::check_for_platforms(Zonestate* zone)
{
    Level* level = zone->get_active_level();
    std::vector<Object*> objects = level->get_objects();
    on_moving_platform = false;
    in_moving_platform = false;
    in_crate = false;
    for (int i = 0; i < objects.size(); i++) {
        switch (objects[i]->get_type())
        {
        case OBJECT_MOVING_PLATFORM:
            if (objects[i]->get_color() != m_color) {
                if (check_in_platform(col_rect, objects[i]->get_rect())) {
                    in_moving_platform = true;
                    in_moving_platform_idx = i;
                    in_platform_x = objects[i]->get_rect().x;
                    in_platform_y = objects[i]->get_rect().y;
                }
            } else {
                if (check_grounded(col_rect, objects[i]->get_rect(), m_color)) {
                    on_moving_platform = true;
                    on_moving_platform_idx = i;
                    on_platform_x = objects[i]->get_rect().x;
                    on_platform_y = objects[i]->get_rect().y;
                }
            }
            break;
        case OBJECT_CRATE:
            if (objects[i]->get_color() != m_color) {
                if (m_status == CHAR_INACTIVE) {
                    Vector repos;
                    if (check_collision(col_rect, objects[i]->get_rect(), &repos)) {
                        in_crate = true;
                        in_crate_idx = i;
                    }
                } else {
                    if (check_in_platform(col_rect, objects[i]->get_rect())) {
                        in_crate = true;
                        in_crate_idx = i;
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}

void Dot::update_x(Zonestate* zone, SDL_Rect other_player)
{
    Level* level = zone->get_active_level();
    if (exiting) {
        int lvl_x = level->get_x();
        int lvl_y = level->get_y();
        int lvl_w = level->get_w()*TILE_WIDTH;
        int lvl_h = level->get_h()*TILE_WIDTH;
        if (col_rect.x + col_rect.w < lvl_x || col_rect.x > lvl_x + lvl_w) {
            exiting = false;
            exited = true;
            left = right = up = down = false;
            m_xvel = 0;
            zone->check_exit();
        }
    }

    if (exited && zone->active_color == m_color) {
        enter();
    }

    if (zone->active_color != m_color && m_status != CHAR_EXITED) {
        m_status = (entering || exited) ? CHAR_EXITED : CHAR_INACTIVE;
    } else {
        if (m_status == CHAR_INACTIVE) {
            m_status = CHAR_IDLE;
        }
    }

    // update x velocity
    if (right && !left && m_xvel < DOT_VEL) {
        m_xvel += DOT_ACC;
    }
    if (left && !right && m_xvel > -(DOT_VEL)) {
        m_xvel -= DOT_ACC;
    }
    if ((!right && !left) || (right && left)) {
        if (m_xvel < 0) {
            m_xvel += DOT_ACC;
        } else if (m_xvel > 0) {
            m_xvel -= DOT_ACC;
        }
    }

    // if the char is moving slowly enough, stop it
    if (m_xvel < (DOT_ACC) && m_xvel > -(DOT_ACC)) {
        m_xvel = 0;
        if (m_status != CHAR_JUMP && m_status != CHAR_INACTIVE && m_status != CHAR_EXITED) {
            m_status = CHAR_IDLE;
        }
    }

    // set direction (for animation)
    if (m_xvel < 0) {
        dir = DIR_LEFT;
    } else if (m_xvel > 0) {
        dir = DIR_RIGHT;
    }

    if (m_xvel != 0 && m_status != CHAR_JUMP && m_status != CHAR_INACTIVE && m_status != CHAR_EXITED) {
        m_status = CHAR_RUN;
    }

    std::vector<Tile> tileset = level->get_tileset();
    std::vector<Object*> objects = level->get_objects();
    Vector repos;

    bool pushed_left, pushed_right;
    pushed_left = pushed_right = false;

    int ext_xvel = 0;
    MovingPlatform* platform;
    if (on_moving_platform) {
        platform = (MovingPlatform*) objects[on_moving_platform_idx];
        int new_x = objects[on_moving_platform_idx]->get_rect().x;
        ext_xvel = new_x - on_platform_x;
    } else if (in_moving_platform) {
        platform = (MovingPlatform*) objects[in_moving_platform_idx];
        int new_x = objects[in_moving_platform_idx]->get_rect().x;
        ext_xvel = new_x - in_platform_x;
    }
    col_rect.x += ext_xvel;

    // move that Dot (x)
    col_rect.x += m_xvel;

    Crate* crate;
    ShiftBlock* shiftblock;
    for (int i = 0; i < objects.size(); i++) {
        switch (objects[i]->get_type())
        {
        case OBJECT_MOVING_PLATFORM:
            if (on_moving_platform || objects[i]->get_color() != m_color) {
                break;
            }
            platform = (MovingPlatform*) objects[i];
            if (check_collision(col_rect, objects[i]->get_rect(), &repos)) {
                col_rect.x += repos.x;
                if ((repos.x > 0 && m_xvel < 0) ||
                    (repos.x < 0 && m_xvel > 0)) {
                    m_xvel = 0;
                }
                if (repos.x < 0) {
                    pushed_left = true;
                } else if (repos.x > 0) {
                    pushed_right = true;
                }
            }
            break;
        case OBJECT_SHIFTBLOCK:
            shiftblock = (ShiftBlock*) objects[i];
            if (shiftblock->get_status() == !m_color + 1) {
                break;
            }
            if (check_collision(col_rect, objects[i]->get_rect(), &repos)) {
                col_rect.x += repos.x;
                if ((repos.x > 0 && m_xvel < 0) ||
                    (repos.x < 0 && m_xvel > 0)) {
                    m_xvel = 0;
                }
            }
            break;
        case OBJECT_CRATE:
            if (objects[i]->get_color() != m_color) {
                break;
            }
            if ((left && right) || (!left && !right)) {
                if (check_collision(col_rect, objects[i]->get_rect(), &repos)) {
                    col_rect.x += repos.x;
                    if ((repos.x > 0 && m_xvel < 0) ||
                        (repos.x < 0 && m_xvel > 0)) {
                        m_xvel = 0;
                    }
                }
            } else {
                crate = (Crate*) objects[i];
                crate->push(col_rect, other_player);
                if (check_collision(col_rect, objects[i]->get_rect(), &repos)) {
                    col_rect.x += repos.x;
                }
            }
            break;
        case OBJECT_XSPRING:
            if (check_collision(col_rect, objects[i]->get_rect(), &repos)) {
                col_rect.x += repos.x;
                if ((repos.x > 0 && m_xvel < 0) ||
                    (repos.x < 0 && m_xvel > 0)) {
                    m_xvel = 0;
                }
            }
            break;
        default:
            break;
        }
    }

    bool wall_col = false;
    if (in_moving_platform) {
        platform = (MovingPlatform*) objects[in_moving_platform_idx];
        walls = platform->get_walls(tileset);
        wall_col = true;
    }
    if (in_crate) {
        crate = (Crate*) objects[in_crate_idx];
        walls = crate->get_walls(tileset);
        wall_col = true;
    }

    if (wall_col) {
        for (int i = 0; i < walls.size(); i++) {
            if (walls[i].get_side() != INVISIBLE_WALL_LEFT && walls[i].get_side() != INVISIBLE_WALL_RIGHT) {
                continue;
            }
            if (check_collision(col_rect, walls[i].get_rect(), &repos)) {
                col_rect.x += repos.x;
                m_xvel = 0;
            }
        }
    }

    if (entering && (exit_dir == EXIT_LEFT || exit_dir == EXIT_RIGHT)) {
        int lvl_x = level->get_x();
        int lvl_y = level->get_y();
        int lvl_w = level->get_w()*TILE_WIDTH;
        int lvl_h = level->get_h()*TILE_WIDTH;
        if (col_rect.x > lvl_x && col_rect.x + col_rect.w < lvl_x + lvl_w &&
            col_rect.y > lvl_y && col_rect.y + col_rect.h < lvl_y + lvl_h) {
            entering = false;
            up = down = left = right = false;
        } else {
            return;
        }
    }

    // resolve x collisions
    int size = level->get_w()*level->get_h();
    for (int i = 0; i < size; i++) {
        if (wall_col) {
            break;
        }
        TileType type = tileset[i].get_type();
        if (type == TILE_BLACK_SOLID + !m_color || type == TILE_CLEAR || type == TILE_INVISIBLE) {
            continue;
        }
        if (check_collision(col_rect, tileset[i].get_rect(), &repos)) {
            if (tiletype_isslope(type)) {
                continue;
            }
            if (type == TILE_SLOPE_PAD_BLACK || type == TILE_SLOPE_PAD_WHITE) {
                continue;
            }
            if (type == TILE_SPIKES_LEFT || type == TILE_SPIKES_RIGHT) {
                m_status = CHAR_DIE;
                zone->reset_level();
                return;
            }
            if (type == TILE_BLACK_PLATFORM + m_color) {
                platform_drop = true;
                continue;
            }
            if (repos.x < 0 && pushed_right) {
                m_status = CHAR_DIE;
                zone->reset_level();
                return;
            }
            if (repos.x > 0 && pushed_left) {
                m_status = CHAR_DIE;
                zone->reset_level();
                return;
            }
            col_rect.x += repos.x;
            if ((repos.x > 0 && m_xvel < 0) ||
                (repos.x < 0 && m_xvel > 0)) {
                m_xvel = 0;
            }
        }
    }
}

void Dot::update_y(Zonestate* zone)
{
    if (exited) {
        return;
    }
    Level* level = zone->get_active_level();
    std::vector<Tile> tileset = level->get_tileset();
    std::vector<Object*> objects = level->get_objects();
    Vector repos;

    bool pushed_up, pushed_down;
    pushed_up = pushed_down = false;

    int ext_yvel = 0;
    MovingPlatform* platform;
    if (on_moving_platform) {
        platform = (MovingPlatform*) objects[on_moving_platform_idx];
        int new_y = objects[on_moving_platform_idx]->get_rect().y;
        ext_yvel = new_y - on_platform_y;
    } else if (in_moving_platform) {
        platform = (MovingPlatform*) objects[in_moving_platform_idx];
        int new_y = objects[in_moving_platform_idx]->get_rect().y;
        ext_yvel = new_y - in_platform_y;
    }

    if (m_status != CHAR_JUMP) {
        short_hop = -1;
    }

    true_y += ext_yvel;

    // update y velocity with gravity
    if (m_status == CHAR_JUMP) {
        int jump_duration = SDL_GetTicks() - jump_start;
        if (jump_duration > 100 && jump_duration < 150) {
            if (short_hop == -1) {
                short_hop = !up;
            }
        } else if (jump_duration >= 150) {
            short_hop = 0;
        }
    }

    if (exited) {
        return;
    }

    m_yvel += (!m_color - m_color) * GRAVITY * ((short_hop > 0)*4.2 + 1);

    if ((exited || entering || exiting) && (exit_dir == EXIT_LEFT || exit_dir == EXIT_RIGHT)) {
        m_yvel = 0;
    }

    // move that Dot (y)
    true_y += m_yvel;
    col_rect.y = (int) true_y + (m_color == 0);

    airborne = true;
    ShiftBlock* shiftblock;
    XSpring* xspring;
    SDL_Rect y_rect;
    for (int i = 0; i < objects.size(); i++) {
        switch (objects[i]->get_type())
        {
        case OBJECT_MOVING_PLATFORM:
            if (objects[i]->get_color() != m_color) {
                break;
            }
            if (check_collision(col_rect, objects[i]->get_rect(), &repos)) {
                if ((m_color == 0 && repos.y < 0) ||
                    (m_color == 1 && repos.y > 0)) {
                    // land!
                    if (m_status == CHAR_JUMP) {
                        m_status = CHAR_IDLE;
                    }
                    col_rect.y += repos.y;
                    true_y = col_rect.y;
                    m_yvel = 0;
                    shiftable = true;
                    // jump!
                    if (up) {
                        m_status = CHAR_JUMP;
                        jump_start = SDL_GetTicks();
                        m_yvel = (m_color - !m_color) * JUMP_VEL + ext_yvel / 1.5;
                        shiftable = false;
                    } else if (down) {
                        platform_drop = true;
                    } else {
                        airborne = false;
                    }
                } else {
                    // ceiling
                    if ((m_color == 0 && m_yvel < 0) || (m_color == 1 && m_yvel > 0)) {
                        // adjust y pos
                        col_rect.y += repos.y;
                        true_y = col_rect.y;
                        m_yvel = ((m_color == 0) - (m_color == 1)) * .1;
                        short_hop = 0;
                        shiftable = false;
                        up = false;
                    }
                }
                if (repos.y < 0) {
                    pushed_up = true;
                } else if (repos.y > 0) {
                    pushed_down = true;
                }
            }
            break;
        case OBJECT_SHIFTBLOCK:
            shiftblock = (ShiftBlock*) objects[i];
            if (shiftblock->get_status() == !m_color + 1) {
                break;
            }
            if (check_collision(col_rect, objects[i]->get_rect(), &repos)) {
                if ((m_color == 0 && repos.y < 0) ||
                    (m_color == 1 && repos.y > 0)) {
                    // land!
                    if (m_status == CHAR_JUMP) {
                        m_status = CHAR_IDLE;
                    }
                    col_rect.y += repos.y;
                    true_y = col_rect.y;
                    m_yvel = 0;
                    shiftable = true;
                    shiftblock->shift(m_color);
                    // jump!
                    if (up) {
                        m_status = CHAR_JUMP;
                        jump_start = SDL_GetTicks();
                        m_yvel = (m_color - !m_color) * JUMP_VEL + ext_yvel / 1.5;
                        shiftable = false;
                    } else if (down) {
                        platform_drop = true;
                    } else {
                        airborne = false;
                    }
                } else {
                    // ceiling
                    if ((m_color == 0 && m_yvel < 0) || (m_color == 1 && m_yvel > 0)) {
                        // adjust y pos
                        col_rect.y += repos.y;
                        true_y = col_rect.y;
                        m_yvel = ((m_color == 0) - (m_color == 1)) * .1;
                        short_hop = 0;
                        shiftable = false;
                        up = false;
                    }
                }
                if (repos.y < 0) {
                    pushed_up = true;
                } else if (repos.y > 0) {
                    pushed_down = true;
                }
            }
            break;
        case OBJECT_CRATE:
            if (objects[i]->get_color() != m_color) {
                break;
            }
            if (check_collision(col_rect, objects[i]->get_rect(), &repos)) {
                if ((m_color == 0 && repos.y < 0) ||
                    (m_color == 1 && repos.y > 0)) {
                    // land!
                    if (m_status == CHAR_JUMP) {
                        m_status = CHAR_IDLE;
                    }
                    col_rect.y += repos.y;
                    true_y = col_rect.y;
                    m_yvel = 0;
                    shiftable = true;
                    // jump!
                    if (up) {
                        m_status = CHAR_JUMP;
                        jump_start = SDL_GetTicks();
                        m_yvel = (m_color - !m_color) * JUMP_VEL + ext_yvel / 1.5;
                        shiftable = false;
                    } else if (down) {
                        platform_drop = true;
                    } else {
                        airborne = false;
                    }
                } else {
                    // ceiling
                    if ((m_color == 0 && m_yvel < 0) || (m_color == 1 && m_yvel > 0)) {
                        // adjust y pos
                        col_rect.y += repos.y;
                        true_y = col_rect.y;
                        m_yvel = ((m_color == 0) - (m_color == 1)) * .1;
                        short_hop = 0;
                        shiftable = false;
                        up = false;
                    }
                }
                if (repos.y < 0) {
                    pushed_up = true;
                } else if (repos.y > 0) {
                    pushed_down = true;
                }
            }
            break;
        case OBJECT_XSPRING:
            xspring = (XSpring*) objects[i];
            if (check_touching_vert(col_rect, xspring->get_land_rect())) {
                if ((m_color == 0 && m_yvel > 0) ||
                    (m_color == 1 && m_yvel < 0)) {
                    (m_color == 0) ? xspring->black_land(zone, this) : xspring->white_land(zone, this);
                }
            }
            if (check_collision(col_rect, objects[i]->get_rect(), &repos)) {
                if ((m_color == 0 && repos.y < 0) ||
                    (m_color == 1 && repos.y > 0)) {
                    // land!
                    if (m_status == CHAR_JUMP) {
                        m_status = CHAR_IDLE;
                    }
                    col_rect.y += repos.y;
                    true_y = col_rect.y;
                    m_yvel = 0;
                    shiftable = true;
                    // jump!
                    if (up) {
                        m_status = CHAR_JUMP;
                        jump_start = SDL_GetTicks();
                        m_yvel = (m_color - !m_color) * JUMP_VEL + ext_yvel / 1.5;
                        shiftable = false;
                    } else if (down) {
                        platform_drop = true;
                    } else {
                        airborne = false;
                    }
                } else {
                    // ceiling
                    if ((m_color == 0 && m_yvel < 0) || (m_color == 1 && m_yvel > 0)) {
                        // adjust y pos
                        col_rect.y += repos.y;
                        true_y = col_rect.y;
                        m_yvel = ((m_color == 0) - (m_color == 1)) * .1;
                        short_hop = 0;
                        shiftable = false;
                        up = false;
                    }
                }
            }
            break;
        default:
            break;
        }
    }

    bool walls_col = false;
    Crate* crate;
    if (in_moving_platform) {
        platform = (MovingPlatform*) objects[in_moving_platform_idx];
        walls = platform->get_walls(tileset);
        walls_col = true;
    } else if (in_crate) {
        crate = (Crate*) objects[in_crate_idx];
        walls = crate->get_walls(tileset);
        walls_col = true;
    }

    if (walls_col) {
        for (int i = 0; i < walls.size(); i++) {
            if (walls[i].get_side() != INVISIBLE_WALL_TOP && walls[i].get_side() != INVISIBLE_WALL_BOTTOM) {
                continue;
            }
            if (check_collision(col_rect, walls[i].get_rect(), &repos)) {
                if ((m_color == 0 && repos.y < 0) ||
                    (m_color == 1 && repos.y > 0)) {
                    // land!
                    if (m_status == CHAR_JUMP) {
                        m_status = CHAR_IDLE;
                    }
                    col_rect.y += repos.y;
                    true_y = col_rect.y;
                    m_yvel = 0;
                    shiftable = true;
                    // jump!
                    if (up) {
                        m_status = CHAR_JUMP;
                        jump_start = SDL_GetTicks();
                        m_yvel = (m_color - !m_color) * JUMP_VEL;
                        shiftable = false;
                    } else if (down) {
                        platform_drop = true;
                    } else {
                        airborne = false;
                    }
                } else {
                    // ceiling
                    if ((m_color == 0 && m_yvel < 0) || (m_color == 1 && m_yvel > 0)) {
                        // adjust y pos
                        col_rect.y += repos.y;
                        true_y = col_rect.y;
                        m_yvel = ((m_color == 0) - (m_color == 1)) * .1;
                        short_hop = 0;
                        shiftable = false;
                        up = false;
                    }
                }
            }
        }
    }

    // resolve y collisions
    bool platform_col = false;
    SDL_Rect tile_rect;
    int size = level->get_w()*level->get_h();
    // first pass for non-slope tiles
    for (int i = 0; i < size; i++) {
        if (walls_col) {
            break;
        }
        TileType type = tileset[i].get_type();
        if (type == TILE_BLACK_SOLID + !m_color || type == TILE_CLEAR || type == TILE_INVISIBLE) {
            continue;
        }
        if (type == TILE_SLOPE_PAD_BLACK && m_color == 1) {
            continue;
        }
        if (type == TILE_SLOPE_PAD_WHITE && m_color == 0) {
            continue;
        }
        tile_rect = tileset[i].get_rect();
        int point_x = col_rect.x + col_rect.w / 2;
        int point_y = col_rect.y + (m_color == 0)*col_rect.h;
        if (check_collision(col_rect, tile_rect, &repos)) {
            if (type == TILE_SPIKES_FLOOR || type == TILE_SPIKES_CEILING) {
                m_status = CHAR_DIE;
                zone->reset_level();
                return;
            }
            if (tiletype_isslope(type)) {
                continue;
            }
            if (type == TILE_BLACK_PLATFORM + m_color) {
                platform_col = true;
                if (platform_drop || (!m_color - m_color)*m_yvel < 0) {
                    continue;
                }
            }
            if ((type == TILE_SLOPE_PAD_BLACK && m_color == 0) ||
              (type == TILE_SLOPE_PAD_WHITE && m_color == 1)) {
                if (check_point_in_rect(point_x, point_y, m_color, tile_rect, &repos)) {
                    if (m_status == CHAR_JUMP) {
                        m_status = CHAR_IDLE;
                    }
                    col_rect.y += repos.y;
                    true_y = col_rect.y;
                    m_yvel = 0;
                    shiftable = true;
                    // jump!
                    if (up) {
                        m_status = CHAR_JUMP;
                        jump_start = SDL_GetTicks();
                        m_yvel = (m_color - !m_color) * JUMP_VEL;
                        shiftable = false;
                    } else {
                        airborne = false;
                    }
                }
                continue;
            }
            if ((m_color == 0 && repos.y < 0) ||
                (m_color == 1 && repos.y > 0)) {
                if (repos.y < 0 && pushed_down) {
                    m_status = CHAR_DIE;
                    zone->reset_level();
                    return;
                }
                if (repos.y > 0 && pushed_up) {
                    m_status = CHAR_DIE;
                    zone->reset_level();
                    return;
                }
                // land!
                if (m_status == CHAR_JUMP) {
                    m_status = CHAR_IDLE;
                }
                col_rect.y += repos.y;
                true_y = col_rect.y;
                m_yvel = 0;
                shiftable = true;
                // jump!
                if (up) {
                    m_status = CHAR_JUMP;
                    jump_start = SDL_GetTicks();
                    m_yvel = (m_color - !m_color) * JUMP_VEL;
                    shiftable = false;
                } else if (down) {
                    platform_drop = true;
                } else {
                    airborne = false;
                }
            } else {
                if (repos.y < 0 && pushed_down) {
                    m_status = CHAR_DIE;
                    zone->reset_level();
                    return;
                }
                if (repos.y > 0 && pushed_up) {
                    m_status = CHAR_DIE;
                    zone->reset_level();
                    return;
                }
                // ceiling
                if ((m_color == 0 && m_yvel < 0) || (m_color == 1 && m_yvel > 0)) {
                    check_for_platforms(zone);
                    if (walls_col) {
                        continue;
                    }
                    // adjust y pos
                    col_rect.y += repos.y;
                    true_y = col_rect.y;
                    m_yvel = ((m_color == 0) - (m_color == 1)) * .1;
                    short_hop = 0;
                    shiftable = false;
                    up = false;
                }
            }
            if (airborne && check_grounded(col_rect, tileset[i].get_rect(), m_color)) {
                airborne = false;
            }
        }
    }
    if (!platform_col) {
        platform_drop = false;
    }
    // second pass for slopes
    for (int i = 0; i < size; i++) {
        TileType type = tileset[i].get_type();
        SDL_Rect tile_rect = tileset[i].get_rect();
        int point_x = col_rect.x + col_rect.w / 2;
        int point_y = col_rect.y + (m_color == 0)*col_rect.h;
        if (check_collision(col_rect, tile_rect, &repos)) {
            if (tiletype_isslope(type)) {
                bool half_color = !(type == TILE_SLOPE_2_DOWN_A || type == TILE_SLOPE_2_UP_B);
                SDL_Rect tri_rect = {tile_rect.x, tile_rect.y, tile_rect.w, tile_rect.h / 2};
                tri_rect.y += (half_color == 1) * tile_rect.h / 2;
                bool down = (type == TILE_SLOPE_2_DOWN_A || type == TILE_SLOPE_2_DOWN_B);
                if (check_point_in_triangle(point_x, point_y, m_color, tri_rect, down, &repos)) {
                    if (m_status == CHAR_JUMP) {
                        m_status = CHAR_IDLE;
                    }
                    col_rect.y += repos.y;
                    true_y = col_rect.y;
                    m_yvel = 2*(!m_color - m_color);
                    shiftable = true;
                    // jump!
                    if (up) {
                        m_status = CHAR_JUMP;
                        jump_start = SDL_GetTicks();
                        m_yvel = (m_color - !m_color) * JUMP_VEL;
                        shiftable = false;
                    } else {
                        airborne = false;
                    }
                }
                continue;
            }
        }
    }
    if (on_moving_platform) {
        airborne = false;
    }
    if (walls_col) {
        SDL_Rect floor;
        InvisibleWallSide side;
        if (m_color == 0) {
            side = INVISIBLE_WALL_BOTTOM;
        } else {
            side = INVISIBLE_WALL_TOP;
        }
        for (int i = 0; i < walls.size(); i++) {
            if (walls[i].get_side() == side) {
                floor = walls[i].get_rect();
                break;
            }
        }
        if (check_grounded(col_rect, floor, m_color)) {
            airborne = false;
        }
    }
    if (airborne && m_status != CHAR_JUMP && m_status != CHAR_INACTIVE && m_status != CHAR_EXITED) {
        m_status = CHAR_JUMP;
        jump_start = 0;
        shiftable = false;
    }
    if (airborne) {
        shiftable = false;
    }

    Spring* spring;
    for (int i = 0; i < objects.size(); i++) {
        if (check_collision(col_rect, objects[i]->get_rect(), &repos)) {
            switch (objects[i]->get_type())
            {
            case OBJECT_SPRING:
                spring = (Spring*) objects[i];
                spring->spring();
                spring_me(spring->get_yvel());
                break;
            default:
                break;
            }
        }
    }

    if (entering) {
        int lvl_x = level->get_x();
        int lvl_y = level->get_y();
        int lvl_w = level->get_w()*TILE_WIDTH;
        int lvl_h = level->get_h()*TILE_WIDTH;
        if (col_rect.x > lvl_x && col_rect.x + col_rect.w < lvl_x + lvl_w &&
            col_rect.y > lvl_y && col_rect.y + col_rect.h < lvl_y + lvl_h) {
            entering = false;
            up = down = left = right = false;
        } else {
            return;
        }
    }

    if (!exited && !exiting) {
        int lvl_x = level->get_x();
        int lvl_y = level->get_y();
        int lvl_w = level->get_w()*TILE_WIDTH;
        int lvl_h = level->get_h()*TILE_WIDTH;
        if (col_rect.x < lvl_x) {
            exit(EXIT_LEFT);
        } else if (col_rect.x + col_rect.w > lvl_x + lvl_w) {
            exit(EXIT_RIGHT);
        } else if (col_rect.y + col_rect.h < lvl_y) {
            exit(EXIT_UP);
            zone->check_exit();
        } else if (col_rect.y > lvl_y + lvl_h) {
            exit(EXIT_DOWN);
            zone->check_exit();
        }

        if (m_status != CHAR_INACTIVE) {
            zone->shiftable = shiftable;
        }
    }
}

void Dot::exit(ExitDir dir)
{
    exiting = true;
    exit_dir = dir;
    up = down = false;
    switch (exit_dir)
    {
    case EXIT_LEFT:
        left = true;
        right = false;
        break;
    case EXIT_RIGHT:
        right = true;
        left = false;
        break;
    case EXIT_UP:
    case EXIT_DOWN:
        exited = true;
        exiting = false;
        m_yvel = m_xvel = 0;
        left = right = false;
        break;
    }
}

void Dot::enter()
{
    m_status = CHAR_IDLE;
    exiting = exited = false;
    entering = true;
    switch (exit_dir)
    {
    case EXIT_LEFT:
        right = true;
        left = up = down = false;
        break;
    case EXIT_RIGHT:
        left = true;
        right = up = down = false;
        break;
    case EXIT_DOWN:
        m_yvel = (m_color == 0)*-5;
        break;
    case EXIT_UP:
        m_yvel = (m_color == 1)*5;
        break;
    }
}

void Dot::good_exit(Zonestate* zone)
{
    exiting = exited = entering = false;
    up = down = left = right = false;
    m_xvel = m_yvel = 0;
    m_status = CHAR_IDLE;
    if (exit_dir == EXIT_UP && m_color == 0) {
        m_status = CHAR_JUMP;
        m_yvel = -5;
    } else if (exit_dir == EXIT_DOWN && m_color == 1) {
        m_status = CHAR_JUMP;
        m_yvel = 5;
    }
    if (zone->active_color != m_color) {
        m_status = CHAR_INACTIVE;
    }
    return;
}

bool Dot::in_level(Level* lvl)
{
    int lvl_x = lvl->get_x();
    int lvl_y = lvl->get_y();
    int lvl_w = lvl->get_w()*TILE_WIDTH;
    int lvl_h = lvl->get_h()*TILE_WIDTH;
    return (col_rect.x + col_rect.w > lvl_x && col_rect.x < lvl_x + lvl_w &&
            col_rect.y + col_rect.h > lvl_y && col_rect.y < lvl_y + lvl_h);
}

void Dot::render(Camera* cam, Level* lvl)
{
    int animation_length;
    double animation_speed;
    switch (m_status) {
        case CHAR_IDLE:
            animation_speed = 100.0;
            animation_length = 6;
            break;
        case CHAR_RUN:
            animation_speed = 30.0;
            animation_length = 12;
            break;
        case CHAR_JUMP:
            animation_speed = 100.0;
            animation_length = 5;
            break;
        case CHAR_INACTIVE:
            animation_speed = 150.0;
            animation_length = 10;
            break;
        case CHAR_EXITED:
            animation_speed = 150.0;
            animation_length = 10;
            break;
        default:
            animation_speed = 100;
            animation_length = 2;
            break;
    }

    int frame;
    if (m_status != CHAR_JUMP) {
        frame = ((int) ((float) SDL_GetTicks() / animation_speed)) % animation_length;
    } else {
        frame = ((int) (((float) SDL_GetTicks() - (float) jump_start)
                / animation_speed));
        if (frame >= animation_length) {
            frame = animation_length - 1;
        }
    }

    // relevant clips
    SDL_Rect frame_clip = {frame * 16, m_status * 16, 16, 16};
    int render_x = col_rect.x + (col_rect.w - m_sprite.get_width()) / 2;
    int render_y = col_rect.y + (m_color == 0) * (col_rect.h - m_sprite.get_height());
    float angle = 0.0;
    if (m_status == CHAR_EXITED) {
        int lvl_x = lvl->get_x();
        int lvl_y = lvl->get_y();
        int lvl_w = lvl->get_w()*TILE_WIDTH;
        int lvl_h = lvl->get_h()*TILE_WIDTH;
        if (exit_dir == EXIT_LEFT) {
            render_x = lvl_x;
        } else if (exit_dir == EXIT_RIGHT) {
            render_x = lvl_x + lvl_w - m_sprite.get_width();
        } else if (exit_dir == EXIT_UP) {
            render_y = lvl_y;
            angle = 90.0;
            dir = DIR_LEFT;
        } else if (exit_dir == EXIT_DOWN) {
            render_y = lvl_y + lvl_h - m_sprite.get_height();
            angle = 270.0;
            dir = DIR_LEFT;
        }
    }
    m_sprite.render(render_x, render_y, &frame_clip, cam, dir, m_color);
};

void Dot::save_state()
{
    saved_yvel = m_yvel;
    saved_col_rect = col_rect;
    saved_dir = dir;
    saved_status = m_status;
}

void Dot::reset(Zonestate* zone)
{
    up = down = right = left = false;
    exited = entering = exiting = false;
    m_xvel = 0;
    m_yvel = saved_yvel;
    m_status = saved_status;
    dir = saved_dir;
    col_rect = saved_col_rect;
    true_y = round(col_rect.y);
    on_moving_platform = false;
}

void Dot::spring_me(float yvel)
{
    jump_start = SDL_GetTicks();
    m_yvel = yvel;
}
