#ifndef editor_hpp
#define editor_hpp

#include <engine.hpp>
#include <levels.hpp>
#include <tiles.hpp>

typedef enum Color {
    COLOR_BLACK,
    COLOR_WHITE,
    COLOR_CLEAR,
    COLOR_SOLID
} Color;

typedef enum PlacingType {
    PLACING_TILES_BW,
    PLACING_TILES_CS,
    PLACING_SPIKES_FC,
    PLACING_SPIKES_RL,
    PLACING_SLOPE_2,
    PLACING_PLATFORMS,
    PLACING_CRATES,
    PLACING_SMALL_LAMPS,
    PLACING_SPRINGS,
    PLACING_XSPRINGS,
    PLACING_MOVING_PLATFORMS,
    PLACING_SHIFTBLOCKS,
    PLACING_BUTTONS,
    PLACING_KEYS,
    PLACING_DOORS,
    PLACING_DELETE
} PlacingType;
static const int NUM_PLACING_TYPES = PLACING_DELETE + 1;

static const int EDITOR_CAMERA_SPEED = 10;

// a specific class for our editor camera. it doesn't mess around
class EditorCamera
{
public:
    EditorCamera(int w, int h, int lvlw, int lvlh);

    void handle_event(SDL_Event e);
    void update();

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
    void draw(EditorCamera* cam);
private:
    SDL_Rect rect;
};

// a class for drawing the gridlines of our tile grid
class Gridlines
{
public:
    Gridlines(int w, int h);
    void update(int w, int h);
    void draw(EditorCamera* cam);
private:
    int width, height;
};

class EditorObject
{
public:
    ObjectType type;
    int x, y;
    // for moving platforms
    int w, h;
    int x2, y2;
    int pause_length, move_length;
    // for springs
    int spring_height;
    int light_strength;
    Color color;
    SDL_Texture* tex;
};

class EditorTileset
{
public:
    EditorTileset(int w, int h, std::vector<std::vector<TileType> > tiles_arg, std::vector<EditorObject> objs);
    void draw(EditorCamera* cam);
    void handle_event(SDL_Event e, int scr_w, int scr_h, SDL_Rect cam_rect, PlacingType placing);

    void add_row_top();
    void remove_row_top();
    void add_row_bottom();
    void remove_row_bottom();
    void add_col_left();
    void remove_col_left();
    void add_col_right();
    void remove_col_right();

    std::vector<std::vector<TileType> > tiles;
    std::vector<EditorObject> objs;

private:
    void fill_rect(TileType type, int x, int y);

    // tiles and objects we want to keep track of
    int width, height;

    // these are for remembering two clicks when drawing tile rects
    int click_x, click_y;
    bool clicked;
    TileType clicked_type;

    int moving_platforms;
    bool shiftblock;
    bool crate;

    OldTexture m_tiletex;
    OldTexture m_shiftblocktex;
};

std::string get_str(std::string prompt, std::string result = "");
bool get_yes_no(std::string prompt);

// the editor state class itself
class LevelEditor : public Gamestate
{
public:
    LevelEditor(int zone_num, int lvl_num) { m_zone_num = zone_num; m_lvl_num = lvl_num; }
    void init();
    void cleanup();

    void handle_events();
    void update();
    void draw();

private:
    // helper functions for the editor
    void draw_UI(int scr_w, int scr_h);
    std::vector<std::vector<std::string> > output_arr(std::vector<std::vector<TileType> > tiles);
    void write_level();

    EditorCamera* camera;
    Border* border;
    Gridlines* grid;
    EditorTileset* tileset;

    PlacingType placing;
    int lvl_w, lvl_h;
    int m_zone_num, m_lvl_num;

    TTF_Font* my_font;
};

class LevelLoaderCamera
{
public:
    LevelLoaderCamera(int scr_w, int scr_h);
    void handle_event(SDL_Event e);
    void update(int max_y);

    SDL_Rect get_rect() { return rect; }

private:
    // camera rect
    SDL_Rect rect;

    // controller booleans
    bool up, down;
};

class LevelLoaderThumbnail
{
public:
    LevelLoaderThumbnail(int zone_num, int lvl_num, int x, int y);
    void draw(SDL_Rect cam_rect);
    void select();
    void unselect();
    SDL_Rect get_rect() { SDL_Rect res = {m_x, m_y, m_w, m_h}; return res; }

private:
    int m_zone_num, m_lvl_num;
    int m_x, m_y;
    int m_w, m_h;
    bool selected;
    SDL_Texture* m_tex;
};

class ZoneList
{
public:
    ZoneList(int zone_num, int y);

    void draw(SDL_Rect cam_rect);
    int click(int mousex, int mousey);
    void select(int lvl_num);
    int num_levels() { return levels.size(); }
    int get_tex_height();
    int get_y() { return m_y; }

private:
    // m_tex will just be text that says "zone 1" or something like that
    SDL_Texture* m_tex;
    int m_zone_num;
    int m_y;
    std::vector<LevelLoaderThumbnail> levels;
};

class LevelLoader : public Gamestate
{
public:
    LevelLoader(int zone_num) { m_zone_num = zone_num; }
    void init();
    void cleanup();

    void handle_events();
    void update();
    void draw();

    void load_level();

private:
    SDL_Texture* m_tex;
    LevelLoaderCamera* camera;
    std::vector<ZoneList*> zones;
    int selected_zone;
    int selected_lvl;
    int m_zone_num;
};

class LevelThumbnailExit
{
public:
    LevelThumbnailExit(int x, int y, ExitDir dir, int lvl_x, int lvl_y) { m_x = x; m_y = y; m_dir = dir; m_lvl_x = lvl_x; m_lvl_y = lvl_y; }
    void move(int lvl_x, int lvl_y) { m_lvl_x = lvl_x; m_lvl_y = lvl_y; }
    void draw(SDL_Rect cam_rect, int scr_w, int scr_h);

    int m_x, m_y;
    ExitDir m_dir;
    int m_lvl_x, m_lvl_y;
};

class LevelThumbnail
{
public:
    LevelThumbnail(int zone_num, int lvl_num, int x, int y, std::vector<LevelThumbnail> &levels);
    ~LevelThumbnail();

    void draw(SDL_Rect cam_rect, int scr_w, int scr_h);
    void move(int x, int y, std::vector<LevelThumbnail> &valid_levels);
    SDL_Rect get_rect();

    bool selected;
    bool start;
    bool valid;
    int m_x, m_y;
    int m_w, m_h;

private:
    std::vector<LevelThumbnailExit*> exits;

    int m_lvl_num;
    SDL_Texture* m_tex;
};

class ZoneEditor : public Gamestate
{
public:
    ZoneEditor() { }
    void init();
    void cleanup();

    void handle_events();
    void update();
    void draw();

private:
    void delete_level(int lvl_num);
    void write_zone();
    void destroy_editor();

    int m_zone_num;
    int min_x, max_x;
    int min_y, max_y;
    int r, g, b;

    // mouse drag controls
    bool mousedown;
    int x_offset, y_offset;
    // which level is selected?
    int selected;
    // which level do the chars start in?
    int start;
    bool placing_chars;
    int b_char_x, b_char_y;
    int w_char_x, w_char_y;

    bool edited_level, created_level, loaded_level;

    EditorCamera* camera;

    std::vector<LevelThumbnail> levels;
};

#endif /* editor_hpp */
