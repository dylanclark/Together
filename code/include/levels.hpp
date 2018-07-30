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

class Level;

class Camera
{
public:
    Camera (int scr_w, int scr_h, Level* level, SDL_Rect active_char, int dir);
    ~Camera();
    void update (SDL_Rect active_char, int dir);
    SDL_Rect* get_display() { return &display; }
    void set_level(Level* level);

private:
    SDL_Rect display;
    int m_lvl_x, m_lvl_y;
    int m_lvl_w, m_lvl_h;
    float loc_x, loc_y;
    SDL_Rect get_target(SDL_Rect active_char, int dir);
};

class Tile;

class Level
{
public:
    Level(Engine* game, int zone_num, int lvl_num, int x, int y, SDL_Color palette);
    ~Level();
    void load_level(Engine* game, int zone_num, int lvl_num, SDL_Color palette);
    bool update(Zonestate* zone, std::vector<Dot> &chars);
    void draw_bg(Engine* game, SDL_Rect cam_rect, bool active_color);
    void draw_fg(Engine* game, SDL_Rect cam_rect, bool active_color);
    void cleanup();

    void shift();
    int get_x() { return m_x; }
    int get_y() { return m_y; }
    int get_w() { return m_w; }
    int get_h() { return m_h; }
    std::vector<Tile> get_tileset() { return tileset; }
    std::vector<Object*> get_objects() { return objects; }

    bool just_exited;

private:
    // level number
    int m_zone_num;
    int m_lvl_num;

    // these give the level's coords in the zone. they are by tile
    int m_x, m_y;
    int m_w, m_h;

    // chars and level endings!
    int num_chars_ready;

    // tileset
    std::vector<Tile> tileset;
    std::vector<Object*> objects;
    Texture tile_tex;
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
    void check_exit();

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
