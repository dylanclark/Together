
#ifndef char_hpp
#define char_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
#include <textures.hpp>
#include <engine.hpp>
#include <tiles.hpp>

static const float GRAVITY = .25;

class Vector
{
public:
    // init
    Vector();

    // coords
    int x;
    int y;
};

bool check_collision(SDL_Rect a, SDL_Rect b, Vector* overlap);
bool check_grounded(SDL_Rect a, SDL_Rect b, bool a_color);

class Zonestate;
class LevelExit;

typedef enum _CharDir {
    DIR_RIGHT = 0,
    DIR_LEFT
} CharDir;

typedef enum _CharStatus {
    CHAR_IDLE = 0,
    CHAR_RUN,
    CHAR_JUMP,
    CHAR_INACTIVE
} CharStatus;

class Dot
{
public:
    Dot(int x, int y, bool color, SDL_Renderer* rend, SDL_Color* palette = NULL);

    bool my_color;

    bool handle_event(SDL_Event &e, Zonestate* zone, Engine* game);
    void update(Zonestate* zone, Engine* game);
    void render(SDL_Rect* camera, Engine* game);

    void move(int x, int y);
    void snap(LevelExit exit);

    SDL_Rect get_rect() { return col_rect; }
    int get_dir() { return dir; }
    float get_yvel() { return m_yvel; }

    void spring_me(float yvel);
    bool center(SDL_Rect* end_rect);

    bool snapped;
    // ready for exit
    bool ready;

private:
    // status = idle, jump, run, inactive, etc.
    CharStatus status;
    // which way is the dot facing?
    CharDir dir;
    // what animation frame is the dot on?
    int jump_start;

    // is the dot jumping?
    int short_hop;
    float m_xvel, m_yvel;
    float true_y;
    bool up, down, left, right;
    bool platform_drop;
    Controller* controller;
    Texture m_tex;
    SDL_Rect col_rect;

    bool crate_col(Zonestate* zone, Engine* game);
};

#endif /* char_hpp */
