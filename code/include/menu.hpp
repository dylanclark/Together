
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
    FadeIn(Engine* game, int time);

    SDL_Rect rect;
    Texture tex;
    Uint8 alpha;
    int timer;

    void render(SDL_Renderer* rend);
    void update();
};

class MenuItem
{
public:
    MenuItemType type;
    bool selected;
    SDL_Rect rect;
    Texture tex;

    virtual void render(SDL_Renderer* rend) = 0;
    virtual void select(Engine* game) = 0;
};

class MenuButton : public MenuItem
{
public:
    MenuButton(int x, int y, int w, int h);

    void render(SDL_Renderer* rend);
    virtual void select(Engine* game) = 0;
};

class MenuSlider : public MenuItem
{
public:
    MenuSlider(int length, bool permanent, int x, int y, int w, int h);

    void render(SDL_Renderer* rend);
    virtual void select(Engine* game) = 0;

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
    Texture tex;

    void render(SDL_Renderer* rend);
};

class Menu : public Gamestate
{
public:
    Menu();

    // init and shutdown
    virtual void init(Engine* game) = 0;
    void cleanup();

    // pause and resume
    void pause();
    void resume();

    // handling control of screen
    void handle_events(Engine* game);
    void update(Engine* game);
    void draw(Engine* game);

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
    NewGameButton(int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

class ContinueButton : public MenuButton
{
public:
    ContinueButton(int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

class OptionsButton : public MenuButton
{
public:
    OptionsButton(int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

class QuitButton : public MenuButton
{
public:
    QuitButton(int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

class YesQuitButton : public MenuButton
{
public:
    YesQuitButton(int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

class YesNewGameButton : public MenuButton
{
public:
    YesNewGameButton(int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

class NoButton : public MenuButton
{
public:
    NoButton(int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

class ResumeButton : public MenuButton
{
public:
    ResumeButton(int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

class LevelSelectButton : public MenuButton
{
public:
    LevelSelectButton(int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

class SaveAndQuitButton : public MenuButton
{
public:
    SaveAndQuitButton(int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

class ControlsButton : public MenuButton
{
public:
    ControlsButton(int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

class BackButton : public MenuButton
{
public:
    BackButton(int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

// SLIDERS

class VolumeSlider : public MenuSlider
{
public:
    VolumeSlider(int length, bool permanent, int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

class SfxSlider : public MenuSlider
{
public:
    SfxSlider(int length, bool permanent, int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

class LevelSlider : public MenuSlider
{
public:
    LevelSlider(int length, bool permanent, int x, int y, int w, int h, SDL_Renderer* rend);
    void select(Engine* game);
};

#endif /* menu_hpp */
