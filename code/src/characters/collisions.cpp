
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header
#include <char.hpp>

Vector::Vector()
{
    x = 0;
    y = 0;
};

// general collision checker (thanks SDL tutorial for inspiration!)
bool check_collision(SDL_Rect a, SDL_Rect b, Vector* repos)
{
    // the sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    // calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    // Get the amount of overlap
    if(bottomA <= topB)
    {
        repos->x = 0;
        repos->y = 0;
        return false;
    }

    if(topA >= bottomB)
    {
        repos->x = 0;
        repos->y = 0;
        return false;
    }

    if(rightA <= leftB)
    {
        repos->x = 0;
        repos->y = 0;
        return false;
    }

    if(leftA >= rightB)
    {
        repos->x = 0;
        repos->y = 0;
        return false;
    }

    // if none of the sides from A are outside B
    repos->x = (leftA > leftB) ? (rightB - leftA) : (leftB - rightA);
    repos->y = (topA < topB) ? (topB - bottomA) : (bottomB - topA);

    return true;
}

bool check_point_in_triangle(int x, int y, bool color, SDL_Rect triangle, bool down, Vector* repos)
{
    if (x < triangle.x) {
        return false;
    }
    if (x >= triangle.x + triangle.w) {
        return false;
    }

    int x_rel = x - triangle.x;
    if (!down) {
        x_rel = triangle.w - x_rel;
    }

    if (color == false) {
        if (y < triangle.y + x_rel / (triangle.w / triangle.h)) {
            return false;
        }
        repos->x = 0;
        repos->y = triangle.y + x_rel / (triangle.w / triangle.h) - y;
        return true;
    } else {
        if (y > triangle.y + x_rel / (triangle.w / triangle.h) + 1) {
            return false;
        }
        repos->x = 0;
        repos->y = triangle.y + x_rel / (triangle.w / triangle.h) + 1 - y;
        return true;
    }
}

bool check_point_in_rect(int x, int y, bool color, SDL_Rect rect, Vector* repos)
{
    if (x < rect.x) {
        return false;
    }
    if (x > rect.x + rect.w) {
        return false;
    }
    if (y < rect.y) {
        return false;
    }
    if (y > rect.y + rect.h) {
        return false;
    }
    repos->x = 0;
    repos->y = (color == 0) ? rect.y - y : rect.y + rect.h - y;
    return true;
}

bool check_grounded(SDL_Rect a, SDL_Rect b, bool a_color)
{
    // the sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    // calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    // Get the amount of overlap
    if(bottomA < topB)
    {
        return false;
    }

    if(topA > bottomB)
    {
        return false;
    }

    if(rightA <= leftB)
    {
        return false;
    }

    if(leftA >= rightB)
    {
        return false;
    }

    return true;
}

// checks if b is fully inside a
bool check_full_overlap(SDL_Rect a, SDL_Rect b)
{
    // the sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    // calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if (topB < topA) {
        return false;
    }
    if (bottomB > bottomA) {
        return false;
    }
    if (rightB > rightA) {
        return false;
    }
    if (leftB < leftA) {
        return false;
    }
    return true;
}

// a is the player, b is the platform
bool check_in_platform(SDL_Rect a, SDL_Rect b)
{
    // the sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    // calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if (bottomA < topB) {
        return false;
    }

    if (topA > bottomB) {
        return false;
    }

    if (rightA < leftB) {
        return false;
    }

    if (leftA > rightB) {
        return false;
    }

    if (leftA >= leftB && rightA <= rightB) {
        return true;
    } else if (topA >= topB && bottomA <= bottomB) {
        return true;
    }
    return false;
}
