// using SDL and standard IO
#include <stdio.h>
#include <glad.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <SDL2/SDL_ttf.h>
#include <fstream>
#include <iostream>

// include headers
#include <engine.hpp>
#include <shader.hpp>

Engine* game = NULL;

Engine::Engine()
{
    running_flag = true;
    screen_width = SCREEN_WIDTH;
    screen_height = SCREEN_HEIGHT;

    // init SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    // init OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    TTF_Init();
    font = TTF_OpenFont("resources/fonts/slkscr.ttf", 24);
    printf("initialization successful!\n");

    // creates a window that we can (eventually) draw into
    window = SDL_CreateWindow("together.", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);
    if(!gladLoadGL()) {
        printf("Something went wrong!\n");
        exit(-1);
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    m_shader = new Shader("resources/shaders/shader.vs", "resources/shaders/shader.fs");

    // initialize audio
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 256 ) == -1)
    {
        printf("error initializing audio: %s\n", SDL_GetError());
        Mix_CloseAudio();
    }

    sound = new SoundPlayer;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // initialize PNG loading
    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(img_flags) & img_flags)) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
}

void Engine::cleanup()
{
    // clean up!
    free(sound);
    save_file.close();
    save_reader.close();
    Mix_CloseAudio();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Engine::change_state(Gamestate* state)
{
    if (!states.empty()) {
        states.back()->cleanup();
        states.pop_back();
    }

    states.push_back(state);
    states.back()->init();
}

void Engine::push_state(Gamestate* state)
{
    states.push_back(state);
    states.back()->init();
}

void Engine::pop_state()
{
    states.back()->cleanup();
    states.pop_back();
    if (states.size() == 0) {
        quit();
    }
}

void Engine::restart_state()
{
    states.back()->init();
}

void Engine::handle_events()
{
    states.back()->handle_events();
}

void Engine::update()
{
    states.back()->update();
}

void Engine::draw()
{
    states.back()->draw();
}

bool Engine::save(int level)
{
    bool success = true;

    save_file.open("resources/save/save_file.txt");
    save_file << level;
    save_file.close();

    if (!save_file) {
        printf("error saving!\n");
        success = false;
    }

    return success;
}

int Engine::read_save()
{
    int to_return = -1;

    save_reader.open("resources/save/save_file.txt");
    save_reader >> to_return;
    save_reader.close();

    if (!save_reader) {
        printf("error reading save!\n");
    }

    return to_return;
}
