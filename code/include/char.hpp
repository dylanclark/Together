
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

class Zonestate;
class Level;

bool check_collision(SDL_Rect a, SDL_Rect b, Vector* overlap);
bool check_point_in_triangle(int x, int y, bool color, SDL_Rect triangle, bool down, Vector* repos);
bool check_point_in_rect(int x, int y, bool color, SDL_Rect rect, Vector* repos);
bool check_grounded(SDL_Rect a, SDL_Rect b, bool a_color);
bool check_full_overlap(SDL_Rect a, SDL_Rect b);
bool check_in_platform(SDL_Rect a, SDL_Rect b);

typedef enum _CharDir {
    DIR_RIGHT = 0,
    DIR_LEFT
} CharDir;

typedef enum _CharStatus {
    CHAR_IDLE = 0,
    CHAR_RUN,
    CHAR_JUMP,
    CHAR_INACTIVE,
    CHAR_EXITED,
    CHAR_DIE,
} CharStatus;

typedef enum _ExitDir {
    EXIT_LEFT = 0,
    EXIT_RIGHT,
    EXIT_UP,
    EXIT_DOWN
} ExitDir;

class MovingPlatformWall;

class Dot
{
public:
    Dot(int x, int y, bool color, SDL_Color* palette = NULL);

    bool m_color;

    bool handle_event(SDL_Event &e, Zonestate* zone);
    void update_x(Zonestate* zone);
    void update_y(Zonestate* zone);
    void check_for_platforms(Zonestate* zone);
    void render(SDL_Rect* camera, Level* lvl);

    void save_state();
    void reset(Zonestate* zone);

    SDL_Rect get_rect() { return col_rect; }
    int get_dir() { return dir; }
    float get_yvel() { return m_yvel; }
    int get_exit_dir() { return (exited || entering) ? (int) exit_dir : -1; }

    void spring_me(float yvel);

    void exit(ExitDir dir);
    void enter();
    void good_exit(Zonestate* zone);
    bool in_level(Level* lvl);

private:
    Texture m_tex;

    CharStatus m_status;

    SDL_Rect col_rect;
    float true_y;
    float m_xvel, m_yvel;
    int ext_yvel;
    int ext_xvel;
    CharDir dir;

    bool on_moving_platform;
    int on_moving_platform_idx;
    int on_platform_x, on_platform_y;

    bool in_moving_platform;
    int in_moving_platform_idx;
    int in_platform_x, in_platform_y;
    std::vector<MovingPlatformWall> walls;

    Controller* controller;
    bool up, down, left, right;

    int jump_start;
    int short_hop;
    bool platform_drop;

    bool exiting, exited, entering;
    ExitDir exit_dir;

    SDL_Rect saved_col_rect;
    float saved_yvel;
    CharStatus saved_status;
    CharDir saved_dir;
};

#endif /* char_hpp */
