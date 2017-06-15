// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header
#include <textures.hpp>
#include <char.hpp>
#include <levels.hpp>

// reinitialize textures
extern texture level_1_start;
extern texture level_1_end;
extern texture level_2_start;
extern texture level_2_end;

level_message::level_message()
{

};

// level messagse class
void level_message::send_message(int type, engine* game)
{
    switch (type)
    {
        case START:
        {
            // initialize collision rectangle
            col_rect.w = 128;
            col_rect.h = 16;
            col_rect.y = 300;
            col_rect.x = 400;
            break;

        }
        case END:
        {
            // initialize collision rectangle
            col_rect.w = 128;
            col_rect.h = 16;
            col_rect.y = 300;
            col_rect.x = 1600;
            break;
        }
    }

}

// draw on screen
void level_message::render(SDL_Rect* camera, SDL_Renderer* rend)
{
    SDL_Rect active_clip = {0, 0, 128, 16};

    tex.render_message(&col_rect, &active_clip, rend);
}

