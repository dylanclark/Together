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

class Camera
{
public:
    Camera (int scr_w, int scr_h,
            int lev_w, int lev_h,
            SDL_Rect active_char, int dir);
    void update (SDL_Rect active_char, int dir);
    SDL_Rect* get_display();

private:
    SDL_Rect display;
    float loc_x, loc_y, loc_w, loc_h;
    int level_w, level_h;
    int screen_w, screen_h;
    void track (SDL_Rect char1, SDL_Rect char2);
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
    Level(int lvl_num) { m_lvl_num = lvl_num; }
    void init(Engine* game);
    void update(Engine* game);
    void draw(Engine* game);
    void handle_events(Engine* game);
    void load_level(Engine* game, std::string lvlnum);
    void cleanup();

    void shift();

private:
    // level number
    int m_lvl_num;

    // these give the level's coords in the zone. they are by tile
    int m_x, m_y;

    // tile dimensions of level
    int m_width, m_height;

    // shifting info
    bool shiftable;
    bool active_color;

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

private:
    // level number
    int m_zone_num;

    Camera* camera;
    std::vector<Dot*> chars;
    std::vector<Level*> levels;

    // shifting info
    bool shiftable;
    bool active_color;
};

#endif /* levels_hpp */
