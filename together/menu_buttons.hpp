
#ifndef menu_buttons_hpp
#define menu_buttons_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// include button header
#include "menu.hpp"

class new_game_button : public menu_button
{
public:
    using menu_button::menu_button;
    
    void select(engine* game);
};

class continue_button : public menu_button
{
public:
    using menu_button::menu_button;
    
    void select(engine* game);
};

class options_button : public menu_button
{
    using menu_button::menu_button;
    
    void select(engine* game);
};

class quit_button : public menu_button
{
public:
    using menu_button::menu_button;
    
    void select(engine* game);
};

class yes_quit_button : public menu_button
{
public:
    using menu_button::menu_button;
    
    void select(engine* game);
};

class yes_newgame_button : public menu_button
{
public:
    using menu_button::menu_button;
    
    void select(engine* game);
};

class no_button : public menu_button
{
public:
    using menu_button::menu_button;
    
    void select(engine* game);
};

class resume_button : public menu_button
{
public:
    using menu_button::menu_button;
    
    void select(engine* game);
};

class level_select_button : public menu_button
{
public:
    using menu_button::menu_button;
    
    void select(engine* game);
};

class save_and_quit_button : public menu_button
{
public:
    using menu_button::menu_button;
    
    void select(engine* game);
};

class controls_button : public menu_button
{
public:
    using menu_button::menu_button;
    
    void select(engine* game);
};

class back_button : public menu_button
{
public:
    using menu_button::menu_button;
    
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
    using menu_slider::menu_slider;
    
    void select(engine* game);
};


#endif /* menu_buttons_hpp */
