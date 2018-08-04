
#ifndef object_hpp
#define object_hpp

// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
#include <textures.hpp>
#include <char.hpp>
#include <engine.hpp>
#include <levels.hpp>

typedef enum _ObjectType {
    OBJECT_SPRING = 0,
    OBJECT_BLOCK,
    OBJECT_BUTTON,
    OBJECT_KEY,
    OBJECT_DOOR,
} ObjectType;

class Object
{
public:
    Object() { };

    virtual void render(SDL_Rect cam_rect, bool active_color) = 0;

    ObjectType get_type() { return m_type; }
    SDL_Rect get_rect() { return m_rect; }

    SDL_Rect m_rect;
    ObjectType m_type;
    Texture m_tex;
    bool m_color;
};

typedef enum _SpringStatus {
    SPRING_IDLE,
    SPRING_SPRUNG,
    SPRING_INACTIVE
} SpringStatus;

class Spring : public Object
{
public:
    Spring(int x, int y, bool color, float y_vel, SDL_Color palette);
    void render(SDL_Rect cam_rect, bool active_color);
    float get_yvel() { return m_yvel; }
    void spring();

private:
    SpringStatus m_status;
    float m_yvel;
    int spring_start;
};

class Block : public Object
{
public:
    Block(int x, int y, int w, int h, TileType blocktype, Level* lvl, SDL_Color palette);
    ~Block();
    void render(SDL_Rect cam_rect, bool active_color) { }

private:
    Level* m_lvl;
    TileType m_tiletype;
    std::vector<TileType> saved_tiles;
};

/*
typedef enum _ButtonType {
    BUTTON_CREATE = 0,
    BUTTON_TOGGLE_ONOFF,
    BUTTON_TOGGLE_THISTHAT,
} ButtonType;

class ButtonCreateArgs
{
    std::vector<Object*> objs;
};

class ButtonToggleOnOffArgs
{
    std::vector<Object*> objs;
};

class ButtonToggleThisThatArgs
{
    std::vector<Object*> this_objs;
    std::vector<Object*> that_objs;
};

typedef union _ButtonArgs {
    ButtonCreateArgs create_args;
    ButtonToggleOnOffArgs onoff_args;
    ButtonToggleThisThatArgs thisthat_args;
} ButtonArgs;

class Button : public Object
{
public:
    Button(int x, int y, bool color, ButtonType buttontype);
    void push(std::vector<Object*> lvl_objs);
    void unpush(std::vector<Object*> lvl_objs);

private:
    ButtonType m_buttontype;
    ButtonArgs m_buttonargs;
    std::vector<int> objs_idx;
    bool done;
};

class Key : public Object
{
};

class Door : public Object
{
};
*/

#endif /* object_hpp */
