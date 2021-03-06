
#include <objects.hpp>

/***********************/
/*   MOVING PLATFORM   */
/***********************/

InvisibleWall::InvisibleWall(int x, int y, int w, int h, InvisibleWallSide side)
{
    m_rect.x = x;
    m_rect.y = y;
    m_rect.w = w;
    m_rect.h = h;
    m_side = side;
}

MovingPlatform::MovingPlatform(int x1, int y1, int x2, int y2, int w, int h, bool color, bool automatic, int move_length, int pause_length, SDL_Color palette)
{
    m_type = OBJECT_MOVING_PLATFORM;
    m_has_light = false;
    m_status = PLATFORM_PAUSE_A;
    m_move_length = move_length;
    m_pause_length = pause_length;
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
    m_auto = automatic;
    m_timestep = 0;
    m_color = color;

    m_rect.x = x1;
    m_rect.y = y1;
    m_rect.w = w*TILE_WIDTH;
    m_rect.h = h*TILE_WIDTH;

    // m_tex.create_square(color, w, h, &palette);
}

void MovingPlatform::update_x(SDL_Rect black_player, SDL_Rect white_player)
{
    SDL_Rect player = (m_color == 0) ? white_player : black_player;
    m_timestep++;
    if (m_auto && (m_status == PLATFORM_PAUSE_A || m_status == PLATFORM_PAUSE_B) && (m_timestep >= m_pause_length)) {
        if (check_in_platform(player, m_rect) && !check_full_overlap(m_rect, player)) {
            m_timestep--;
        } else {
            m_status = (PlatformStatus) (((int) m_status + 1) % 4);
            m_timestep = 0;
        }
    }
    if ((m_status == PLATFORM_MOVETO_A || m_status == PLATFORM_MOVETO_B) && (m_timestep >= m_move_length)) {
        if (check_in_platform(player, m_rect) && !check_full_overlap(m_rect, player)) {
            m_timestep--;
        } else {
            m_status = (PlatformStatus) (((int) m_status + 1) % 4);
            m_timestep = 0;
        }
    }
    if (m_status == PLATFORM_PAUSE_A) {
        m_rect.x = m_x1;
    } else if (m_status == PLATFORM_PAUSE_B) {
        m_rect.x = m_x2;
    } else {
        float t = (float) m_timestep / (float) m_move_length;
        if (m_status == PLATFORM_MOVETO_A) {
            t = 1 - t;
        }
        float s = 1 - t;
        m_rect.x = round(pow(s, 2.0) * (1.0 + 2.0*t) * (float) m_x1 + pow(t, 2.0) * (1.0 + 2.0*s) * (float) m_x2);
    }
}

void MovingPlatform::update_y()
{
    if (m_status == PLATFORM_PAUSE_A) {
        m_rect.y = m_y1;
    } else if (m_status == PLATFORM_PAUSE_B) {
        m_rect.y = m_y2;
    } else {
        float t = (float) m_timestep / (float) m_move_length;
        if (m_status == PLATFORM_MOVETO_A) {
            t = 1 - t;
        }
        float s = 1 - t;
        m_rect.y = round(pow(s, 2.0) * (1.0 + 2.0*t) * (float) m_y1 + pow(t, 2.0) * (1.0 + 2.0*s) * (float) m_y2);
    }
}

void MovingPlatform::render_bg(Camera* cam, std::vector<Light> lights, bool active_color)
{
    m_sprite.render(m_rect.x, m_rect.y, NULL, cam, lights);
}

void MovingPlatform::trigger()
{
    m_status = PLATFORM_MOVETO_B;
}

void MovingPlatform::untrigger()
{
    m_status = PLATFORM_MOVETO_A;
}

std::vector<InvisibleWall> MovingPlatform::get_walls(std::vector<Tile> &tiles)
{
    std::vector<InvisibleWall> result;
    int platform_w = TILE_WIDTH;
    int platform_h = TILE_WIDTH;
    InvisibleWall left_wall(m_rect.x - platform_w, m_rect.y - platform_h, platform_w, m_rect.h + 2*platform_h, INVISIBLE_WALL_LEFT);
    InvisibleWall right_wall(m_rect.x + m_rect.w, m_rect.y - platform_h, platform_w, m_rect.h + 2*platform_h, INVISIBLE_WALL_RIGHT);
    InvisibleWall top_wall(m_rect.x - platform_w, m_rect.y - platform_h, m_rect.w + 2*platform_w, platform_h, INVISIBLE_WALL_TOP);
    InvisibleWall bottom_wall(m_rect.x - platform_w, m_rect.y + m_rect.h, m_rect.w + 2*platform_w, platform_h, INVISIBLE_WALL_BOTTOM);

    bool left_flag, right_flag, top_flag, bottom_flag;
    left_flag = right_flag = top_flag = bottom_flag = true;

    for (int i = 0; i < tiles.size(); i++) {
        if (m_color == 0 && tiles[i].get_type() != TILE_BLACK_SOLID) {
            continue;
        }
        if (m_color == 1 && tiles[i].get_type() != TILE_WHITE_SOLID) {
            continue;
        }
        SDL_Rect left_rect = left_wall.get_rect();
        left_rect.y += platform_h;
        left_rect.h -= platform_h*2;
        if (check_full_overlap(left_rect, tiles[i].get_rect())) {
            left_flag = false;
        }
        SDL_Rect right_rect = right_wall.get_rect();
        right_rect.y += platform_h;
        right_rect.h -= platform_h*2;
        if (check_full_overlap(right_rect, tiles[i].get_rect())) {
            right_flag = false;
        }
        SDL_Rect top_rect = top_wall.get_rect();
        top_rect.x += platform_w;
        top_rect.w -= platform_w*2;
        if (check_full_overlap(top_rect, tiles[i].get_rect())) {
            top_flag = false;
        }
        SDL_Rect bottom_rect = bottom_wall.get_rect();
        bottom_rect.x += platform_w;
        bottom_rect.w -= platform_w*2;
        if (check_full_overlap(bottom_rect, tiles[i].get_rect())) {
            bottom_flag = false;
        }
    }

    if (left_flag) {
        result.push_back(left_wall);
    }
    if (right_flag) {
        result.push_back(right_wall);
    }
    if (top_flag) {
        result.push_back(top_wall);
    }
    if (bottom_flag) {
        result.push_back(bottom_wall);
    }
    return result;
}


