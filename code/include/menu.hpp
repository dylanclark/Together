
#ifndef menu_hpp
#define menu_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

// include headers
#include <textures.hpp>
#include <engine.hpp>

// maximum menu size
const int MAX_BUTTONS = 4;

class menu;

class fade_in
{
public:

    fade_in(int w, int h, int time);
    ~fade_in();

    SDL_Rect rect;
    texture* tex;
    Uint8 alpha;

    int timer;

    void render(SDL_Renderer* rend);
    void update();
};

class menu_button
{
public:
    menu_button(bool selected, int x, int y, int w, int h);
    ~menu_button();

    bool selected;
    SDL_Rect rect;
    texture* tex;

    void render(SDL_Renderer* rend);
    virtual void select(engine* game) = 0;
};

class menu_slider
{
public:
    menu_slider(bool selected, int length, bool permanent, int x, int y, int w, int h);
    ~menu_slider();

    bool selected;
    int frames;
    int cur_frame;
    bool permanent;

    SDL_Rect rect;
    texture* tex;

    void render(SDL_Renderer* rend);
    virtual void select(engine* game) = 0;
};

class title
{
public:
    title(int x, int y, int w, int h);
    ~title();

    SDL_Rect rect;
    texture* tex;

    void render(SDL_Renderer* rend);
};

class menu
{
public:
    menu(int w, int h, int timer);
    ~menu();

    int selector;
    controller* controller;

    bool up, down, left, right;
    bool up_up, down_up, left_up, right_up;

    void update(engine* game);

    title* menu_title;

    std::vector <menu_button*> buttons;
    std::vector <menu_slider*> sliders;

    fade_in* fade_in = NULL;
    int size;

    bool handle_event(SDL_Event &e, engine* game);
};

class new_game_button : public menu_button
{
public:
    new_game_button(bool selected, int x, int y, int w, int h) : menu_button(selected,x,y,w,h) { };
    void select(engine* game);
};

class continue_button : public menu_button
{
public:
    continue_button(bool selected, int x, int y, int w, int h) : menu_button(selected,x,y,w,h) { };
    void select(engine* game);
};

class options_button : public menu_button
{
public:
    options_button(bool selected, int x, int y, int w, int h) : menu_button(selected,x,y,w,h) { };
    void select(engine* game);
};

class quit_button : public menu_button
{
public:
    quit_button(bool selected, int x, int y, int w, int h) : menu_button(selected,x,y,w,h) { };
    void select(engine* game);
};

class yes_quit_button : public menu_button
{
public:
    yes_quit_button(bool selected, int x, int y, int w, int h) : menu_button(selected,x,y,w,h) { };
    void select(engine* game);
};

class yes_newgame_button : public menu_button
{
public:
    yes_newgame_button(bool selected, int x, int y, int w, int h) : menu_button(selected,x,y,w,h) { };
    void select(engine* game);
};

class no_button : public menu_button
{
public:
    no_button(bool selected, int x, int y, int w, int h) : menu_button(selected,x,y,w,h) { };
    void select(engine* game);
};

class resume_button : public menu_button
{
public:
    resume_button(bool selected, int x, int y, int w, int h) : menu_button(selected,x,y,w,h) { };
    void select(engine* game);
};

class level_select_button : public menu_button
{
public:
    level_select_button(bool selected, int x, int y, int w, int h) : menu_button(selected,x,y,w,h) { };
    void select(engine* game);
};

class save_and_quit_button : public menu_button
{
public:
    save_and_quit_button(bool selected, int x, int y, int w, int h) : menu_button(selected,x,y,w,h) { };
    void select(engine* game);
};

class controls_button : public menu_button
{
public:
    controls_button(bool selected, int x, int y, int w, int h) : menu_button(selected,x,y,w,h) { };
    void select(engine* game);
};

class back_button : public menu_button
{
public:
    back_button(bool selected, int x, int y, int w, int h) : menu_button(selected,x,y,w,h) { };
    void select(engine* game);
};

// SLIDERS

class volume_slider : public menu_slider
{
public:
    volume_slider(bool selected, int length, bool permanent, int x, int y, int w, int h);
    void select(engine* game);
};

class sfx_slider : public menu_slider
{
public:
    sfx_slider(bool selected, int length, bool permanent, int x, int y, int w, int h);
    void select(engine* game);
};

class level_slider : public menu_slider
{
public:
    level_slider(bool selected, int length, bool permanent, int x, int y, int w, int h) : menu_slider(selected, length, permanent, x, y, w, h) { };
    void select(engine* game);
};

#endif /* menu_hpp */
