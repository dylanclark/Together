#include <algorithm>
#include <cstdlib>
#include <limits>

#include <editor.hpp>
#include <levels.hpp>
#include <tiles.hpp>
#include <textures.hpp>

EditorCamera::EditorCamera(int scr_w, int scr_h, int x, int y)
{
    center_rect.w = scr_w;
    center_rect.h = scr_h;
    center_rect.x = x;
    center_rect.y = y;
    true_rect.x = center_rect.x - center_rect.w/2;
    true_rect.y = center_rect.y - center_rect.h/2;
    true_rect.w = scr_w;
    true_rect.h = scr_h;
    up = down = left = right = zoomin = zoomout = false;
}

void EditorCamera::handle_event(SDL_Event e)
{
    switch (e.type) {
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode)
            {
            case SDL_SCANCODE_LEFT:
                left = true;
                break;
            case SDL_SCANCODE_RIGHT:
                right = true;
                break;
            case SDL_SCANCODE_UP:
                up = true;
                break;
            case SDL_SCANCODE_DOWN:
                down = true;
                break;
            case SDL_SCANCODE_Z:
                zoomout = true;
                break;
            case SDL_SCANCODE_X:
                zoomin = true;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_LEFT:
                    left = false;
                    break;
                case SDL_SCANCODE_RIGHT:
                    right = false;
                    break;
                case SDL_SCANCODE_UP:
                    up = false;
                    break;
                case SDL_SCANCODE_DOWN:
                    down = false;
                    break;
                case SDL_SCANCODE_Z:
                    zoomout = false;
                    break;
                case SDL_SCANCODE_X:
                    zoomin = false;
                    break;
                default:
                    break;
            }
    }
}

void EditorCamera::update()
{
    int scr_w = game->screen_width;
    int scr_h = game->screen_height;
    if (up) {
        center_rect.y -= EDITOR_CAMERA_SPEED*((float) center_rect.w / ((float)TILE_WIDTH*70));
    }
    if (down) {
        center_rect.y += EDITOR_CAMERA_SPEED*((float) center_rect.w / ((float)TILE_WIDTH*70));;
    }
    if (left) {
        center_rect.x -= EDITOR_CAMERA_SPEED*((float) center_rect.w / ((float)TILE_WIDTH*70));;
    }
    if (right) {
        center_rect.x += EDITOR_CAMERA_SPEED*((float) center_rect.w / ((float)TILE_WIDTH*70));;
    }
    if (zoomin) {
        if (center_rect.w > TILE_WIDTH*4) {
            center_rect.w /= 1.04;
            center_rect.h = center_rect.w * ((float) scr_h / (float) scr_w);
        }
    }
    if (zoomout) {
        if (center_rect.w < TILE_WIDTH*1000) {
            center_rect.w *= 1.04;
            center_rect.h = center_rect.w * ((float) scr_h / (float) scr_w);
        }
    }
    true_rect.x = center_rect.x - center_rect.w/2;
    true_rect.y = center_rect.y - center_rect.h/2;
    true_rect.w = center_rect.w;
    true_rect.h = center_rect.h;
}

Border::Border(int w, int h)
{
    rect.x = rect.y = 0;
    rect.w = w * TILE_WIDTH;
    rect.h = h * TILE_WIDTH;
}

void Border::update(int w, int h)
{
    rect.w = w * TILE_WIDTH;
    rect.h = h * TILE_WIDTH;
}

