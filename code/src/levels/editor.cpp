#include <algorithm>
#include <editor.hpp>

EditorCamera::EditorCamera(int w, int h, int lvlw, int lvlh)
{
    center_rect.x = lvlw/2;
    center_rect.y = lvlh/2;
    center_rect.w = w;
    center_rect.h = h;
    true_rect.x = center_rect.x - center_rect.w/2;
    true_rect.y = center_rect.y - center_rect.h/2;
    true_rect.w = w;
    true_rect.h = h;
}

void EditorCamera::handle_event(SDL_Event e)
{
    switch (e.type) {
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode) {
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
                    zoomout = true;
                    break;
                default:
                    break;
            }
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
                    zoomout = false;
                    break;
                default:
                    break;
            }
    }
}

void EditorCamera::update(int w, int h, int scr_w, int scr_h)
{
    if (up) {
        center_rect.y -= EDITOR_CAMERA_SPEED;
    }
    if (down) {
        center_rect.y += EDITOR_CAMERA_SPEED;
    }
    if (left) {
        center_rect.x -= EDITOR_CAMERA_SPEED;
    }
    if (right) {
        center_rect.x += EDITOR_CAMERA_SPEED;
    }
    if (zoomin) {
        if (center_rect.w > TILE_WIDTH*4) {
            center_rect.w /= 1.1;
            center_rect.h = center_rect.w * ((float) scr_h / (float) scr_w);
        }
    }
    if (zoomout) {
        center_rect.w *= 1.1;
        center_rect.h = center_rect.w * ((float) scr_h / (float) scr_w);
    }
    true_rect.x = center_rect.x - center_rect.w/2;
    true_rect.y = center_rect.y - center_rect.h/2;
    true_rect.w = w;
    true_rect.h = h;
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

void Border::draw(int scr_w, int scr_h, SDL_Rect cam_rect, Engine* game)
{
    int x1 = (rect.x - cam_rect.x) / (cam_rect.w / (float) scr_w);
    int y1 = (rect.y - cam_rect.y) / (cam_rect.h / (float) scr_h);
    int x2 = x1 + rect.w * (scr_w / cam_rect.w);
    int y2 = y1 + rect.h * (scr_h / cam_rect.h);
    SDL_Point points[4] = {{x1,y1},{x2,y1},{x2,y2},{x2,y2}};
    SDL_SetRenderDrawColor(game->rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(game->rend, points, 4);
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

void Gridlines::draw(int scr_w, int scr_h, SDL_Rect cam_rect, Engine* game)
{
    for (int i = 1; i < width; i++) {
        int x = (i*TILE_WIDTH - cam_rect.x) / (cam_rect.w / (float) scr_w);
        int y1 = (-cam_rect.y) / (cam_rect.h / (float) scr_h);
        int y2 = (height*TILE_WIDTH - cam_rect.y) / (cam_rect.h / (float) scr_h);
        SDL_SetRenderDrawColor(game->rend, 0, 200, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(game->rend, x, y1, x, y2);
    }
    for (int i = 1; i < width; i++) {
        int y = (i*TILE_WIDTH - cam_rect.y) / (cam_rect.h / (float) scr_h);
        int x1 = (-cam_rect.x) / (cam_rect.w / (float) scr_w);
        int x2 = (width*TILE_WIDTH - cam_rect.x) / (cam_rect.w / (float) scr_w);
        SDL_SetRenderDrawColor(game->rend, 0, 200, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(game->rend, x1, y, x2, y);
    }
}

Tileset::Tileset(int w, int h, int* tiles_arg=NULL, std::vector<Object> objs_arg=std::vector<Object>())
{
    clicked = 0;
    click_x = click_y = 0;
    width = w;
    height = h;
    if (tiles_arg == NULL) {
        tiles = (int*) malloc(sizeof(int) * height * width);
        for (int i = 0; i < height*width; i++) {
            tiles[i] = COLOR_WHITE;
        }
    } else {
        tiles = tiles_arg;
    }
    objs = objs_arg;
}

void Tileset::draw(int scr_w, int scr_h, SDL_Rect cam_rect, Engine* game)
{
    // first we will draw all of the tiles
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (tiles[i*width + j] == COLOR_BLACK) {
                int x1 = (j*TILE_WIDTH - cam_rect.x) / ((float) cam_rect.w / (float) scr_w);
                int x2 = ((j+1)*TILE_WIDTH - cam_rect.x) / ((float) cam_rect.w / (float) scr_w);
                int y1 = (i*TILE_WIDTH - cam_rect.y) / ((float) cam_rect.h / (float) scr_h);
                int y2 = ((i+1)*TILE_WIDTH - cam_rect.y) / ((float) cam_rect.h / (float) scr_h);
                SDL_Rect to_draw;
                to_draw.x = x1;
                to_draw.y = y1;
                to_draw.w = x2-x1;
                to_draw.h = y2-y1;
                SDL_SetRenderDrawColor(game->rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawRect(game->rend, &to_draw);
            }
            if (clicked && click_x == j && click_y == i) {
                int x1 = (j*TILE_WIDTH - cam_rect.x) / ((float) cam_rect.w / (float) scr_w);
                int x2 = ((j+1)*TILE_WIDTH - cam_rect.x) / ((float) cam_rect.w / (float) scr_w);
                int y1 = (i*TILE_WIDTH - cam_rect.y) / ((float) cam_rect.h / (float) scr_h);
                int y2 = ((i+1)*TILE_WIDTH - cam_rect.y) / ((float) cam_rect.h / (float) scr_h);
                SDL_Rect to_draw;
                to_draw.x = x1;
                to_draw.y = y1;
                to_draw.w = x2-x1;
                to_draw.h = y2-y1;
                if (clicked_color == COLOR_BLACK) {
                    SDL_SetRenderDrawColor(game->rend, 0, 0, 255, SDL_ALPHA_OPAQUE);
                } else {
                    SDL_SetRenderDrawColor(game->rend, 255, 255, 0, SDL_ALPHA_OPAQUE);
                }
                SDL_RenderDrawRect(game->rend, &to_draw);
            }
        }
    }

    // next we will draw all of the objects... eventually, bc we don't support textures yet
}

void Tileset::handle_event(SDL_Event e, int scr_w, int scr_h, SDL_Rect cam_rect, PlacingType placing)
{
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
        case PLACING_TILES:
            if (!clicked) {
                // if we haven't clicked before, save the click
                click_x = x1;
                click_y = y1;
                clicked = true;
                clicked_color = (e.button.button == SDL_BUTTON_LEFT) ? COLOR_BLACK : COLOR_WHITE;
            } else {
                // otherwise fill the rectangle that our clicks make!
                fill_rect(clicked_color, x1, y1);
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
        default:
            Color placing_color = e.button.button == SDL_BUTTON_LEFT ? COLOR_BLACK : COLOR_WHITE;
            if (tiles[y1 * width + x1] == placing_color) {
                printf("\a");
            }
            if (placing == PLACING_CHARS || placing == PLACING_LEVEL_ENDS) {
                for (int i = 0; i < objs.size(); i++) {
                    if (objs[i].type == placing && objs[i].color == placing_color) {
                        objs.erase(objs.begin() + i);
                    }
                }
                Object new_obj;
                new_obj.x = x1;
                new_obj.y = y1;
                new_obj.type = placing;
                new_obj.color = placing_color;
                objs.push_back(new_obj);
            }
            break;
        }
    }
}

void Tileset::fill_rect(Color color, int x, int y)
{
    int x1 = std::min(click_x, x);
    int x2 = std::max(click_x, x);
    int y1 = std::min(click_y, y);
    int y2 = std::max(click_y, y);
    for (int i = y1; i <= y2; i++) {
        for (int j = x1; j <= x2; j++) {
            tiles[i*width + j] = color;
        }
    }
    clicked = false;
}

void Tileset::add_row_top()
{
    height++;
    tiles = (int*) realloc(tiles, sizeof(int) * height * width);
    memcpy(tiles, tiles + width, sizeof(int) * (height-1) * width);
    for (int i = 0; i < width; i++) {
        tiles[i] = COLOR_WHITE;
    }
}

void Tileset::remove_row_top()
{
    memcpy(tiles + width, tiles, sizeof(int) * (height-1) * width);
    height--;
    tiles = (int*) realloc(tiles, sizeof(int) * height * width);
}

void Tileset::add_row_bottom()
{
    height++;
    tiles = (int*) realloc(tiles, sizeof(int) * height * width);
    for (int i = 0; i < width; i++) {
        tiles[height-1 * width + i] = COLOR_WHITE;
    }
}

void Tileset::remove_row_bottom()
{
    height--;
    tiles = (int*) realloc(tiles, sizeof(int) * height * width);
}

void Tileset::add_col_left()
{
    width++;
    tiles = (int*) realloc(tiles, sizeof(int) * height * width);
    for (int i = height - 1; i >= 0; i--) {
        memcpy(tiles + i * (width-1), tiles + i * width + 1, sizeof(int) * (width-1));
        tiles[i*width] = COLOR_WHITE;
    }
}

void Tileset::remove_col_left()
{
    for (int i = 0; i < height; i++) {
        memcpy(tiles + i * width + 1, tiles + i * (width-1), sizeof(int) * (width-1));
    }
    width--;
    tiles = (int*) realloc(tiles, sizeof(int) * height * width);
}

void Tileset::add_col_right()
{
    width++;
    tiles = (int*) realloc(tiles, sizeof(int) * height * width);
    for (int i = height-1; i >= 0; i--) {
        memcpy(tiles + i * (width-1), tiles + i * width, sizeof(int) * (width-1));
        tiles[i * width + (width-1)] = COLOR_WHITE;
    }
}

void Tileset::remove_col_right()
{
    for (int i = 0; i < height; i++) {
        memcpy(tiles + i * width, tiles + i * (width - 1), sizeof(int) * (width-1));
    }
    width--;
    tiles = (int*) realloc(tiles, sizeof(int) * height * width);
}

void Editor::init(Engine* game)
{

}

void Editor::cleanup()
{

}

void Editor::handle_events(Engine* game)
{

}

void Editor::update(Engine* game)
{

}

void Editor::draw(Engine* game)
{

}

void Editor::draw_UI(int scr_w, int scr_h, PlacingType placing)
{

}

std::string Editor::get_str(Engine* game, std::string prompt)
{

}

bool Editor::get_yes_no(Engine* game, std::string prompt)
{

}

int* Editor::output_arr(int w, int h, int* tiles)
{

}
