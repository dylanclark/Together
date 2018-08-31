
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
#include <utils.hpp>
#include <engine.hpp>
#include <levels.hpp>

/*************/
/*   LIGHT   */
/*************/

class Light
{
public:
    Light() { }
    Light(int x, int y, float range, SDL_Color palette);

    float get_range();
    float get_strength();
    int get_x() { return m_x; }
    int get_y() { return m_y; }
    void turn_on();
    void turn_off();

private:
    // position
    bool on;
    int m_x, m_y;
    int m_start_time;
    float m_range;
    SDL_Color m_palette;
    // pulsing period
    int m_period;
};

/**************/
/*   OBJECT   */
/**************/

class Camera;

typedef enum _ObjectType {
    OBJECT_SPRING = 0,
    OBJECT_MOVING_PLATFORM,
    OBJECT_CRATE,
    OBJECT_SHIFTBLOCK,
    OBJECT_XSPRING,
    OBJECT_BUTTON,
    OBJECT_KEY,
    OBJECT_DOOR,
    OBJECT_SMALL_LAMP,
} ObjectType;

class Object
{
public:
    Object() { };

    virtual void render_bg(Camera* cam, std::vector<Light> lights, bool active_color) = 0;
    virtual void render_fg(Camera* cam, std::vector<Light> lights, bool active_color) = 0;
    virtual void update_x(SDL_Rect black_player, SDL_Rect white_player) = 0;
    virtual void update_y() = 0;
    void trigger();
    void untrigger();

    // getters
    ObjectType get_type() { return m_type; }
    virtual SDL_Rect get_rect() { return m_rect; }
    bool get_color() { return m_color; }

    // light methods
    bool has_light() { return m_has_light; }
    Light get_light() { return m_light; }
    void turn_on_light() { m_light.turn_on(); }
    void turn_off_light() { m_light.turn_off(); }

    SDL_Rect m_rect;
    ObjectType m_type;
    bool m_has_light;
    Light m_light;
    Sprite m_sprite;
    bool m_color;
};

/**************/
/*   SPRING   */
/**************/

typedef enum _SpringStatus {
    SPRING_IDLE,
    SPRING_SPRUNG,
    SPRING_INACTIVE
} SpringStatus;

class Spring : public Object
{
public:
    Spring(int x, int y, bool color, float y_vel, SDL_Color palette);
    void render_bg(Camera* cam, std::vector<Light> lights, bool active_color);
    void render_fg(Camera* cam, std::vector<Light> lights, bool active_color) { }
    void update_x(SDL_Rect black_player, SDL_Rect white_player) { }
    void update_y() { }
    float get_yvel() { return m_yvel; }
    void spring();

private:
    SpringStatus m_status;
    float m_yvel;
    int spring_start;
};

/***********************/
/*   MOVING PLATFORM   */
/***********************/

typedef enum _PlatformStatus {
    PLATFORM_PAUSE_A,
    PLATFORM_MOVETO_B,
    PLATFORM_PAUSE_B,
    PLATFORM_MOVETO_A,
} PlatformStatus;

typedef enum _InvisibleWallSide {
    INVISIBLE_WALL_LEFT,
    INVISIBLE_WALL_RIGHT,
    INVISIBLE_WALL_TOP,
    INVISIBLE_WALL_BOTTOM,
} InvisibleWallSide;

class InvisibleWall
{
public:
    InvisibleWall(int x, int y, int w, int h, InvisibleWallSide m_side);
    ~InvisibleWall() { }

    SDL_Rect get_rect() { return m_rect; }
    InvisibleWallSide get_side() { return m_side; }

private:
    SDL_Rect m_rect;
    InvisibleWallSide m_side;
};

class MovingPlatform : public Object
{
public:
    MovingPlatform(int x1, int y1, int x2, int y2, int w, int h, bool color, bool automatic, int move_length, int pause_length, SDL_Color palette);
    ~MovingPlatform() { }
    void update_x(SDL_Rect black_player, SDL_Rect white_player);
    void update_y();
    void render_bg(Camera* cam, std::vector<Light> lights, bool active_color);
    void render_fg(Camera* cam, std::vector<Light> lights, bool active_color) { }
    void trigger();
    void untrigger();

