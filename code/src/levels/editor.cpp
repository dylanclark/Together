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

Tileset::Tileset(int w, int h, int* tiles, std::vector<Object> objs)
{

}

void Tileset::draw(Engine* game)
{

}

void Tileset::handle_event(SDL_Event e, int scr_w, int scr_h, SDL_Rect cam_rect, PlacingType placing)
{

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

}

void Tileset::add_col_right()
{

}

void Tileset::remove_col_right()
{

}

void Editor::init(Engine* game)
{

}

void Editor::cleanup(Engine* game)
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
