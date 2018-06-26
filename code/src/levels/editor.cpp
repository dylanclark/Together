#include <algorithm>
#include <cstdlib>

#include <editor.hpp>
#include <levels.hpp>

EditorCamera::EditorCamera(int scr_w, int scr_h, int lvl_w, int lvl_h)
{
    center_rect.x = lvl_w/2;
    center_rect.y = lvl_h/2;
    center_rect.w = scr_w;
    center_rect.h = scr_h;
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

void Border::draw(int scr_w, int scr_h, SDL_Rect cam_rect, SDL_Renderer* rend)
{
    int x1 = (rect.x - cam_rect.x) / ((float) cam_rect.w / (float) scr_w);
    int y1 = (rect.y - cam_rect.y) / ((float) cam_rect.h / (float) scr_h);
    int x2 = x1 + rect.w * ((float) scr_w / (float) cam_rect.w);
    int y2 = y1 + rect.h * ((float) scr_h / (float) cam_rect.h);
    SDL_Rect to_draw = {x1, y1, x2-x1, y2-y1};
    SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(rend, &to_draw);
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

void Gridlines::draw(int scr_w, int scr_h, SDL_Rect cam_rect, SDL_Renderer* rend)
{
    for (int i = 1; i < width; i++) {
        int x = (i*TILE_WIDTH - cam_rect.x) / (cam_rect.w / (float) scr_w);
        int y1 = (-cam_rect.y) / (cam_rect.h / (float) scr_h);
        int y2 = (height*TILE_WIDTH - cam_rect.y) / (cam_rect.h / (float) scr_h);
        SDL_SetRenderDrawColor(rend, 0, 200, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(rend, x, y1, x, y2);
    }
    for (int i = 1; i < height; i++) {
        int y = (i*TILE_WIDTH - cam_rect.y) / (cam_rect.h / (float) scr_h);
        int x1 = (-cam_rect.x) / (cam_rect.w / (float) scr_w);
        int x2 = (width*TILE_WIDTH - cam_rect.x) / (cam_rect.w / (float) scr_w);
        SDL_SetRenderDrawColor(rend, 0, 200, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(rend, x1, y, x2, y);
    }
}

Tileset::Tileset(int w, int h, std::vector<std::vector<int> > tiles_arg=std::vector<std::vector<int> >(), std::vector<Object> objs_arg=std::vector<Object>())
{
    clicked = 0;
    click_x = click_y = 0;
    width = w;
    height = h;
    if (tiles_arg.size() == 0) {
        for (int i = 0; i < height; i++) {
            tiles.push_back(std::vector<int>());
            for (int j = 0; j < width; j++) {
                tiles[i].push_back(COLOR_WHITE);
            }
        }
    } else {
        tiles = tiles_arg;
    }
    objs = objs_arg;
}

void Tileset::draw(int scr_w, int scr_h, SDL_Rect cam_rect, SDL_Renderer* rend)
{
    // first we will draw all of the tiles
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (tiles[i][j] == COLOR_BLACK) {
                int x1 = (j*TILE_WIDTH - cam_rect.x) / ((float) cam_rect.w / (float) scr_w);
                int x2 = ((j+1)*TILE_WIDTH - cam_rect.x) / ((float) cam_rect.w / (float) scr_w);
                int y1 = (i*TILE_WIDTH - cam_rect.y) / ((float) cam_rect.h / (float) scr_h);
                int y2 = ((i+1)*TILE_WIDTH - cam_rect.y) / ((float) cam_rect.h / (float) scr_h);
                SDL_Rect to_draw;
                to_draw.x = x1;
                to_draw.y = y1;
                to_draw.w = x2-x1;
                to_draw.h = y2-y1;
                SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(rend, &to_draw);
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
                    SDL_SetRenderDrawColor(rend, 0, 0, 255, SDL_ALPHA_OPAQUE);
                } else {
                    SDL_SetRenderDrawColor(rend, 255, 255, 0, SDL_ALPHA_OPAQUE);
                }
                SDL_RenderFillRect(rend, &to_draw);
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
            if (tiles[y1][x1] == placing_color) {
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
            tiles[i][j] = color;
        }
    }
    clicked = false;
}

void Tileset::add_row_top()
{
    height++;
    std::vector<int> new_row = std::vector<int>();
    for (int j = 0; j < width; j++) {
        new_row.push_back(COLOR_WHITE);
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
    std::vector<int> new_row = std::vector<int>();
    for (int j = 0; j < width; j++) {
        new_row.push_back(COLOR_WHITE);
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
        tiles[i].insert(tiles[i].begin(), COLOR_WHITE);
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
        tiles[i].push_back(COLOR_WHITE);
    }
}

void Tileset::remove_col_right()
{
    width--;
    for (int i = 0; i < height; i++) {
        tiles[i].pop_back();
    }
}

void Editor::init(Engine* game)
{
    my_font = TTF_OpenFont("resources/fonts/slkscr.ttf", 24);

    bool loading;
    if (get_yes_no(game, "edit this level?")) {
        loading = true;
    } else {
        if (get_yes_no(game, "edit another level?")) {
            loading = true;
            m_lvl_num = atoi(get_str(game, "level to edit").c_str());
        } else {
            loading = false;
            lvl_w = atoi(get_str(game, "level width").c_str());
            lvl_h = atoi(get_str(game, "level height").c_str());
            tileset = new Tileset(lvl_w, lvl_h);
        }
    }
    if (loading) {
        printf("debug");
        char lvl_num_cstr[3];
        snprintf(lvl_num_cstr, 3, "%02d", m_lvl_num);
        std::string lvl_num_str(lvl_num_cstr);

        std::string path = "resources/level-files/level"+lvl_num_str+".lvl";
        std::ifstream level_file(path.c_str());

        int r, g, b;
        level_file >> r;
        level_file >> g;
        level_file >> b;

        level_file >> lvl_w;
        level_file >> lvl_h;

        std::vector<std::vector<int> > tiles;
        std::vector<Object> objs;

        // load all the tiles
        int cur_tile;
        for (int i = 0; i < lvl_h; i++) {
            tiles.push_back(std::vector<int>());
            for (int j = 0; j < lvl_w; j++) {
                level_file >> cur_tile;
                tiles[i].push_back(cur_tile < 15 ? COLOR_BLACK : COLOR_WHITE);
            }
        }

        // load the characters and their level ends!
        int num_chars;
        level_file >> num_chars;
        for (int i = 0; i < num_chars; i++) {
            Object new_char, new_lvl_end;
            level_file >> new_char.x;
            level_file >> new_char.y;
            level_file >> new_lvl_end.x;
            level_file >> new_lvl_end.y;
            new_char.type = PLACING_CHARS;
            new_lvl_end.type = PLACING_LEVEL_ENDS;
            new_char.color = new_lvl_end.color = (Color) i;
            objs.push_back(new_char);
            objs.push_back(new_lvl_end);
        }
        // TODO: load all objects!
        tileset = new Tileset(lvl_w, lvl_h, tiles, objs);
    }
    placing = PLACING_TILES;

    border = new Border(lvl_w, lvl_h);
    grid = new Gridlines(lvl_w, lvl_h);
    camera = new EditorCamera(game->screen_width, game->screen_height, lvl_w*TILE_WIDTH, lvl_h*TILE_WIDTH);
}

void Editor::cleanup()
{
    printf("leaving editor\n");
}

void Editor::handle_events(Engine* game)
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
                if (lvl_h > 2) {
                    tileset->remove_row_bottom();
                    lvl_h--;
                }
                break;
            case SDL_SCANCODE_K:
                if (lvl_h > 2) {
                    camera->move_up();
                    tileset->remove_row_top();
                    lvl_h--;
                }
                break;
            case SDL_SCANCODE_J:
                if (lvl_w > 2) {
                    tileset->remove_col_right();
                    lvl_w--;
                }
                break;
            case SDL_SCANCODE_L:
                if (lvl_w > 2) {
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
        default:
            break;
        }
        camera->handle_event(e);
        tileset->handle_event(e, game->screen_width, game->screen_height, camera->get_rect(), placing);
    }
}

void Editor::update(Engine* game)
{
    SDL_SetRenderDrawColor(game->rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(game->rend);

    camera->update(lvl_w, lvl_h, game->screen_width, game->screen_height);
    border->update(lvl_w, lvl_h);
    grid->update(lvl_w, lvl_h);
}

void Editor::draw(Engine* game)
{
    int scr_w = game->screen_width;
    int scr_h = game->screen_height;
    SDL_Rect cam_rect = camera->get_rect();

    tileset->draw(scr_w, scr_h, cam_rect, game->rend);
    border->draw(scr_w, scr_h, cam_rect, game->rend);
    grid->draw(scr_w, scr_h, cam_rect, game->rend);
    draw_UI(scr_w, scr_h);

    SDL_RenderPresent(game->rend);
}

void Editor::draw_UI(int scr_w, int scr_h)
{

}

std::string Editor::get_str(Engine* game, std::string prompt)
{
    std::string result = "";
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
                default:
                    break;
                }
            default:
                break;
            }
        }
        SDL_RenderClear(game->rend);
        SDL_Color black = {0,0,0};
        std::string final_prompt = prompt + ": " + result;
        SDL_Surface* prompt_surf = TTF_RenderText_Solid(my_font, final_prompt.c_str(), black);
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

bool Editor::get_yes_no(Engine* game, std::string prompt)
{
    SDL_RenderClear(game->rend);

    TTF_Font* my_font = TTF_OpenFont("resources/fonts/slkscr.ttf", 24);
    std::string final_prompt = prompt + " (y/n)";
    SDL_Color black = {0,0,0};
    SDL_Surface* prompt_surf = TTF_RenderText_Solid(my_font, final_prompt.c_str(), black);
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
                    game->change_state(new Levelstate(1));
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

int* Editor::output_arr(int w, int h, int* tiles)
{

}

void Editor::write_level()
{

}