    std::vector<InvisibleWall> get_walls(std::vector<Tile> &tiles);

private:
    int m_x1, m_y1, m_x2, m_y2;
    int m_timestep;

    std::vector<Tile> m_tiles;
    PlatformStatus m_status;
    bool m_auto;
    int m_move_length;
    int m_pause_length;
};

/******************/
/*   SHIFTBLOCK   */
/******************/

typedef enum _ShiftBlockStatus {
    SHIFTBLOCK_IDLE = 0,
    SHIFTBLOCK_BLACK,
    SHIFTBLOCK_WHITE
} ShiftBlockStatus;

class ShiftBlock : public Object
{
public:
    ShiftBlock(int x, int y, int w, int h, SDL_Color palette);
    ~ShiftBlock() { }

    void render_bg(Camera* cam, std::vector<Light> lights, bool active_color);
    void render_fg(Camera* cam, std::vector<Light> lights, bool active_color);

    void trigger() { }
    void untrigger() { }
    void update_x(SDL_Rect black_player, SDL_Rect white_player);
    void update_y() { }

    ShiftBlockStatus get_status() { return m_status; }
    void unshift() { m_status = SHIFTBLOCK_IDLE; }
    void shift(bool color) { m_status = (ShiftBlockStatus) (SHIFTBLOCK_BLACK + color); }

private:
    ShiftBlockStatus m_status;
};

/*************/
/*   CRATE   */
/*************/

class Crate : public Object
{
public:
    Crate(int x, int y, int w, int h, bool color, SDL_Color palette);
    ~Crate() { }

    void render_bg(Camera* cam, std::vector<Light> lights, bool active_color);
    void render_fg(Camera* cam, std::vector<Light> lights, bool active_color) { }

    void trigger() { }
    void untrigger() { }
    void update_x(SDL_Rect black_player, SDL_Rect white_player);
    void update_y() { }

    std::vector<InvisibleWall> get_walls(std::vector<Tile> &tiles);

    void push(SDL_Rect player_pushing, SDL_Rect other_player);
    void unpush() { pushed = false; }

private:
    bool pushed;
};

/********************/
/*   CROSS SPRING   */
/********************/

typedef enum _XSpringStatus {
    XSPRING_IDLE = 0,
    XSPRING_WHITE_LAND,
    XSPRING_WHITE_ON,
    XSPRING_BLACK_LAND,
    XSPRING_BLACK_ON,
    XSPRING_WHITE_SPRING_BLACK,
    XSPRING_BLACK_SPRING_WHITE,
    XSPRING_BLACK_TO_IDLE,
    XSPRING_WHITE_TO_IDLE,
} XSpringStatus;

class XSpring : public Object
{
public:
    XSpring(int x, int y, SDL_Color palette);
    ~XSpring() { }

    void render_bg(Camera* cam, std::vector<Light> lights, bool active_color);
    void render_fg(Camera* cam, std::vector<Light> lights, bool active_color) { }

    void trigger() { }
    void untrigger() { }
    void update_x(SDL_Rect black_player, SDL_Rect white_player) { }
    void update_y();

    void white_land(Zonestate* zone, Dot* white);
    void black_land(Zonestate* zone, Dot* black);

    SDL_Rect get_land_rect();

private:
    XSpringStatus m_status;
    int animation_start;
    Dot* dot_on;
};

/******************/
/*   SMALL LAMP   */
/******************/

class SmallLamp : public Object
{
public:
    SmallLamp(int x, int y, bool color, float strength, SDL_Color palette);
    ~SmallLamp() { }

    void render_bg(Camera* cam, std::vector<Light> lights, bool active_color);
    void render_fg(Camera* cam, std::vector<Light> lights, bool active_color) { }

    void trigger() { }
    void untrigger() { }
    void update_x(SDL_Rect black_player, SDL_Rect white_player) { }
    void update_y() { }
};

/*
class Block : public Object
{
public:
    Block(int x, int y, int w, int h, TileType blocktype, Level* lvl, SDL_Color palette);
    ~Block();
    void render(Camera* cam, bool active_color) { }

private:
    Level* m_lvl;
    TileType m_tiletype;
    std::vector<TileType> saved_tiles;
};
*/

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
