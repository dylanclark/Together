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
extern Texture level_1_start;
extern Texture level_1_end;
extern Texture level_2_start;
extern Texture level_2_end;

LevelMessage::LevelMessage()
{

};

// level messagse class
void LevelMessage::send_message(int type, Engine* game)
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
void LevelMessage::render(SDL_Rect* camera, SDL_Renderer* rend)
{
    SDL_Rect active_clip = {0, 0, 128, 16};

    tex.render_message(&col_rect, &active_clip, rend);
}

