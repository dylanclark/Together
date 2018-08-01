#ifndef menustate_hpp
#define menustate_hpp

#include <menu.hpp>
#include <engine.hpp>

class MainMenuState : public Menu
{
public:
    void init();
};

class NewGameMenuState : public Menu
{
public:
    void init();
};

class OptionsMenuState : public Menu
{
public:
    void init();
};

class LevelSelectMenuState : public Menu
{
public:
    void init();
};

class QuitMenuState : public Menu
{
public:
    void init();
};

class PauseMenuState : public Menu
{
public:
    void init();
};

#endif /* menustate_hpp */
