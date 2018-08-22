
#ifndef engine_hpp
#define engine_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>

#include <sound.hpp>
#include <shader.hpp>
#include <textures.hpp>

// screen dimensions!
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720

/**************/
/*   ENGINE   */
/**************/

class Gamestate;

class Engine
{
public:
    Engine();
    void cleanup();

    void change_state(Gamestate* state);
    void push_state(Gamestate* state);
    void pop_state();
    void restart_state();

    void handle_events();
    void update();
    void draw();

    void resize();
    int screen_width, screen_height;

    bool running() { return running_flag; }
    void quit() { running_flag = false; }

    SDL_Window* window;
    SDL_GLContext gl_context;
    SDL_Renderer* rend;
    Shader* m_shader;

    SoundPlayer* sound;
    TTF_Font* font;

    std::ofstream save_file;
    std::ifstream save_reader;
    bool save(int level);
    int read_save();

    std::vector<Gamestate*> states;

private:
    void load_resources();
    bool running_flag;
    Uint32 last_time;
    int num_frames;
};

extern Engine* game;

/*****************/
/*   GAMESTATE   */
/*****************/

class Gamestate
{
public:
    Gamestate() { };

    virtual void init() = 0;
    virtual void cleanup() = 0;

    virtual void handle_events() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    void change_state(Gamestate* state)
    {
        game->change_state(state);
    }

    SDL_Color palette;
};

/************************/
/*   RESOURCE MANAGER   */
/************************/

class ResourceManager
{
public:
    static std::map<std::string, Shader>  shaders;
    static std::map<std::string, Texture> textures;
    static Shader load_shader(const GLchar* vertex_shader_file, const GLchar* frag_shader_file, std::string name);
    static Shader get_shader(std::string name);
    static Texture load_texture(const GLchar* file, std::string name);
    static Texture get_texture(std::string name);
private:
    ResourceManager() { };
};

/******************/
/*   CONTROLLER   */
/******************/

class Controller
{
public:
    Controller();
    ~Controller();

    SDL_Joystick* joystick;
    SDL_GameController* gamepad;
};

#endif /* engine_hpp */
