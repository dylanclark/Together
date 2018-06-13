// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
#include <levels.hpp>
#include <engine.hpp>

// camera constants
static const float BUFFER = .5;
static const float CAM_ACC = 0.05;

static SDL_Rect get_target(SDL_Rect char1, SDL_Rect char2, int level_w, int level_h)
{
    // these will be for our ideal camera position (x and y are centered)
    float x1, x2, y1, y2;

    // x pos between characters
    x1 = char1.x + char1.w / 2.0;
    x2 = char2.x + char2.w / 2.0;

    // y pos between characters
    y1 = char1.y + char1.h / 2.0;
    y2 = char2.y + char2.h / 2.0;

    // ideal camera position
    SDL_Rect target;
    target.x = (x1 + x2) / 2.0;
    target.y = (y1 + y2) / 2.0;
    target.w = abs(x1 - x2) * (1 + 2*BUFFER);
    target.h = abs(y1 - y2) * (1 + 2*BUFFER);

    // left bound
    if (target.x - target.w / 2 < 0) {
        int diff = -(target.x - target.w / 2);
        target.w -= diff;
        target.x += diff / 2;
    }
    // right bound
    if (target.x + target.w / 2 > level_w) {
        int diff = target.x + target.w / 2 - level_w;
        target.w -= diff;
        target.x -= diff / 2;
    }
    // top bound
    if (target.y - target.h / 2 < 0) {
        int diff = -(target.y - target.h / 2);
        target.h -= diff;
        target.y += diff / 2;
    }
    // bottom bound
    if (target.y + target.h / 2 > level_h) {
        int diff = target.y + target.h / 2 - level_h;
        target.h -= diff;
        target.y -= diff / 2;
    }

    return target;
}

Camera::Camera(int scr_w, int scr_h, int lev_w, int lev_h, SDL_Rect char1, SDL_Rect char2)
{
    // level / screen dimensions
    level_w = lev_w;
    level_h = lev_h;
    screen_w = scr_w;
    screen_h = scr_h;

    // we want to start by tracking the chars
    SDL_Rect target = get_target(char1, char2, level_w, level_h);

    // initialize rectangle
    loc_x = target.x;
    loc_y = target.y;
    loc_w = target.w;
    loc_h = target.h;

    // update the proper SDL_Rect
    display.x = loc_x - loc_w / 2.0;
    display.y = loc_y - loc_h / 2.0;
    display.w = loc_w;
    display.h = loc_h;
};

SDL_Rect*
Camera::get_display()
{
    return &display;
}

void Camera::update(SDL_Rect char1, SDL_Rect char2)
{
    SDL_Rect target = get_target(char1, char2, level_w, level_h);

    // move our display closer to our target
    loc_x += ((float) target.x - loc_x) * CAM_ACC;
    loc_y += ((float) target.y - loc_y) * CAM_ACC;
    loc_w += ((float) target.w - loc_w) * CAM_ACC;
    loc_h += ((float) target.h - loc_h) * CAM_ACC;

    // update the proper SDL_Rect
    display.x = loc_x - loc_w / 2.0;
    display.y = loc_y - loc_h / 2.0;
    display.w = loc_w;
    display.h = loc_h;
}
