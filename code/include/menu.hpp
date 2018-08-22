
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

// types of menu items
typedef enum MenuItemType {
    MENU_BUTTON,
    MENU_SLIDER
} MenuItemType;

class FadeIn
{
public:
    FadeIn(int time);

    SDL_Rect rect;
    OldTexture tex;
    Uint8 alpha;
    int timer;

    void render();
    void update();
};

class MenuItem
{
public:
    MenuItemType type;
    bool selected;
    SDL_Rect rect;
    OldTexture tex;

    virtual void render() = 0;
    virtual void select() = 0;
};

class MenuButton : public MenuItem
{
public:
    MenuButton(int x, int y, int w, int h);

    void render();
    virtual void select() = 0;
};

class MenuSlider : public MenuItem
{
public:
    MenuSlider(int length, bool permanent, int x, int y, int w, int h);

    void render();
    virtual void select() = 0;

    int frames;
    int cur_frame;
    bool permanent;
};

class Title
{
public:
    Title(int x, int y, int w, int h);
    ~Title();

    SDL_Rect rect;
    OldTexture tex;

    void render();
};

class Menu : public Gamestate
{
public:
    Menu();

    // init and shutdown
    virtual void init() = 0;
    void cleanup();

    // pause and resume
    void pause();
    void resume();

    // handling control of screen
    void handle_events();
    void update();
    void draw();

    // controller stuff
    int selector;
    Controller* controller;
    bool up, down, left, right;

    // menu elements
    Title* title;
    std::vector <MenuItem*> items;
    FadeIn* fade_in;
};

class NewGameButton : public MenuButton
{
public:
    NewGameButton(int x, int y, int w, int h);
    void select();
};

class ContinueButton : public MenuButton
{
public:
    ContinueButton(int x, int y, int w, int h);
    void select();
};

class OptionsButton : public MenuButton
{
public:
    OptionsButton(int x, int y, int w, int h);
    void select();
};

class QuitButton : public MenuButton
{
public:
    QuitButton(int x, int y, int w, int h);
    void select();
};

class YesQuitButton : public MenuButton
{
public:
    YesQuitButton(int x, int y, int w, int h);
    void select();
};

class YesNewGameButton : public MenuButton
{
public:
    YesNewGameButton(int x, int y, int w, int h);
    void select();
};

class NoButton : public MenuButton
{
public:
    NoButton(int x, int y, int w, int h);
    void select();
};

class ResumeButton : public MenuButton
{
public:
    ResumeButton(int x, int y, int w, int h);
    void select();
};

class LevelSelectButton : public MenuButton
{
public:
    LevelSelectButton(int x, int y, int w, int h);
    void select();
};

class SaveAndQuitButton : public MenuButton
{
public:
    SaveAndQuitButton(int x, int y, int w, int h);
    void select();
};

class ControlsButton : public MenuButton
{
public:
    ControlsButton(int x, int y, int w, int h);
    void select();
};

class BackButton : public MenuButton
{
public:
    BackButton(int x, int y, int w, int h);
    void select();
};

// SLIDERS

class VolumeSlider : public MenuSlider
{
public:
    VolumeSlider(int length, bool permanent, int x, int y, int w, int h);
    void select();
};

class SfxSlider : public MenuSlider
{
public:
    SfxSlider(int length, bool permanent, int x, int y, int w, int h);
    void select();
};

class LevelSlider : public MenuSlider
{
public:
    LevelSlider(int length, bool permanent, int x, int y, int w, int h);
    void select();
};

#endif /* menu_hpp */
