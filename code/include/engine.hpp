
#ifndef engine_hpp
#define engine_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <fstream>

// include headers
#include <sound/sound.hpp>

// screen dimensions!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

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

class gamestate
{
public:
    // init and cleanup
    virtual void init(engine* game) = 0;
    virtual void cleanup() = 0;

    // pause and resume
    virtual void pause() = 0;
    virtual void resume() = 0;

    // handling control of screen
    virtual void handle_events(engine* game) = 0;
    virtual void update(engine* game) = 0;
    virtual void draw(engine* game) = 0;

    // changing states
    void change_state(engine* game, gamestate* state)
    {
        game->change_state(state);
    }

    // create class
    gamestate() { };
};

class controller
{
public:
    // init
    controller();

    // dealloc
    ~controller();

    // joystick
    SDL_Joystick* joystick;

    // gamepad
    SDL_GameController* gamepad;

    // dead zone constant
    const int DEAD_ZONE = 8000;
};

#endif /* engine_hpp */
