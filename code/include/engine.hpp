
#ifndef engine_hpp
#define engine_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <iostream>
#include <fstream>

#include <sound.hpp>

// screen dimensions!
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720

class Gamestate;

class Engine
{
public:
    bool init();
    void cleanup();

    void change_state(Gamestate* state);
    void push_state(Gamestate* state);
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
    SoundPlayer* sound;

    std::ofstream save_file;
    std::ifstream save_reader;
    bool save(int level);
    int read_save();

    std::vector<Gamestate*> states;

private:
    bool running_flag;
};

class Gamestate
{
public:
    Gamestate() { };

    virtual void init(Engine* game) = 0;
    virtual void cleanup() = 0;

    virtual void handle_events(Engine* game) = 0;
    virtual void update(Engine* game) = 0;
    virtual void draw(Engine* game) = 0;

    void change_state(Engine* game, Gamestate* state)
    {
        game->change_state(state);
    }

    SDL_Color palette;
};

class Controller
{
public:
    Controller();
    ~Controller();

    SDL_Joystick* joystick;
    SDL_GameController* gamepad;
};

#endif /* engine_hpp */
