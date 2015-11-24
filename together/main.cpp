/*
 *
 *      "together" is a game by Fraser Weist and Dylan Clark.
 *
 *      They're both pretty lame.
 *      But rock socks. 
 *      Doubt it.
 *
 */

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include all headers
#include "tiles.hpp"
#include "environment.hpp"
#include "initialization.hpp"
#include "characters.hpp"
#include "collisions.hpp"
#include "level_draw.hpp"
#include "close.hpp"
#include "textures.hpp"
#include "camera.hpp"

// window re-initialization
extern SDL_Window* win;

// renderer re-initialization
extern SDL_Renderer* rend;

// reinitialize textures
extern texture b_char_tex;
extern texture w_char_tex;

// Let's do this.
int main(void)
{
    // start up the window
    if (!init())
    {
        printf("Initialization unsuccessful!\n");
        return 1;
    }
    
    enum gamestate
    {
        main_menu,
        pause_menu,
        settings,
        new_game,
        load_game,
        running,
    };
    
    // clear render for initialization
    SDL_RenderClear(rend);
    
    // quit flag
    bool quit = false;
    
    // level pointer
    level* cur_level = nullptr;
    
    // define level 1
    level level_1;
    level_1.width = 44;
    level_1.height = 18;
    level_1.path = "levels/level_1.map";
    
    // point to level 1
    cur_level = &level_1;
    
    // event handler
    SDL_Event event;
    
    // level tiles
    tile* tile_set[(cur_level->width) * (cur_level->height)];
    
    // load all media
    if (!load_media(tile_set, cur_level->width, cur_level->height))
    {
        printf("Could not load media!\n");
        return 1;
    }
    
    // black dot!
    dot b_char;
    b_char.status = CHAR_ACTIVE;
    b_char.tex = b_char_tex;
    b_char.col_rect.x = 120;
    b_char.col_rect.y = 200;
    b_char.black = true;
    
    // white dot!
    dot w_char;
    w_char.status = CHAR_INACTIVE;
    w_char.tex = w_char_tex;
    w_char.col_rect.x = 120;
    w_char.col_rect.y = 600;
    w_char.black = false;
    
    // the camera!
    camera camera;
    
    // clear the window for good measure
    SDL_RenderClear(rend);
    
    // while the application is running
    while (!quit)
    {
        // handle those events, bruh
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            }
        
            // quit if he pressed escape
            if (!b_char.handle_event(event))
                quit = true;
            
            if (!w_char.handle_event(event))
                quit = true;
            
        }
        
        // clear the window
        SDL_RenderClear(rend);
        
        // move the square
        b_char.move(tile_set, cur_level->width, cur_level->height);
        w_char.move(tile_set, cur_level->width, cur_level->height);
        
        // track the player
        camera.track(&b_char.col_rect, &w_char.col_rect);
        
        // move that camera!
        camera.move(cur_level);
        
        // draw stuff to the screen!
        for (int i = 0; i < (cur_level->width) * (cur_level->height); i++)
        {
            tile_set[i]->render(b_char.status, &camera.display);
        }
        
        b_char.render(&camera.display);
        w_char.render(&camera.display);
        SDL_RenderPresent(rend);
    }
    
    // close the window
    close(rend, win, tile_set, cur_level->width, cur_level->height);
}