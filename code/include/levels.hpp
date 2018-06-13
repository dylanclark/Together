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

// camera edge buffer
static const float BUFFER = 400;

// max level size
static const int MAX_SIZE = 50 * 50;
static const int MAX_CRATES = 10;

// speed constants
static const float CAM_ACC = 0.3;

class Camera
{
public:
    Camera (int scr_w, int scr_h,
            int lev_w, int lev_h,
            SDL_Rect char1, SDL_Rect char2);
    void update (SDL_Rect char1, SDL_Rect char2);
    SDL_Rect* get_display();

private:
    SDL_Rect display;
    SDL_Rect location;
    int level_w, level_h;
    int screen_w, screen_h;
    float x_vel;
    float y_vel;
    float w_vel;
    float h_vel;
    void track (SDL_Rect char1, SDL_Rect char2);
    void move ();
    SDL_Rect get_target (SDL_Rect char1, SDL_Rect char2);
};

class LevelEnd
{
public:
    LevelEnd(int x, int y, bool is_black, SDL_Renderer* rend);
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

class Crate;
class Tile;

bool load_level_from_file(Engine* game, Levelstate* lvl, std::string filename);

class Levelstate : public Gamestate
{
public:
    void update(Engine* game);
    void handle_events(Engine* game);
    void load_level(Engine* game, std::string lvlnum);
    virtual void init_objects(Engine* game) = 0;
    virtual void interactions(Engine* game) = 0;

    void pause(Engine* game);

    // tile dimensions of level
    int width;
    int height;
    bool shiftable;
    int status;

    Camera* camera;
    std::vector<Dot*> chars;
    Dot* b_char;
    Dot* w_char;

    // level ends
    std::vector<LevelEnd*> level_ends;
    LevelEnd* b_level_end;
    LevelEnd* w_level_end;

    // objects
    std::vector<Crate*> crates;
    std::vector<Button*> buttons;
    std::vector<Springboard*> springs;
    std::vector<LevelMessage*> messages;

    // other objects
    Button b_button;
    Button w_button;
    Button b_button2;
    Springboard w_springboard;
    Springboard b_springboard;
    Springboard w_cross_spring;
    Springboard b_cross_spring;
    LevelMessage level1_end;
    LevelMessage level2_end;
    LevelMessage level1_start;
    LevelMessage level2_start;

    // textures
    Texture tile_tex;
    Texture b_button_tex;
    Texture w_button_tex;
    Texture w_platform;
    Texture b_springboard_tex;
    Texture w_springboard_tex;
    Texture b_cross_spring_tex;
    Texture w_cross_spring_tex;

    // tileset
    Tile* tileset[MAX_SIZE];

    // level file path
    std::string path;
};

#endif /* levels_hpp */
