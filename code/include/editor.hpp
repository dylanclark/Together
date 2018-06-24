#ifndef editor_hpp
#define editor_hpp

#include <engine.hpp>
#include <tiles.hpp>

typedef enum Color {
    COLOR_BLACK,
    COLOR_WHITE
} Color;

typedef enum PlacingType {
    PLACING_DELETE,
    PLACING_TILES,
    PLACING_CHARS,
    PLACING_LEVEL_ENDS,
    PLACING_BUTTONS,
    PLACING_CRATES,
    PLACING_SPRINGS
} PlacingType;

static const int EDITOR_CAMERA_SPEED = 10;

// a specific class for our editor camera. it doesn't mess around
class EditorCamera
{
public:
    EditorCamera(int w, int h, int lvlw, int lvlh);

    void handle_event(SDL_Event e);
    void update(int w, int h, int scr_w, int scr_h);

    // return the rect for other draw functions
    SDL_Rect get_rect() { return true_rect; }

    // move the camera when we delete rows of tiles
    void move_up()      { center_rect.y -= TILE_WIDTH; }
    void move_down()    { center_rect.y += TILE_WIDTH; }
    void move_right()   { center_rect.x += TILE_WIDTH; }
    void move_left()    { center_rect.x -= TILE_WIDTH; }

private:
    // the centered camera position and the true rectangle in SDL coords
    SDL_Rect center_rect;
    SDL_Rect true_rect;

    // controller booleans
    bool up, down, left, right;
    bool zoomin, zoomout;
};

// a class for the border of our level
class Border
{
public:
    Border(int w, int h);
    void update(int w, int h);
    void draw(int scr_w, int scr_h, SDL_Rect cam_rect, Engine* game);
private:
    SDL_Rect rect;
};

// a class for drawing the gridlines of our tile grid
class Gridlines
{
public:
    Gridlines(int w, int h);
    void update(int w, int h);
    void draw(int scr_w, int scr_h, SDL_Rect cam_rect, Engine* game);
private:
    int width, height;
};

class Object
{
public:
    PlacingType type;
    int x, y;
    Color color;
};

class Tileset
{
public:
    Tileset(int w, int h, int* tiles, std::vector<Object> objs);
    void draw(int scr_w, int scr_h, SDL_Rect cam_rect, Engine* game);
    void handle_event(SDL_Event e, int scr_w, int scr_h, SDL_Rect cam_rect, PlacingType placing);

private:
    void fill_rect(Color color, int x, int y);
    void add_row_top();
    void remove_row_top();
    void add_row_bottom();
    void remove_row_bottom();
    void add_col_left();
    void remove_col_left();
    void add_col_right();
    void remove_col_right();

    // tiles and objects we want to keep track of
    int* tiles;
    int width, height;
    std::vector<Object> objs;

    // these are for remembering two clicks when drawing tile rects
    int click_x, click_y;
    bool clicked;
    Color clicked_color;
};


// the editor state class itself
class Editor : public Gamestate
{
public:
    void init(Engine* game);
    void cleanup();

    void handle_events(Engine* game);
    void update(Engine* game);
    void draw(Engine* game);

private:
    // helper functions for the editor
    void draw_UI(int scr_w, int scr_h, PlacingType placing);
    std::string get_str(Engine* game, std::string prompt);
    bool get_yes_no(Engine* game, std::string prompt);
    int* output_arr(int w, int h, int* tiles);

    EditorCamera* cam;
    Border* border;
    Gridlines* grid;
    Tileset* tileset;
};

#endif /* editor_hpp */
