
#ifndef engine_hpp
#define engine_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <fstream>

#include <sound.hpp>

// screen dimensions!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

class gamestate;

class engine
{
public:
    bool init();
    void cleanup();

    void change_state(gamestate* state);
    void push_state(gamestate* state);
    void pop_state();
    void restart_state();

    void handle_events();
    void update();
    void draw();

    void resize();
    int screen_width;
    int screen_height;

    bool running() { return running_flag; }
    void quit() { running_flag = false; }

    SDL_Window* win;
    SDL_Renderer* rend;
    sound_player* sound;

    std::ofstream save_file;
    std::ifstream save_reader;
    bool save(int level);
    int read_save();

private:
    std::vector<gamestate*> states;
    bool running_flag = true;

};

class gamestate
{
public:
    virtual void init(engine* game) = 0;
    virtual void cleanup() = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;

    virtual void handle_events(engine* game) = 0;
    virtual void update(engine* game) = 0;
    virtual void draw(engine* game) = 0;

    void change_state(engine* game, gamestate* state)
    {
        game->change_state(state);
    }

    gamestate() { };
};

class controller
{
public:
    controller();
    ~controller();

    SDL_Joystick* joystick;
    SDL_GameController* gamepad;

    const int DEAD_ZONE = 8000;
};

#endif /* engine_hpp */
