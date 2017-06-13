
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header
#include <characters/collisions.hpp>
#include <levels/tiles.hpp>
#include <characters/characters.hpp>

vector::vector()
{
    x = 0;
    y = 0;
};

// general collision checker (thanks SDL tutorial for inspiration!)
bool check_collision(SDL_Rect a, SDL_Rect b, vector* repos)
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
        repos->x = 0;
        repos->y = 0;
        return false;
    }

    if(topA > bottomB)
    {
        repos->x = 0;
        repos->y = 0;
        return false;
    }

    if(rightA < leftB)
    {
        repos->x = 0;
        repos->y = 0;
        return false;
    }

    if(leftA > rightB)
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