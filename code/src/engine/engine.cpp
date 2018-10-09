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
    last_time = SDL_GetTicks();
    num_frames = 0;

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
    SDL_Surface* icon_surface = IMG_Load("resources/textures/icon.png");
    SDL_SetWindowIcon(window, icon_surface);
    SDL_FreeSurface(icon_surface);
    gl_context = SDL_GL_CreateContext(window);
    rend = NULL;
    SDL_GL_SetSwapInterval(1);
    if(!gladLoadGL()) {
        printf("Something went wrong!\n");
        exit(-1);
    }
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

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

    load_resources();
}

void Engine::load_resources()
{
    // textures
    ResourceManager::load_texture("char-sheet-black.png", "black_player");
    ResourceManager::load_texture("char-sheet-white.png", "white_player");
    ResourceManager::load_texture("char-normal.png", "player_normal");
    ResourceManager::load_texture("cross-spring.png", "cross_spring");
    ResourceManager::load_texture("xspring-normal.png", "xspring_normal");
    ResourceManager::load_texture("black-spring.png", "black_spring");
    ResourceManager::load_texture("white-spring.png", "white_spring");
    ResourceManager::load_texture("small-lamp.png", "small_lamp");
    ResourceManager::load_texture("shiftblock-idle.png", "shiftblock_idle");
    ResourceManager::load_texture("shiftblock-active.png", "shiftblock_active");
    ResourceManager::load_texture("simple-normal.png", "normal");
    ResourceManager::load_texture("crate.png", "crate");
    ResourceManager::load_texture("crate-normal.png", "crate_normal");

    // shaders
    ResourceManager::load_shader("level.vs", "level.fs", "level");
    ResourceManager::load_shader("level.vs", "level-normal.fs",
                                 "level_normal");
    ResourceManager::load_shader("to-texture.vs", "to-texture.fs",
                                 "to_texture");
    ResourceManager::load_shader("level.vs", "light.fs", "light");
    ResourceManager::load_shader("post-process.vs", "invertor.fs",
                                 "invertor");
    ResourceManager::load_shader("post-process.vs", "display.fs", "display");
    ResourceManager::load_shader("vblur.vs", "blur.fs", "vblur");
    ResourceManager::load_shader("hblur.vs", "blur.fs", "hblur");
    ResourceManager::load_shader("post-process.vs", "bright-filter.fs",
                                 "bright_filter");
    ResourceManager::load_shader("post-process.vs", "combiner.fs",
                                 "combiner");
    ResourceManager::load_shader("post-process.vs", "ripple.fs", "ripple");
    ResourceManager::load_shader("post-process.vs", "heat.fs", "heat");
}

void Engine::cleanup()
{
    // clean up!
    free(sound);
    save_file.close();
    save_reader.close();
    Mix_CloseAudio();
    if (rend) {
        SDL_DestroyRenderer(rend);
    } else {
        SDL_GL_DeleteContext(gl_context);
    }
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
    Uint32 cur_time = SDL_GetTicks();
    num_frames++;
    if (cur_time > last_time + 1000) {
        printf("%f ms/frame\n", 1000.0f/(float)num_frames);
        num_frames = 0;
        last_time += 1000;
    }
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
