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

static SDL_Rect get_target(SDL_Rect active_char, int dir, int level_w, int level_h, int w, int h)
{
    int x = active_char.x + active_char.w / 2;
    int y = active_char.y + active_char.h / 2;

    SDL_Rect target;
    target.x = x + BUFFER * (!dir - dir);
    target.y = y;
    target.w = w;
    target.h = h;
    /*
    // left bound
    if (target.x - target.w / 2 < 0) {
        int diff = -(target.x - target.w / 2);
        target.x += diff;
    }
    // right bound
    if (target.x + target.w / 2 > level_w) {
        int diff = target.x + target.w / 2 - level_w;
        target.x -= diff;
    }
    // top bound
    if (target.y - target.h / 2 < 0) {
        int diff = -(target.y - target.h / 2);
        target.y += diff;
    }
    // bottom bound
    if (target.y + target.h / 2 > level_h) {
        int diff = target.y + target.h / 2 - level_h;
        target.y -= diff;
    }
    */

    return target;
}

Camera::Camera(int scr_w, int scr_h, Level* level, SDL_Rect active_char, int char_dir)
{
    // level dimensions
    level_w = level->get_w();
    level_h = level->get_h();

    // set width and height, they won't change
    display.w = scr_w / 2;
    display.h = scr_h / 2;

    // we want to start by tracking the chars
    SDL_Rect target = get_target(active_char, char_dir, level_w, level_h, display.w, display.h);

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
    level_w = lvl_w;
    level_h = lvl_h;

    // set width and height, they won't change
    display.w = scr_w / 2;
    display.h = scr_h / 2;

    // we want to start by tracking the chars
    SDL_Rect target = get_target(active_char, char_dir, level_w, level_h, display.w, display.h);

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
    SDL_Rect target = get_target(active_char, dir, level_w, level_h, display.w, display.h);

    // move our display closer to our target
    loc_x += ((float) target.x - loc_x) * CAM_ACC;
    loc_y += ((float) target.y - loc_y) * CAM_ACC;

    // update the proper SDL_Rect
    display.x = loc_x - display.w / 2.0;
    display.y = loc_y - display.h / 2.0;
}

void Camera::set_level(Level* level)
{
    level_w = level->get_w();
    level_h = level->get_h();
}
