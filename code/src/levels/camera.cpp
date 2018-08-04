// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
#include <levels.hpp>
#include <engine.hpp>

// camera constants
static const float BUFFER = 50;
static const float CAM_ACC = 0.05;

Camera::Camera(int scr_w, int scr_h, Level* level, SDL_Rect active_char, int char_dir)
{
    splining = false;

    // level dimensions
    m_lvl_x = level->get_x();
    m_lvl_y = level->get_y();
    m_lvl_w = level->get_w()*TILE_WIDTH;
    m_lvl_h = level->get_h()*TILE_WIDTH;

    // set width and height, they won't change
    display.w = scr_w / 2;
    display.h = scr_h / 2;

    // we want to start by tracking the chars
    SDL_Rect target = get_target(active_char, char_dir);

    // initialize rectangle
    loc_x = target.x;
    loc_y = target.y;

    // update the proper SDL_Rect
    display.x = loc_x - display.w / 2.0;
    display.y = loc_y - display.h / 2.0;
}

Camera::~Camera()
{

}

SDL_Rect Camera::get_target(SDL_Rect active_char, int dir)
{
    int x = active_char.x + active_char.w / 2;
    int y = active_char.y + active_char.h / 2;

    SDL_Rect target;
    target.x = x + BUFFER * (!dir - dir);
    target.y = y;
    target.w = display.w;
    target.h = display.h;

    // left bound
    if (target.x - target.w / 2 < m_lvl_x) {
        target.x = m_lvl_x + target.w / 2;
    }
    // right bound
    if (target.x + target.w / 2 > m_lvl_x + m_lvl_w) {
        target.x = m_lvl_x + m_lvl_w - target.w / 2;
    }
    // top bound
    if (target.y - target.h / 2 < m_lvl_y) {
        target.y = m_lvl_y + target.h / 2;
    }
    // bottom bound
    if (target.y + target.h / 2 > m_lvl_y + m_lvl_h) {
        target.y = m_lvl_y + m_lvl_h - target.h / 2;
    }

    return target;
}

void Camera::update(SDL_Rect active_char, int dir)
{
    if (spline_timestep == 0) {
        splining = false;
        loc_x = display.x + display.w / 2;
        loc_y = display.y + display.h / 2;
    }
    if (splining) {
        float s = (float) spline_timestep / (float) spline_duration;
        float t = 1 - s;
        loc_x = pow(s, 2) * (1 + 2*t) * spline_start.x + pow(t, 2) * (1 + 2*s) * spline_end.x;
        loc_y = pow(s, 2) * (1 + 2*t) * spline_start.y + pow(t, 2) * (1 + 2*s) * spline_end.y;
        display.x = loc_x - display.w / 2.0;
        display.y = loc_y - display.h / 2.0;
        spline_timestep--;
        return;
    }

    SDL_Rect target = get_target(active_char, dir);

    // move our display closer to our target
    loc_x += ((float) target.x - loc_x) * CAM_ACC;
    loc_y += ((float) target.y - loc_y) * CAM_ACC;

    // update the proper SDL_Rect
    display.x = loc_x - display.w / 2.0;
    display.y = loc_y - display.h / 2.0;
}

void Camera::set_level(Level* level, SDL_Rect active_char, int dir, int transition_duration)
{
    spline_start.x = (int) loc_x;
    spline_start.y = (int) loc_y;
    spline_start.w = display.w;
    spline_start.h = display.h;
    spline_duration = transition_duration;
    spline_timestep = spline_duration;
    m_lvl_x = level->get_x();
    m_lvl_y = level->get_y();
    m_lvl_w = level->get_w()*TILE_WIDTH;
    m_lvl_h = level->get_h()*TILE_WIDTH;
    spline_end = get_target(active_char, dir);
    splining = true;
}
