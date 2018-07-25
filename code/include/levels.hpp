#ifndef levels_hpp
#define levels_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <engine.hpp>
#include <textures.hpp>
#include <char.hpp>
#include <objects.hpp>
#include <tiles.hpp>

// max level size
static const int MAX_SIZE = 50 * 50;
static const int MAX_CRATES = 10;

typedef enum ExitDir
{
    EXIT_UP,
    EXIT_RIGHT,
    EXIT_DOWN,
    EXIT_LEFT
} ExitDir;

class Level;

class Camera
{
public:
    Camera (int scr_w, int scr_h, Level* level, SDL_Rect active_char, int dir);
    Camera (int scr_w, int scr_h, int level_w, int level_h, SDL_Rect active_char, int dir);
    ~Camera();
    void update (SDL_Rect active_char, int dir);
    SDL_Rect* get_display();
    void set_level(Level* level);

private:
    SDL_Rect display;
    int m_lvl_x, m_lvl_y;
    int m_lvl_w, m_lvl_h;
    float loc_x, loc_y;
    SDL_Rect get_target(SDL_Rect active_char, int dir);
};

class LevelEnd
{
public:
    LevelEnd(int x, int y, bool is_black, SDL_Renderer* rend, SDL_Color* palette);
    void move(int x, int y);
    bool check(SDL_Rect dot_rect);
    bool update(SDL_Rect dot_rect);
    void render(SDL_Rect* camera, Engine* game);

private:
    SDL_Rect col_rect;
    Texture tex;
};

class LevelExit
{
public:
    LevelExit(int x, int y, ExitDir dir, SDL_Renderer* rend, SDL_Color* palette);
    int check(SDL_Rect char_rect);
    void render(Engine* game, SDL_Rect camera);

    SDL_Rect get_rect() { return m_rect; }
    ExitDir get_dir() { return m_dir; }

private:
    ExitDir m_dir;
    SDL_Rect m_rect;
    Texture m_tex;
};

class LevelMessage
{
public:
    LevelMessage();
    void send_message(int type, Engine* game);
    void render(SDL_Rect* camera, SDL_Renderer* rend);
    SDL_Rect col_rect;
    Texture tex;
    int type;
};

class Tile;

bool load_level_from_file(Engine* game, Levelstate* lvl, std::string filename);

class Levelstate : public Gamestate
{
public:
    Levelstate(int lvl_num) { m_lvl_num = lvl_num; }
    void init(Engine* game);
    void update(Engine* game);
    void draw(Engine* game);
    void handle_events(Engine* game);
    void load_level(Engine* game, std::string lvlnum);
    void cleanup();

    void pause(Engine* game);
    void shift();

    // level number
    int m_lvl_num;

    // tile dimensions of level
    int width;
    int height;

    // shifting info
    bool shiftable;
    bool active_color;

    Camera* camera;

    // chars and level endings!
    std::vector<Dot*> chars;
    std::vector<LevelEnd*> level_ends;

    // tileset
    Tile* tileset[MAX_SIZE];
    Texture tile_tex;

    // objects
    std::vector<Crate*> crates;
    std::vector<Button*> buttons;
    std::vector<Springboard*> springs;
    std::vector<LevelMessage*> messages;
};

class Level
{
public:
    Level(Engine* game, int zone_num, int lvl_num, int x, int y, SDL_Color palette);
    ~Level();
    void load_level(Engine* game, int zone_num, int lvl_num, SDL_Color palette);
    int update(Zonestate* zone, std::vector<Dot> &chars);
    void draw(Engine* game, SDL_Rect cam_rect, bool active_color);
    void cleanup();

    void shift();
    int get_x() { return m_x; }
    int get_y() { return m_y; }
    int get_w() { return m_w; }
    int get_h() { return m_h; }
    std::vector<Tile> get_tileset() { return tileset; }

private:
    // level number
    int m_zone_num;
    int m_lvl_num;

    // these give the level's coords in the zone. they are by tile
    int m_x, m_y;
    int m_w, m_h;

    // chars and level endings!
    std::vector<LevelExit> exits;
    int chosen_exit;
    int num_chars_ready;

    // tileset
    std::vector<Tile> tileset;
    Texture tile_tex;

    // objects
    std::vector<Crate*> crates;
    std::vector<Button*> buttons;
    std::vector<Springboard*> springs;
    std::vector<LevelMessage*> messages;
};


class Zonestate : public Gamestate
{
public:
    Zonestate(int zone_num) { m_zone_num = zone_num; }
    void init(Engine* game);

    void update(Engine* game);
    void draw(Engine* game);
    void handle_events(Engine* game);

    void cleanup();

    void pause(Engine* game);
    void shift();

    Level* get_active_level();

    // shifting info
    bool shiftable;
    bool active_color;

private:
    // level number
    int m_zone_num;
    int active_level;
    SDL_Color palette;

    Camera* camera;
    std::vector<Dot> chars;
    std::vector<Level*> levels;
};

#endif /* levels_hpp */
