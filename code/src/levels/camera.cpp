// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
#include <levels.hpp>
#include <engine.hpp>

Camera::Camera(int scr_w, int scr_h, int lev_w, int lev_h, SDL_Rect char1, SDL_Rect char2)
{
    // initialize velocities
    x_vel = 0;
    y_vel = 0;
    w_vel = 0;
    h_vel = 0;

    // level / screen dimensions
    level_w = lev_w;
    level_h = lev_h;
    screen_w = scr_w;
    screen_h = scr_h;

    // we want to start by tracking the chars
    SDL_Rect target = get_target(char1, char2);

    // initialize rectangle
    location = target;

    // update the proper SDL_Rect
    display.x = location.x - location.w / 2.0;
    display.y = location.y - location.h / 2.0;
    display.w = location.w;
    display.h = location.h;
};

SDL_Rect*
Camera::get_display()
{
    return &display;
}

void Camera::update(SDL_Rect b_char, SDL_Rect w_char)
{
    track(b_char, w_char);
    move();
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

void Camera::track(SDL_Rect char1, SDL_Rect char2)
{
    SDL_Rect target = get_target(char1, char2);

    // distances to target
    float x_diff = ((float) target.x - (float) location.x);
    float y_diff = ((float) target.y - (float) location.y);
    float w_diff = ((float) target.w - (float) location.w);
    float h_diff = ((float) target.h - (float) location.h);

    // update speeds!
    x_vel += CAM_ACC * ((x_diff > 0) - (x_diff < 0));
    y_vel += CAM_ACC * ((y_diff > 0) - (y_diff < 0));
    w_vel += CAM_ACC * ((w_diff > 0) - (w_diff < 0));
    h_vel += CAM_ACC * ((h_diff > 0) - (h_diff < 0));

    // slow down when you get closer to your target
    if (x_vel > 0 && x_vel > x_diff / 15.0) {
        x_vel = x_diff / 15.0;
    }
    if (x_vel < 0 && x_vel < x_diff / 15.0) {
        x_vel = x_diff / 15.0;
    }
    if (y_vel > 0 && y_vel > y_diff / 15.0) {
        y_vel = y_diff / 15.0;
    }
    if (y_vel < 0 && y_vel < y_diff / 15.0) {
        y_vel = y_diff / 15.0;
    }
    if (w_vel > 0 && w_vel > w_diff / 15.0) {
        w_vel = w_diff / 15.0;
    }
    if (w_vel < 0 && w_vel < w_diff / 15.0) {
        w_vel = w_diff / 15.0;
    }
    if (h_vel > 0 && h_vel > h_diff / 15.0) {
        h_vel = h_diff / 15.0;
    }
    if (h_vel < 0 && h_vel < h_diff / 15.0) {
        h_vel = h_diff / 15.0;
    }

    // camera can't be too slow...
    if (x_vel < 1.0 && x_vel > -1.0) {
        x_vel = (x_vel > 0) - (x_vel < 0);
    }
    if (y_vel < 1.0 && y_vel > -1.0) {
        y_vel = (y_vel > 0) - (y_vel < 0);
    }
    if (w_vel < 1.0 && w_vel > -1.0) {
        w_vel = (w_vel > 0) - (w_vel < 0);
    }
    if (h_vel < 1.0 && h_vel > -1.0) {
        h_vel = (h_vel > 0) - (h_vel < 0);
    }

    // slow into the target
    if (x_diff < 1.0 && x_diff > -1.0) {
        x_vel = x_diff;
    }
    if (y_diff < 1.0 && y_diff > -1.0) {
        y_vel = y_diff;
    }
    if (w_diff < 1.0 && w_diff > -1.0) {
        w_vel = w_diff;
    }
    if (h_diff < 1.0 && h_diff > -1.0) {
        h_vel = h_diff;
    }
}

SDL_Rect Camera::get_target(SDL_Rect char1, SDL_Rect char2)
{
    // these will be for our ideal camera position (x and y are centered)
    float min_x;
    float max_x;
    float min_y;
    float max_y;

    // x pos between characters
    if (char1.x >= char2.x) {
        max_x = char1.x + char1.w / 2.0;
        min_x = char2.x + char2.w / 2.0;
    }
    else {
        max_x = char2.x + char2.w / 2.0;
        min_x = char1.x + char1.w / 2.0;
    }

    // y pos between characters
    if (char1.y >= char2.y) {
        max_y = char1.y + char1.h / 2.0;
        min_y = char2.y + char2.h / 2.0;
    }
    else {
        max_y = char2.y + char2.h / 2.0;
        min_y = char1.y + char1.h / 2.0;
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

    return target_rect;
}
