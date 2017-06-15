
#ifndef menu_hpp
#define menu_hpp


// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

// include headers
#include <textures/textures.hpp>
#include <engine.hpp>
#include <engine.hpp>

// maximum menu size
const int MAX_BUTTONS = 4;

class menu;

class fade_in
{
public:

    // init
    fade_in(int w, int h, int time);

    // dealloc
    ~fade_in();

    // rectangle
    SDL_Rect rect;

    // texture
    texture* tex;

    // transparency
    Uint8 alpha;

    // timer
    int timer;

    // render function
    void render(SDL_Renderer* rend);

    // update alpha value
    void update();
};

class menu_button
{
public:

    // init
    menu_button(bool selected, int x, int y, int w, int h);

    // dealloc
    ~menu_button();

    // selected bool
    bool selected;

    // position rectangle
    SDL_Rect rect;

    // appearance
    texture* tex;

    // render function
    void render(SDL_Renderer* rend);

    // selection action
    virtual void select(engine* game) = 0;
};

class menu_slider
{
public:

    // init
    menu_slider(bool selected, int length, bool permanent, int x, int y, int w, int h);

    // dealloc
    ~menu_slider();

    // selected bool
    bool selected;

    // total size
    int frames;

    // current frame
    int cur_frame;

    // permanent
    bool permanent;

    // position rectangle
    SDL_Rect rect;

    // appearance
    texture* tex;

    // render function
    void render(SDL_Renderer* rend);

    // selection action
    virtual void select(engine* game) = 0;
};

class title
{
public:

    // init

    title(int x, int y, int w, int h);

    ~title();

    SDL_Rect rect;

    texture* tex;

    void render(SDL_Renderer* rend);
};

class menu
{
public:

    // init
    menu(int w, int h, int timer);

    // dealloc
    ~menu();

    // selected menu item
    int selector;

    // controllers
    controller* controller;

    // controller bools
    bool up, down, left, right;
    bool up_up, down_up, left_up, right_up;

    void update(engine* game);

    // title
    title* menu_title;

    // array of buttons
    std::vector <menu_button*> buttons;
    std::vector <menu_slider*> sliders;

    // fade-in if necessary
    fade_in* fade_in = NULL;

    int size;

    // event handler
    bool handle_event(SDL_Event &e, engine* game);
};

#endif /* menu_hpp */
