
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

class Menu;

class FadeIn
{
public:

    FadeIn(int w, int h, int time);
    ~FadeIn();

    SDL_Rect rect;
    Texture* tex;
    Uint8 alpha;

    int timer;

    void render(SDL_Renderer* rend);
    void update();
};

class MenuButton
{
public:
    MenuButton(bool selected, int x, int y, int w, int h);
    ~MenuButton();

    bool selected;
    SDL_Rect rect;
    Texture* tex;

    void render(SDL_Renderer* rend);
    virtual void select(Engine* game) = 0;
};

class MenuSlider
{
public:
    MenuSlider(bool selected, int length, bool permanent, int x, int y, int w, int h);
    ~MenuSlider();

    bool selected;
    int frames;
    int cur_frame;
    bool permanent;

    SDL_Rect rect;
    Texture* tex;

    void render(SDL_Renderer* rend);
    virtual void select(Engine* game) = 0;
};

class Title
{
public:
    Title(int x, int y, int w, int h);
    ~Title();

    SDL_Rect rect;
    Texture* tex;

    void render(SDL_Renderer* rend);
};

class Menu
{
public:
    Menu(int w, int h, int timer);
    ~Menu();

    int selector;
    Controller* controller;

    bool up, down, left, right;
    bool up_up, down_up, left_up, right_up;

    void update(Engine* game);

    Title* menu_title;

    std::vector <MenuButton*> buttons;
    std::vector <MenuSlider*> sliders;

    FadeIn* fade_in;
    int size;

    bool handle_event(SDL_Event &e, Engine* game);
};

class NewGameButton : public MenuButton
{
public:
    NewGameButton(bool selected, int x, int y, int w, int h) : MenuButton(selected,x,y,w,h) { };
    void select(Engine* game);
};

class ContinueButton : public MenuButton
{
public:
    ContinueButton(bool selected, int x, int y, int w, int h) : MenuButton(selected,x,y,w,h) { };
    void select(Engine* game);
};

class OptionsButton : public MenuButton
{
public:
    OptionsButton(bool selected, int x, int y, int w, int h) : MenuButton(selected,x,y,w,h) { };
    void select(Engine* game);
};

class QuitButton : public MenuButton
{
public:
    QuitButton(bool selected, int x, int y, int w, int h) : MenuButton(selected,x,y,w,h) { };
    void select(Engine* game);
};

class YesQuitButton : public MenuButton
{
public:
    YesQuitButton(bool selected, int x, int y, int w, int h) : MenuButton(selected,x,y,w,h) { };
    void select(Engine* game);
};

class YesNewGameButton : public MenuButton
{
public:
    YesNewGameButton(bool selected, int x, int y, int w, int h) : MenuButton(selected,x,y,w,h) { };
    void select(Engine* game);
};

class NoButton : public MenuButton
{
public:
    NoButton(bool selected, int x, int y, int w, int h) : MenuButton(selected,x,y,w,h) { };
    void select(Engine* game);
};

class ResumeButton : public MenuButton
{
public:
    ResumeButton(bool selected, int x, int y, int w, int h) : MenuButton(selected,x,y,w,h) { };
    void select(Engine* game);
};

class LevelSelectButton : public MenuButton
{
public:
    LevelSelectButton(bool selected, int x, int y, int w, int h) : MenuButton(selected,x,y,w,h) { };
    void select(Engine* game);
};

class SaveAndQuitButton : public MenuButton
{
public:
    SaveAndQuitButton(bool selected, int x, int y, int w, int h) : MenuButton(selected,x,y,w,h) { };
    void select(Engine* game);
};

class ControlsButton : public MenuButton
{
public:
    ControlsButton(bool selected, int x, int y, int w, int h) : MenuButton(selected,x,y,w,h) { };
    void select(Engine* game);
};

class BackButton : public MenuButton
{
public:
    BackButton(bool selected, int x, int y, int w, int h) : MenuButton(selected,x,y,w,h) { };
    void select(Engine* game);
};

// SLIDERS

class VolumeSlider : public MenuSlider
{
public:
    VolumeSlider(bool selected, int length, bool permanent, int x, int y, int w, int h);
    void select(Engine* game);
};

class SfxSlider : public MenuSlider
{
public:
    SfxSlider(bool selected, int length, bool permanent, int x, int y, int w, int h);
    void select(Engine* game);
};

class LevelSlider : public MenuSlider
{
public:
    LevelSlider(bool selected, int length, bool permanent, int x, int y, int w, int h) : MenuSlider(selected, length, permanent, x, y, w, h) { };
    void select(Engine* game);
};

#endif /* menu_hpp */
