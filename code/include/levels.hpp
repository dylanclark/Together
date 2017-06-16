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
const float BUFFER = 400;

// max level size
const int MAX_SIZE = 50 * 50;
const int MAX_CRATES = 10;

// speed constants
const float SPEED = 14;
const float CAM_ACC = 2;

class Camera
{
public:
    Camera(int scr_w, int scr_h, int lev_w, int lev_h);
    void update(SDL_Rect* b_char, SDL_Rect* w_char);
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
    void track(SDL_Rect* b_char, SDL_Rect* w_char);
    void move();
};

class LevelEnd
{
public:
    LevelEnd();

    void render(SDL_Rect* camera, Engine* game);

    SDL_Rect col_rect;
    Texture tex;

    bool check(SDL_Rect dot_rect);
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

class Levelstate : public Gamestate
{
public:
    // tile dimensions of level
    int width;
    int height;
    bool shiftable;

    Camera* camera;
    Dot* b_char;
    Dot* w_char;

    // objects
    LevelEnd b_level_end;
    LevelEnd w_level_end;
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
    Texture b_char_tex;
    Texture w_char_tex;
    Texture tile_tex;
    Texture b_end_tex;
    Texture w_end_tex;
    Texture b_button_tex;
    Texture w_button_tex;
    Texture crate_tex_four_by_two;
    Texture w_crate_tex_four_by_two;
    Texture w_platform;
    Texture crate_tex_four_by_one;
    Texture crate_tex_three_by_two;
    Texture crate_tex_two_by_two;
    Texture b_springboard_tex;
    Texture w_springboard_tex;
    Texture w_end_animate;
    Texture b_end_animate;
    Texture b_cross_spring_tex;
    Texture w_cross_spring_tex;
    Texture level1_end_tex;
    Texture level1_start_tex;
    Texture level2_end_tex;
    Texture level2_start_tex;

    // tileset
    Tile* tileset[MAX_SIZE];

    // crate array
    std::vector<Crate*> crates;

    // level file path
    std::string path;
};

#endif /* levels_hpp */