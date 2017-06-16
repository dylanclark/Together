// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
#include <levels.hpp>
#include <engine.hpp>

Camera::Camera(int scr_w, int scr_h, int lev_w, int lev_h)
{
    // initialize velocities
    x_vel = 0;
    y_vel = 0;
    w_vel = 0;
    h_vel = 0;

    // initialize rectangle
    display.x = 0;
    display.y = 0;
    display.w = scr_w;
    display.h = scr_h;

    // level / screen dimensions
    level_w = lev_w;
    level_h = lev_h;
    screen_w = scr_w;
    screen_h = scr_h;
};

SDL_Rect*
Camera::get_display()
{
    return &display;
}

void Camera::move()
{
    // update positions
    location.x += x_vel;
    location.y += y_vel;
    location.w += w_vel;
    location.h += h_vel;

    // don't get too big (level dimensions)
    if (location.w > level_w) {
        location.w = level_w;
    }
    if (location.h > level_h) {
        location.h = level_h;
    }

    // keep correct aspect ratio
    if ((float) location.w / (float) screen_w > (float) location.h / (float) screen_h) {
        location.h = ((float) location.w / (float) screen_w) * screen_h;
    }
    if ((float) location.w / (float) screen_w < (float) location.h / (float) screen_h) {
        location.w = ((float) location.h / (float) screen_h) * screen_w;
    }

    if (location.w > level_w) {
        location.w = level_w;
        location.h = location.h = ((float) location.w / (float) screen_w) * screen_h;
    }
    if (location.h > level_h) {
        location.h = level_h;
        location.w = ((float) location.h / (float) screen_h) * screen_w;
    }

    // update the proper SDL_Rect
    display.x = location.x - location.w / 2.0;
    display.y = location.y - location.h / 2.0;
    display.w = location.w;
    display.h = location.h;

    // final corrections
    if (display.x < 0) {
        display.x = 0;
    }
    if (display.x + display.w > level_w) {
        display.x = level_w - display.w;
    }
    if (display.y < 0) {
        display.y = 0;
    }
    if (display.y + display.h > level_h) {
        display.y = level_h - display.h;
    }
}

void Camera::update(SDL_Rect* b_char, SDL_Rect* w_char)
{
    track(b_char, w_char);
    move();
}

void Camera::track(SDL_Rect* b_char, SDL_Rect* w_char)
{
    // these will be for our ideal camera position (x and y are centered)
    float min_x;
    float max_x;
    float min_y;
    float max_y;

    // x pos between characters
    if (b_char->x >= w_char->x) {
        max_x = b_char->x + b_char->w / 2.0;
        min_x = w_char->x + w_char->w / 2.0;
    }
    else {
        max_x = w_char->x + w_char->w / 2.0;
        min_x = b_char->x + b_char->w / 2.0;
    }

    // y pos between characters
    if (b_char->y >= w_char->y) {
        max_y = b_char->y + b_char->h / 2.0;
        min_y = w_char->y + w_char->h / 2.0;
    }
    else {
        max_y = w_char->y + w_char->h / 2.0;
        min_y = b_char->y + b_char->h / 2.0;
    }

    // x pos bounds
    if (max_x > level_w - BUFFER) {
        max_x = level_w - BUFFER;
    }
    if (min_x < BUFFER) {
        min_x = BUFFER;
    }

    // y pos bounds
    if (max_y > level_h - BUFFER) {
        max_y = level_h - BUFFER;
    }
    if (min_y < BUFFER) {
        min_y = BUFFER;
    }

    // ideal camera position
    SDL_Rect target_rect;
    target_rect.x = (min_x + max_x) / 2.0;
    target_rect.y = (min_y + max_y) / 2.0;
    target_rect.w = max_x - min_x + BUFFER * 2.0;
    target_rect.h = max_y - min_y + BUFFER * 2.0;

    // don't shrink too small
    if (target_rect.h < screen_h) {
        target_rect.h = screen_h;
    }
    if (target_rect.w < screen_w) {
        target_rect.w = screen_w;
    }

    // don't get too big (level dimensions)
    if (target_rect.w > level_w) {
        target_rect.w = level_w;
    }
    if (target_rect.h > level_h) {
        target_rect.h = level_h;
    }

    // x bounds
    if (target_rect.x - target_rect.w / 2.0 < 0.0) {
        target_rect.x = target_rect.w / 2.0;
    }
    if (target_rect.x + target_rect.w / 2.0 > level_w) {
        target_rect.x = (level_w) - target_rect.w / 2.0;
    }

    // y bounds
    if (target_rect.y - target_rect.h / 2.0 < 0.0) {
        target_rect.y = target_rect.h / 2.0;
    }
    if (target_rect.y + target_rect.h / 2.0 > level_h) {
        target_rect.y = (level_h) - target_rect.h / 2.0;
    }

    // update speeds!
    x_vel = CAM_ACC * ((float) target_rect.x - (float) location.x) / 20.0;
    y_vel = CAM_ACC * ((float) target_rect.y - (float) location.y) / 20.0;
    w_vel = CAM_ACC * ((float) target_rect.w - (float) location.w) / 20.0;
    h_vel = CAM_ACC * ((float) target_rect.h - (float) location.h) / 20.0;
}


