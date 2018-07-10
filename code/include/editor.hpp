#ifndef editor_hpp
#define editor_hpp

#include <engine.hpp>
#include <tiles.hpp>

typedef enum Color {
    COLOR_BLACK,
    COLOR_WHITE
} Color;

static const int NUM_PLACING_TYPES = 7;

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
    void update(Engine* game);

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
    void draw(int scr_w, int scr_h, SDL_Rect cam_rect, SDL_Renderer* rend);
private:
    SDL_Rect rect;
};

// a class for drawing the gridlines of our tile grid
class Gridlines
{
public:
    Gridlines(int w, int h);
    void update(int w, int h);
    void draw(int scr_w, int scr_h, SDL_Rect cam_rect, SDL_Renderer* rend);
private:
    int width, height;
};

class Object
{
public:
    PlacingType type;
    int x, y;
    Color color;
    SDL_Texture* tex;
};

class Tileset
{
public:
    Tileset(int w, int h, std::vector<std::vector<int> > tiles_arg, std::vector<Object> objs);
    void draw(int scr_w, int scr_h, SDL_Rect cam_rect, SDL_Renderer* rend);
    void handle_event(Engine* game, SDL_Event e, int scr_w, int scr_h, SDL_Rect cam_rect, PlacingType placing);

    void add_row_top();
    void remove_row_top();
    void add_row_bottom();
    void remove_row_bottom();
    void add_col_left();
    void remove_col_left();
    void add_col_right();
    void remove_col_right();

    std::vector<std::vector<int> > tiles;
    std::vector<Object> objs;

private:
    void fill_rect(Color color, int x, int y);

    // tiles and objects we want to keep track of
    int width, height;

    // these are for remembering two clicks when drawing tile rects
    int click_x, click_y;
    bool clicked;
    Color clicked_color;
};

std::string get_str(Engine* game, std::string prompt, std::string result = "");
bool get_yes_no(Engine* game, std::string prompt);

// the editor state class itself
class LevelEditor : public Gamestate
{
public:
    LevelEditor(int zone_num, int lvl_num) { m_zone_num = zone_num; m_lvl_num = lvl_num; }
    void init(Engine* game);
    void cleanup();

    void handle_events(Engine* game);
    void update(Engine* game);
    void draw(Engine* game);

private:
    // helper functions for the editor
    void draw_UI(Engine* game, int scr_w, int scr_h);
    std::vector<std::vector<std::string> > output_arr(std::vector<std::vector<int> > tiles);
    void write_level(Engine* game);

    EditorCamera* camera;
    Border* border;
    Gridlines* grid;
    Tileset* tileset;

    PlacingType placing;
    int lvl_w, lvl_h;
    int m_zone_num, m_lvl_num;

    TTF_Font* my_font;
};

class LevelThumbnail
{
public:
    LevelThumbnail(Engine* game, int zone_num, int lvl_num);
    ~LevelThumbnail();

    void draw(SDL_Renderer* rend, SDL_Rect cam_rect, int scr_w, int scr_h);
    void move(int x, int y, std::vector<LevelThumbnail*> levels);
    SDL_Rect get_rect();

    bool selected;
    bool valid;

private:
    int m_x, m_y;
    int m_w, m_h;

    // texture
    SDL_Texture* m_tex;
};

class ZoneEditor : public Gamestate
{
public:
    ZoneEditor() { }
    void init(Engine* game);
    void cleanup();

    void handle_events(Engine* game);
    void update(Engine* game);
    void draw(Engine* game);

private:
    void write_zone();

    int m_zone_num;
    int r, g, b;

    // mouse drag controls
    bool mousedown;
    int selected;
    int x_offset, y_offset;
    bool select_valid;

    EditorCamera* camera;

    std::vector<LevelThumbnail*> levels;
};

#endif /* editor_hpp */