void Border::draw(int scr_w, int scr_h, SDL_Rect cam_rect)
{
    int x = (rect.x - cam_rect.x) / ((float) cam_rect.w / (float) scr_w);
    int y = (rect.y - cam_rect.y) / ((float) cam_rect.h / (float) scr_h);
    int w = rect.w * ((float) scr_w / (float) cam_rect.w);
    int h = rect.h * ((float) scr_h / (float) cam_rect.h);
    SDL_Rect to_draw = {x, y, w, h};
    SDL_SetRenderDrawColor(game->rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(game->rend, &to_draw);
}

Gridlines::Gridlines(int w, int h)
{
    width = w;
    height = h;
}

void Gridlines::update(int w, int h)
{
    width = w;
    height = h;
}

void Gridlines::draw(int scr_w, int scr_h, SDL_Rect cam_rect)
{
    for (int i = 1; i < width; i++) {
        int x = (i*TILE_WIDTH - cam_rect.x) / (cam_rect.w / (float) scr_w);
        int y1 = (-cam_rect.y) / (cam_rect.h / (float) scr_h);
        int y2 = (height*TILE_WIDTH - cam_rect.y) / (cam_rect.h / (float) scr_h);
        SDL_SetRenderDrawColor(game->rend, 0, 200, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(game->rend, x, y1, x, y2);
    }
    for (int i = 1; i < height; i++) {
        int y = (i*TILE_WIDTH - cam_rect.y) / (cam_rect.h / (float) scr_h);
        int x1 = (-cam_rect.x) / (cam_rect.w / (float) scr_w);
        int x2 = (width*TILE_WIDTH - cam_rect.x) / (cam_rect.w / (float) scr_w);
        SDL_SetRenderDrawColor(game->rend, 0, 200, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(game->rend, x1, y, x2, y);
    }
}

Tileset::Tileset(int w, int h, std::vector<std::vector<TileType> > tiles_arg=std::vector<std::vector<TileType> >(), std::vector<EditorObject> objs_arg=std::vector<EditorObject>())
{
    moving_platforms = 0;
    shiftblock = false;
    m_tiletex.load_tile_sheet("tiles.png", NULL);
    m_shiftblocktex.load_tile_sheet("shiftblock-idle.png", NULL);
    clicked = 0;
    click_x = click_y = 0;
    width = w;
    height = h;
    if (tiles_arg.size() == 0) {
        for (int i = 0; i < height; i++) {
            tiles.push_back(std::vector<TileType>());
            for (int j = 0; j < width; j++) {
                tiles[i].push_back(TILE_WHITE_SOLID);
            }
        }
    } else {
        tiles = tiles_arg;
    }
    objs = objs_arg;
}

void Tileset::draw(int scr_w, int scr_h, SDL_Rect cam_rect)
{
    SDL_Rect clip_rect;
    clip_rect.y = 0;
    clip_rect.w = TILE_WIDTH;
    clip_rect.h = TILE_WIDTH;
    float aspect_ratio = ((float) scr_w / (float) cam_rect.w);
    // first we will draw all of the tiles
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            clip_rect.x = tiles[i][j] * TILE_WIDTH * 2;
            int x1 = (j*TILE_WIDTH - cam_rect.x) * aspect_ratio;
            int x2 = ((j+1)*TILE_WIDTH - cam_rect.x) * aspect_ratio;
            int y1 = (i*TILE_WIDTH - cam_rect.y) * aspect_ratio;
            int y2 = ((i+1)*TILE_WIDTH - cam_rect.y) * aspect_ratio;
            int black_height;
            bool platform_flag = false;
            SDL_Rect to_draw = {x1, y1, x2-x1, y2-y1};
            switch (tiles[i][j])
            {
            case TILE_WHITE_SOLID:
                SDL_SetRenderDrawColor(game->rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
                break;
            case TILE_BLACK_SOLID:
                SDL_SetRenderDrawColor(game->rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
                break;
            case TILE_CLEAR:
                SDL_SetRenderDrawColor(game->rend, 100, 100, 100, SDL_ALPHA_OPAQUE);
                break;
            case TILE_SOLID:
                SDL_SetRenderDrawColor(game->rend, 0, 0, 150, SDL_ALPHA_OPAQUE);
                break;
            case TILE_BLACK_PLATFORM:
                black_height = TILE_WIDTH/4;
                platform_flag = true;
                break;
            case TILE_WHITE_PLATFORM:
                black_height = TILE_WIDTH*3/4;
                platform_flag = true;
                break;
            default:
                break;
            }
            if (clicked && click_x == j && click_y == i) {
                platform_flag = false;
                SDL_SetRenderDrawColor(game->rend, 255, 100, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(game->rend, &to_draw);
                continue;
            }
            if (platform_flag) {
                black_height *= aspect_ratio;
                SDL_Rect black_rect = {to_draw.x, to_draw.y, to_draw.w, black_height};
                SDL_Rect white_rect = {to_draw.x, to_draw.y+black_height + 1, to_draw.w, TILE_WIDTH*aspect_ratio - black_height};
                SDL_SetRenderDrawColor(game->rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(game->rend, &black_rect);
                SDL_SetRenderDrawColor(game->rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(game->rend, &white_rect);
            } else {
                m_tiletex.render(j*TILE_WIDTH, i*TILE_WIDTH, &clip_rect, &cam_rect);
            }
        }
    }
    for (int i = 0; i < objs.size(); i++) {
        int x = (objs[i].x*TILE_WIDTH - cam_rect.x) * aspect_ratio;
        int y = (objs[i].y*TILE_WIDTH - cam_rect.y) * aspect_ratio;
        int w, h;
        if (objs[i].type == OBJECT_SPRING) {
            w = (1 + (objs[i].type == OBJECT_SPRING)) * TILE_WIDTH * aspect_ratio;
            h = TILE_WIDTH * aspect_ratio;
        } else if (objs[i].type == OBJECT_MOVING_PLATFORM) {
            w = objs[i].w * TILE_WIDTH * aspect_ratio;
            h = objs[i].h * TILE_WIDTH * aspect_ratio;
            if (i == objs.size() - 1 && moving_platforms == 1) {
                w = TILE_WIDTH * aspect_ratio;
                h = TILE_WIDTH * aspect_ratio;
            }
        }
        SDL_Rect render_rect = {x, y, w, h};

        if (objs[i].type == OBJECT_SPRING) {
            SDL_SetRenderDrawColor(game->rend, 255, 255, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(game->rend, &render_rect);
            int arrow_w = TILE_WIDTH/2 * aspect_ratio;
            int arrow_h = TILE_WIDTH*objs[i].spring_height * aspect_ratio;
            int arrow_x = (((float) objs[i].x+3./4.)*TILE_WIDTH - cam_rect.x) * aspect_ratio;
            int arrow_y = ((objs[i].y + (objs[i].color == 0)) * TILE_WIDTH - cam_rect.y) * aspect_ratio - (!objs[i].color * arrow_h);
            SDL_Rect arrow_rect = {arrow_x, arrow_y, arrow_w, arrow_h};
            SDL_SetRenderDrawColor(game->rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(game->rend, &arrow_rect);
        } else if (objs[i].type == OBJECT_MOVING_PLATFORM) {
            SDL_SetRenderDrawBlendMode(game->rend, SDL_BLENDMODE_BLEND);
            if (objs[i].color == 0) {
                SDL_SetRenderDrawColor(game->rend, 0, 0, 0, 100);
            } else {
                SDL_SetRenderDrawColor(game->rend, 255, 255, 255, 100);
            }
            SDL_RenderFillRect(game->rend, &render_rect);
            if (i == objs.size() - 1 && moving_platforms == 2) {
                break;
            }
            int x2 = (objs[i].x2*TILE_WIDTH - cam_rect.x) * aspect_ratio;
            int y2 = (objs[i].y2*TILE_WIDTH - cam_rect.y) * aspect_ratio;
            SDL_Rect next_rect = {x2, y2, w, h};
            SDL_RenderFillRect(game->rend, &next_rect);
        } else if (objs[i].type == OBJECT_SHIFTBLOCK) {
            for (int j = objs[i].x; j < objs[i].x + objs[i].w; j++) {
                for (int k = objs[i].y; k < objs[i].y + objs[i].h; k++) {
                    SDL_Rect clip_rect = {0,0,TILE_WIDTH,TILE_WIDTH};
                    m_shiftblocktex.render(j*TILE_WIDTH, k*TILE_WIDTH, &clip_rect, &cam_rect);
                }
            }
        }
    }
    // next we will draw all of the objects... eventually, bc we don't support textures yet
}

void Tileset::handle_event(SDL_Event e, int scr_w, int scr_h, SDL_Rect cam_rect, PlacingType placing)
{
    int idx;
    bool click_color;
    switch (e.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        // get mouse position
        int mousex = e.button.x;
        int mousey = e.button.y;
        int x1 = (mousex * ((float) cam_rect.w / (float) scr_w) + cam_rect.x) / TILE_WIDTH;
        int y1 = (mousey * ((float) cam_rect.h / (float) scr_h) + cam_rect.y) / TILE_WIDTH;
        if (x1 >= width || y1 >= height || x1 < 0 || y1 < 0) {
            return;
        }
        switch (placing)
        {
        // we're placing tiles
        case PLACING_TILES_BW:
        case PLACING_TILES_CS:
        case PLACING_SPIKES_FC:
        case PLACING_SPIKES_RL:
        case PLACING_SLOPE_2:
        case PLACING_PLATFORMS:
            if (!clicked) {
                // if we haven't clicked before, save the click
                click_x = x1;
                click_y = y1;
                clicked = true;
                if (placing == PLACING_TILES_BW) {
                    clicked_type = (e.button.button == SDL_BUTTON_LEFT) ? TILE_BLACK_SOLID : TILE_WHITE_SOLID;
                } else if (placing == PLACING_TILES_CS) {
                    clicked_type = (e.button.button == SDL_BUTTON_LEFT) ? TILE_CLEAR : TILE_SOLID;
                } else if (placing == PLACING_PLATFORMS) {
                    clicked_type = (e.button.button == SDL_BUTTON_LEFT) ? TILE_BLACK_PLATFORM : TILE_WHITE_PLATFORM;
                } else if (placing == PLACING_SPIKES_FC) {
                    clicked_type = (e.button.button == SDL_BUTTON_LEFT) ? TILE_SPIKES_FLOOR : TILE_SPIKES_CEILING;
                } else if (placing == PLACING_SPIKES_RL) {
                    clicked_type = (e.button.button == SDL_BUTTON_LEFT) ? TILE_SPIKES_LEFT : TILE_SPIKES_RIGHT;
                } else if (placing == PLACING_SLOPE_2) {
                    clicked_type = (e.button.button == SDL_BUTTON_LEFT) ? TILE_SLOPE_2_UP_A : TILE_SLOPE_2_DOWN_A;
                }
            } else {
                // otherwise fill the rectangle that our clicks make!
                fill_rect(clicked_type, x1, y1);
            }
            break;
        // we're deleting stuff
        case PLACING_DELETE:
            for (int i = 0; i < objs.size(); i++) {
                if (objs[i].x == x1 && objs[i].y == y1) {
                    objs.erase(objs.begin() + i);
                    return;
                }
            }
            break;
        // we're placing objects
        case PLACING_SPRINGS:
            click_color = (e.button.button == SDL_BUTTON_LEFT) ? COLOR_BLACK : COLOR_WHITE;
            if (!click_color != tiles[y1][x1]) {
                printf("\a");
                return;
            }
            for (int i = 0; i < objs.size(); i++) {
                if (x1 == objs[i].x && y1 == objs[i].y) {
                    objs[i].spring_height = atoi(get_str("spring height").c_str());
                    return;
                }
            }
            EditorObject new_obj;
            new_obj.x = x1;
            new_obj.y = y1;
            new_obj.type = OBJECT_SPRING;
            new_obj.color = (Color) click_color;
            new_obj.spring_height = atoi(get_str("spring height").c_str());
            objs.push_back(new_obj);
            break;
        case PLACING_MOVING_PLATFORMS:
            click_color = (e.button.button == SDL_BUTTON_LEFT) ? COLOR_BLACK : COLOR_WHITE;
            idx = objs.size() - 1;
            if (moving_platforms == 0) {
                EditorObject new_obj;
                new_obj.x = x1;
                new_obj.y = y1;
                new_obj.type = OBJECT_MOVING_PLATFORM;
                new_obj.color = (Color) click_color;
                objs.push_back(new_obj);
                moving_platforms = 1;
            } else if (moving_platforms == 1) {
                objs[idx].w = abs(x1 - objs[idx].x + 1);
                objs[idx].h = abs(y1 - objs[idx].y + 1);
                objs[idx].x = std::min(x1, objs[idx].x);
                objs[idx].y = std::min(y1, objs[idx].y);
                moving_platforms = 2;
            } else {
                objs[idx].x2 = x1;
                objs[idx].y2 = y1;
                objs[idx].pause_length = atoi(get_str("pause time").c_str());
                objs[idx].move_length = atoi(get_str("move time").c_str());
                moving_platforms = 0;
            }
            break;
        case PLACING_SHIFTBLOCKS:
            idx = objs.size() - 1;
            if (!shiftblock) {
                EditorObject new_obj;
                new_obj.x = x1;
                new_obj.y = y1;
                new_obj.w = 1;
                new_obj.h = 1;
                new_obj.type = OBJECT_SHIFTBLOCK;
                objs.push_back(new_obj);
                shiftblock = true;
            } else {
                objs[idx].w = abs(x1 - objs[idx].x + 1);
                objs[idx].h = abs(y1 - objs[idx].y + 1);
                objs[idx].x = std::min(x1, objs[idx].x);
                objs[idx].y = std::min(y1, objs[idx].y);
                shiftblock = false;
            }
        default:
            break;
        }
    }
}

void Tileset::fill_rect(TileType type, int x, int y)
{
    clicked = false;
    int x1 = std::min(click_x, x);
    int x2 = std::max(click_x, x);
    int y1 = std::min(click_y, y);
    int y2 = std::max(click_y, y);
    if (type == TILE_SLOPE_2_UP_A || type == TILE_SLOPE_2_DOWN_A) {
        if (x2-x1+1 != 2*(y2-y1+1)) {
            return;
        }
        if (type == TILE_SLOPE_2_DOWN_A) {
            for (int i = y1; i <= y2; i++) {
                for (int j = x1; j <= x2; j++) {
                    if (i - y1 == (j - x1) / 2) {
                        tiles[i][j] = (TileType) ((int) type + ((j-x1) % 2));
                    } else if (i - y1 == (j - x1 - 1) / 2) {
                        tiles[i][j] = TILE_SLOPE_PAD_WHITE;
                    } else if (i - y1 == (j - x1 + 1) / 2) {
                        tiles[i][j] = TILE_SLOPE_PAD_BLACK;
                    }
                }
            }
        } else {
            for (int i = y2; i >= y1; i--) {
                for (int j = x1; j <= x2; j++) {
                    if (y2 - i == (j - x1) / 2) {
                        tiles[i][j] = (TileType) ((int) type + ((j-x1) % 2));
                    } else if (y2 - i == (j - x1 + 1) / 2) {
                        tiles[i][j] = TILE_SLOPE_PAD_WHITE;
                    } else if (y2 - i == (j - x1 - 1) / 2) {
                        tiles[i][j] = TILE_SLOPE_PAD_BLACK;
                    }
                }
            }
        }
        if (type == TILE_SLOPE_2_UP_A) {
            tiles[y2][x1-1] = TILE_SLOPE_PAD_WHITE;
            tiles[y1][x2+1] = TILE_SLOPE_PAD_BLACK;
        } else {
            tiles[y1][x1-1] = TILE_SLOPE_PAD_BLACK;
            tiles[y2][x2+1] = TILE_SLOPE_PAD_WHITE;
        }
    } else {
        for (int i = y1; i <= y2; i++) {
            for (int j = x1; j <= x2; j++) {
                tiles[i][j] = type;
            }
        }
    }
}

void Tileset::add_row_top()
{
    height++;
    std::vector<TileType> new_row = std::vector<TileType>();
    for (int j = 0; j < width; j++) {
        new_row.push_back(TILE_WHITE_SOLID);
    }
    tiles.insert(tiles.begin(), new_row);
}

void Tileset::remove_row_top()
{
    height--;
    tiles.erase(tiles.begin());
}

void Tileset::add_row_bottom()
{
    height++;
    std::vector<TileType> new_row = std::vector<TileType>();
    for (int j = 0; j < width; j++) {
        new_row.push_back(TILE_WHITE_SOLID);
    }
    tiles.push_back(new_row);
}

void Tileset::remove_row_bottom()
{
    height--;
    tiles.pop_back();
}

void Tileset::add_col_left()
{
    width++;
    for (int i = 0; i < height; i++) {
        tiles[i].insert(tiles[i].begin(), TILE_WHITE_SOLID);
    }
}

void Tileset::remove_col_left()
{
    width--;
    for (int i = 0; i < height; i++) {
        tiles[i].erase(tiles[i].begin());
    }
}

void Tileset::add_col_right()
{
    width++;
    for (int i = 0; i < height; i++) {
        tiles[i].push_back(TILE_WHITE_SOLID);
    }
}

void Tileset::remove_col_right()
{
    width--;
    for (int i = 0; i < height; i++) {
        tiles[i].pop_back();
    }
}

int get_height(float v0)
{
    float res;
    res = pow(v0, 2) / (2.*GRAVITY) / TILE_WIDTH;
    return round(res);
}

void LevelEditor::init()
{
    bool loading;
    if (m_lvl_num == -1) {
        m_zone_num = atoi(get_str("zone number").c_str());
        m_lvl_num = atoi(get_str("level number").c_str());
    }
    char lvl_cstr[5];
    snprintf(lvl_cstr, 5, "%d-%02d", m_zone_num, m_lvl_num);
    std::string lvl_str(lvl_cstr);
    lvl_cstr[1] = '\0';
    std::string zone_str(lvl_cstr);
    std::string path = "resources/level-files/"+zone_str+"/level"+lvl_str+".lvl";
    std::ifstream level_file(path.c_str());
    loading = !(level_file == NULL);

    if (!loading) {
        lvl_w = 67;
        lvl_h = 45;
        tileset = new Tileset(lvl_w, lvl_h);
    } else {
        level_file >> lvl_w;
        level_file >> lvl_h;

        std::vector<std::vector<TileType> > tiles;
        std::vector<EditorObject> objs;

        // load all the tiles
        int cur_tile;
        for (int i = 0; i < lvl_h; i++) {
            tiles.push_back(std::vector<TileType>());
            for (int j = 0; j < lvl_w; j++) {
                level_file >> cur_tile;
                tiles[i].push_back((TileType) cur_tile);
            }
        }

        // load all level exits
        int num_objs;
        level_file >> num_objs;
        int obj_type;
        int obj_x, obj_y;
        int obj_w, obj_h;
        int obj_x2, obj_y2;
        int obj_pause_length, obj_move_length;
        bool obj_color;
        float obj_springvel;
        EditorObject new_obj;

        // load all objects!
        for (int i = 0; i < num_objs; i++) {
            level_file >> obj_type;
            new_obj.type = (ObjectType) obj_type;
            switch (obj_type)
            {
            case OBJECT_SPRING:
                level_file >> obj_x >> obj_y >> obj_color;
                level_file >> obj_springvel;
                new_obj.x = obj_x;
                new_obj.y = obj_y;
                new_obj.color = (Color) obj_color;
                new_obj.spring_height = get_height(obj_springvel);
                break;
            case OBJECT_MOVING_PLATFORM:
                level_file >> obj_color >> obj_w >> obj_h >> obj_x >> obj_y >> obj_x2 >> obj_y2 >> obj_pause_length >> obj_move_length;
                new_obj.x = obj_x;
                new_obj.y = obj_y;
                new_obj.color = (Color) obj_color;
                new_obj.x2 = obj_x2;
                new_obj.y2 = obj_y2;
                new_obj.w = obj_w;
                new_obj.h = obj_h;
                new_obj.pause_length = obj_pause_length;
                new_obj.move_length = obj_move_length;
                break;
            case OBJECT_SHIFTBLOCK:
                level_file >> obj_x >> obj_y >> obj_w >> obj_h;
                new_obj.x = obj_x;
                new_obj.y = obj_y;
                new_obj.w = obj_w;
                new_obj.h = obj_h;
                break;
            default:
                break;
            }
            objs.push_back(new_obj);
        }
        tileset = new Tileset(lvl_w, lvl_h, tiles, objs);
    }
    placing = PLACING_TILES_BW;

    border = new Border(lvl_w, lvl_h);
    grid = new Gridlines(lvl_w, lvl_h);
    camera = new EditorCamera(game->screen_width, game->screen_height, lvl_w*TILE_WIDTH/2, lvl_h*TILE_WIDTH/2);
}

void LevelEditor::cleanup()
{

}

void LevelEditor::handle_events()
{
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        switch (e.type)
        {
        case SDL_QUIT:
            game->quit();
            break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                game->pop_state();
                break;
            case SDL_SCANCODE_RETURN:
                write_level();
                break;
            case SDL_SCANCODE_S:
                tileset->add_row_bottom();
                lvl_h++;
                break;
            case SDL_SCANCODE_W:
                camera->move_down();
                tileset->add_row_top();
                lvl_h++;
                break;
            case SDL_SCANCODE_A:
                camera->move_right();
                tileset->add_col_left();
                lvl_w++;
                break;
            case SDL_SCANCODE_D:
                tileset->add_col_right();
                lvl_w++;
                break;
            case SDL_SCANCODE_I:
                if (lvl_h > 45) {
                    tileset->remove_row_bottom();
                    lvl_h--;
                }
                break;
            case SDL_SCANCODE_K:
                if (lvl_h > 45) {
                    camera->move_up();
                    tileset->remove_row_top();
                    lvl_h--;
                }
                break;
            case SDL_SCANCODE_J:
                if (lvl_w > 67) {
                    tileset->remove_col_right();
                    lvl_w--;
                }
                break;
            case SDL_SCANCODE_L:
                if (lvl_w > 67) {
                    camera->move_left();
                    tileset->remove_col_left();
                    lvl_w--;
                }
                break;
            case SDL_SCANCODE_RIGHTBRACKET:
                placing = (PlacingType) ((placing + 1) % NUM_PLACING_TYPES);
                break;
            case SDL_SCANCODE_LEFTBRACKET:
                placing = (PlacingType) ((placing + NUM_PLACING_TYPES - 1) % NUM_PLACING_TYPES);
            default:
                break;
            }
            break;
        default:
            break;
        }
        camera->handle_event(e);
        tileset->handle_event(e, game->screen_width, game->screen_height, camera->get_rect(), placing);
    }
}

void LevelEditor::update()
{
    SDL_SetRenderDrawColor(game->rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(game->rend);

    camera->update();
    border->update(lvl_w, lvl_h);
    grid->update(lvl_w, lvl_h);
}

void LevelEditor::draw()
{
    int scr_w = game->screen_width;
    int scr_h = game->screen_height;
    SDL_Rect cam_rect = camera->get_rect();

    tileset->draw(scr_w, scr_h, cam_rect);
    border->draw(scr_w, scr_h, cam_rect);
    grid->draw(scr_w, scr_h, cam_rect);
    draw_UI(scr_w, scr_h);

    SDL_RenderPresent(game->rend);
}

void LevelEditor::draw_UI(int scr_w, int scr_h)
{
    SDL_SetRenderDrawColor(game->rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_Color black = {0,0,0};
    std::string placing_str = "placing:\n";
    switch (placing)
    {
    case PLACING_TILES_BW:
        placing_str += "tiles (black/white)";
        break;
    case PLACING_TILES_CS:
        placing_str += "tiles (clear/solid)";
        break;
    case PLACING_SPIKES_FC:
        placing_str += "spikes (floor/ceiling)";
        break;
    case PLACING_SPIKES_RL:
        placing_str += "spikes (left/right wall)";
        break;
    case PLACING_SLOPE_2:
        placing_str += "slope 1/2 (up/down)";
        break;
    case PLACING_PLATFORMS:
        placing_str += "platforms (black/white)";
        break;
    case PLACING_BLOCKS:
        placing_str += "blocks";
        break;
    case PLACING_KEYS:
        placing_str += "keys";
        break;
    case PLACING_DOORS:
        placing_str += "doors";
        break;
    case PLACING_BUTTONS:
        placing_str += "buttons";
        break;
    case PLACING_SPRINGS:
        placing_str += "springs";
        break;
    case PLACING_MOVING_PLATFORMS:
        placing_str += "moving platforms (black/white)";
        break;
    case PLACING_SHIFTBLOCKS:
        placing_str += "shiftblocks";
        break;
    case PLACING_DELETE:
        placing_str = "deleting";
        break;
    default:
        break;
    }
    SDL_Surface* text_surf = TTF_RenderText_Solid(game->font, placing_str.c_str(), black);
    SDL_Texture* text_tex = SDL_CreateTextureFromSurface(game->rend, text_surf);
    int w, h;
    SDL_QueryTexture(text_tex, NULL, NULL, &w, &h);
    SDL_Rect render_rect = {scr_w / 2 - w / 2, 30, w, h};
    SDL_Rect bg_render_rect = {scr_w / 2 - w / 2 - 20, 20, w + 40, h + 20};
    SDL_SetRenderDrawColor(game->rend, 200, 200, 200, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(game->rend, &bg_render_rect);
    SDL_RenderCopy(game->rend, text_tex, NULL, &render_rect);
}

std::string get_str(std::string prompt, std::string result)
{
    while (1) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type)
            {
            case SDL_QUIT:
                game->quit();
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.scancode)
                {
                case SDL_SCANCODE_0:
                    result += '0';
                    break;
                case SDL_SCANCODE_1:
                    result += '1';
                    break;
                case SDL_SCANCODE_2:
                    result += '2';
                    break;
                case SDL_SCANCODE_3:
                    result += '3';
                    break;
                case SDL_SCANCODE_4:
                    result += '4';
                    break;
                case SDL_SCANCODE_5:
                    result += '5';
                    break;
                case SDL_SCANCODE_6:
                    result += '6';
                    break;
                case SDL_SCANCODE_7:
                    result += '7';
                    break;
                case SDL_SCANCODE_8:
                    result += '8';
                    break;
                case SDL_SCANCODE_9:
                    result += '9';
                    break;
                case SDL_SCANCODE_RETURN:
                    return result;
                    break;
                case SDL_SCANCODE_BACKSPACE:
                case SDL_SCANCODE_DELETE:
                    result.pop_back();
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }
        SDL_SetRenderDrawColor(game->rend, 0, 10, 30, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(game->rend);
        SDL_Color color = {255,255,255};
        std::string final_prompt = prompt + ": " + result;
        SDL_Surface* prompt_surf = TTF_RenderText_Solid(game->font, final_prompt.c_str(), color);
        SDL_Texture* prompt_tex = SDL_CreateTextureFromSurface(game->rend, prompt_surf);
        int w, h;
        SDL_QueryTexture(prompt_tex, NULL, NULL, &w, &h);
        int scr_w = game->screen_width;
        int scr_h = game->screen_height;
        SDL_Rect render_rect = {scr_w / 2 - w / 2, scr_h / 2 - h / 2, w, h};
        SDL_RenderCopy(game->rend, prompt_tex, NULL, &render_rect);
        SDL_RenderPresent(game->rend);
    }
}

bool get_yes_no(std::string prompt)
{
    SDL_SetRenderDrawColor(game->rend, 0, 10, 30, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(game->rend);

    std::string final_prompt = prompt + " (y/n)";
    SDL_Color color = {255,255,255};
    SDL_Surface* prompt_surf = TTF_RenderText_Solid(game->font, final_prompt.c_str(), color);
    SDL_Texture* prompt_tex = SDL_CreateTextureFromSurface(game->rend, prompt_surf);
    int w, h;
    SDL_QueryTexture(prompt_tex, NULL, NULL, &w, &h);
    int scr_w = game->screen_width;
    int scr_h = game->screen_height;
    SDL_Rect render_rect = {scr_w / 2 - w / 2, scr_h / 2 - h / 2, w, h};
    SDL_RenderCopy(game->rend, prompt_tex, NULL, &render_rect);
    SDL_RenderPresent(game->rend);

    while (1) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type)
            {
            case SDL_QUIT:
                game->quit();
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    return false;
                    break;
                case SDL_SCANCODE_Y:
                    return true;
                    break;
                case SDL_SCANCODE_N:
                    return false;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

std::vector<std::vector<std::string> > LevelEditor::output_arr(std::vector<std::vector<TileType> > tiles)
{
    std::vector<std::vector<std::string> > result;
    for (int i = 0; i < tiles.size(); i++) {
        std::vector<std::string> new_row;
        for (int j = 0; j < tiles[0].size(); j++) {
            char new_tile[3];
            snprintf(new_tile, 3, "%02d", (int) tiles[i][j]);
            std::string new_tile_str(new_tile);
            new_row.push_back(new_tile_str);
        }
        result.push_back(new_row);
    }
    return result;
}

// this determines the spring velocity from the ideal spring height
static double get_v0(int x)
{
    double height = TILE_WIDTH*x;
    double v0 = pow((float) height * 2.* GRAVITY,.5);
    return v0;
}

void LevelEditor::write_level()
{
    std::vector<std::vector<std::string> > result = output_arr(tileset->tiles);

    if (m_lvl_num == -1) {
        m_lvl_num = atoi(get_str("level number", std::to_string(m_lvl_num)).c_str());
    }

    char lvl_cstr[5];
    snprintf(lvl_cstr, 5, "%d-%02d", m_zone_num, m_lvl_num);
    std::string lvl_str(lvl_cstr);
    lvl_cstr[1] = '\0';
    std::string zone_str(lvl_cstr);
    std::string path = "resources/level-files/"+zone_str+"/level"+lvl_str+".lvl";
    std::ofstream level_file(path.c_str());

    level_file << lvl_w << " " << lvl_h << std::endl;

    for (int i = 0; i < lvl_h; i++) {
        for (int j = 0; j < lvl_w; j++) {
            level_file << result[i][j] << " ";
        }
        level_file << std::endl;
    }
    std::vector<EditorObject> objs = tileset->objs;
    level_file << objs.size() << std::endl;
    for (int i = 0; i < objs.size(); i++) {

        level_file << objs[i].type << " ";
        switch (objs[i].type)
        {
        case OBJECT_SPRING:
            level_file << objs[i].x << " " << objs[i].y << " " << objs[i].color << " " << get_v0(objs[i].spring_height);
            break;
        case OBJECT_MOVING_PLATFORM:
            level_file << objs[i].color << " " << objs[i].w << " " << objs[i].h << " ";
            level_file << objs[i].x << " " << objs[i].y << " " << objs[i].x2 << " " << objs[i].y2 << " ";
            level_file << objs[i].pause_length << " " << objs[i].move_length;
            break;
        case OBJECT_SHIFTBLOCK:
            level_file << objs[i].x << " " << objs[i].y << " " << objs[i].w << " " << objs[i].h;
            break;
        default:
            break;
        }
        level_file << std::endl;
    }

    level_file.close();
    game->pop_state();
}
