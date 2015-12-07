
#ifndef engine_hpp
#define engine_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <vector>
#include <SDL2_ttf/SDL_ttf.h>
#include <iostream>
#include <fstream>

// include headers
#include "sound.hpp"

class gamestate;

class engine
{
public:
    // game startup
    bool init();
    
    // game shutdown
    void cleanup();
    
    // gamestate stack functions
    void change_state(gamestate* state);
    void push_state(gamestate* state);
    void pop_state();
    void restart_state();
    
    // game operations
    void handle_events();
    void update();
    void draw();
    
    // resize window!
    void resize();
    int screen_width;
    int screen_height;
    
    // running / quit functions
    bool running() { return running_flag; }
    void quit() { running_flag = false; }
    
    // window and renderer
    SDL_Window* win;
    SDL_Renderer* rend;
    
    // audio player
    sound_player* sound;
    
    // save objects
    std::ofstream save_file;
    std::ifstream save_reader;
    
    // save functions
    bool save(int level);
    int read_save();
    
    // stack of gamestates
    std::vector<gamestate*> states;
    
private:
    
    // quit flag
    bool running_flag = true;
    
};

#endif /* engine_hpp */
