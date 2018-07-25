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

Camera::Camera(int scr_w, int scr_h, Level* level, SDL_Rect active_char, int char_dir)
{
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

Camera::Camera(int scr_w, int scr_h, int lvl_w, int lvl_h, SDL_Rect active_char, int char_dir)
{
    // level dimensions
    m_lvl_w = lvl_w;
    m_lvl_h = lvl_h;

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

SDL_Rect*
Camera::get_display()
{
    return &display;
}

void Camera::update(SDL_Rect active_char, int dir)
{
    SDL_Rect target = get_target(active_char, dir);

    // move our display closer to our target
    loc_x += ((float) target.x - loc_x) * CAM_ACC;
    loc_y += ((float) target.y - loc_y) * CAM_ACC;

    // update the proper SDL_Rect
    display.x = loc_x - display.w / 2.0;
    display.y = loc_y - display.h / 2.0;
}

void Camera::set_level(Level* level)
{
    m_lvl_x = level->get_x();
    m_lvl_y = level->get_y();
    m_lvl_w = level->get_w()*TILE_WIDTH;
    m_lvl_h = level->get_h()*TILE_WIDTH;
}
