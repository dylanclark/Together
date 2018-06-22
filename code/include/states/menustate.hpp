#ifndef menustate_hpp
#define menustate_hpp

#include <menu.hpp>
#include <engine.hpp>

class MainMenuState : public Menu
{
public:
    void init(Engine* game);
};

class NewGameMenuState : public Menu
{
public:
    void init(Engine* game);
};

class OptionsMenuState : public Menu
{
public:
    void init(Engine* game);
};

class LevelSelectMenuState : public Menu
{
public:
    void init(Engine* game);
};

class QuitMenuState : public Menu
{
public:
    void init(Engine* game);
};

class PauseMenuState : public Menu
{
public:
    void init(Engine* game);
};

#endif /* menustate_hpp */
