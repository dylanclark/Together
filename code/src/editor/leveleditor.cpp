#include <algorithm>
#include <cstdlib>
#include <limits>

#include <editor.hpp>
#include <levels.hpp>
#include <tiles.hpp>

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

void EditorCamera::update(Engine* game)
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

void Border::draw(int scr_w, int scr_h, SDL_Rect cam_rect, SDL_Renderer* rend)
{
    int x = (rect.x - cam_rect.x) / ((float) cam_rect.w / (float) scr_w);
    int y = (rect.y - cam_rect.y) / ((float) cam_rect.h / (float) scr_h);
    int w = rect.w * ((float) scr_w / (float) cam_rect.w);
    int h = rect.h * ((float) scr_h / (float) cam_rect.h);
    SDL_Rect to_draw = {x, y, w, h};
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
            int x1 = (j*TILE_WIDTH - cam_rect.x) / ((float) cam_rect.w / (float) scr_w);
            int x2 = ((j+1)*TILE_WIDTH - cam_rect.x) / ((float) cam_rect.w / (float) scr_w);
            int y1 = (i*TILE_WIDTH - cam_rect.y) / ((float) cam_rect.h / (float) scr_h);
            int y2 = ((i+1)*TILE_WIDTH - cam_rect.y) / ((float) cam_rect.h / (float) scr_h);
            SDL_Rect to_draw;
            to_draw.x = x1;
            to_draw.y = y1;
            to_draw.w = x2-x1;
            to_draw.h = y2-y1;
            switch (tiles[i][j])
            {
            case COLOR_WHITE:
                SDL_SetRenderDrawColor(rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
                break;
            case COLOR_BLACK:
                SDL_SetRenderDrawColor(rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
                break;
            case COLOR_GLASS:
                SDL_SetRenderDrawColor(rend, 100, 100, 100, SDL_ALPHA_OPAQUE);
                break;
            case COLOR_BRICK:
                SDL_SetRenderDrawColor(rend, 0, 0, 150, SDL_ALPHA_OPAQUE);
                break;
            default:
                SDL_SetRenderDrawColor(rend, 255, 0, 0, SDL_ALPHA_OPAQUE);
                break;
            }
            if (clicked && click_x == j && click_y == i) {
                switch (clicked_color)
                {
                case COLOR_BLACK:
                    SDL_SetRenderDrawColor(rend, 0, 0, 255, SDL_ALPHA_OPAQUE);
                    break;
                case COLOR_WHITE:
                    SDL_SetRenderDrawColor(rend, 255, 255, 0, SDL_ALPHA_OPAQUE);
                    break;
                case COLOR_GLASS:
                    SDL_SetRenderDrawColor(rend, 0, 255, 0, SDL_ALPHA_OPAQUE);
                    break;
                case COLOR_BRICK:
                    SDL_SetRenderDrawColor(rend, 255, 100, 0, SDL_ALPHA_OPAQUE);
                    break;
                default:
                    break;
                }
            }
            SDL_RenderFillRect(rend, &to_draw);
        }
    }
    for (int i = 0; i < objs.size(); i++) {
        int x = (objs[i].x*TILE_WIDTH - cam_rect.x) / ((float) cam_rect.w / (float) scr_w) + 1;
        int y = (objs[i].y*TILE_WIDTH - cam_rect.y) / ((float) cam_rect.h / (float) scr_h) + 1;
        int w = TILE_WIDTH * ((float) scr_w / (float) cam_rect.w);
        int h = TILE_WIDTH * ((float) scr_h / (float) cam_rect.h);
        SDL_Rect render_rect = {x, y, w, h};

        if (objs[i].type == PLACING_EXITS) {
            render_rect.w *= (2 + 4*(objs[i].dir == EXIT_UP || objs[i].dir == EXIT_DOWN));
            render_rect.h *= (2 + 4*(objs[i].dir == EXIT_LEFT || objs[i].dir == EXIT_RIGHT));
            SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(rend, 150, 150, 0, 150);
            SDL_RenderFillRect(rend, &render_rect);
        }
    }
    // next we will draw all of the objects... eventually, bc we don't support textures yet
}

void Tileset::handle_event(Engine* game, SDL_Event e, int scr_w, int scr_h, SDL_Rect cam_rect, PlacingType placing, bool lshift, bool rshift)
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
                if (lshift) {
                    clicked_color = COLOR_GLASS;
                } else if (rshift) {
                    clicked_color = COLOR_BRICK;
                } else {
                    clicked_color = (e.button.button == SDL_BUTTON_LEFT) ? COLOR_BLACK : COLOR_WHITE;
                }
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
            if (placing == PLACING_EXITS) {
                Object new_obj;
                new_obj.x = x1;
                new_obj.y = y1;
                new_obj.type = placing;
                new_obj.color = placing_color;
                if (x1 == 0) {
                    new_obj.dir = EXIT_LEFT;
                } else if (x1 == width-2) {
                    new_obj.dir = EXIT_RIGHT;
                } else if (y1 == 0) {
                    new_obj.dir = EXIT_UP;
                } else if (y1 == height-2) {
                    new_obj.dir = EXIT_DOWN;
                } else {
                    printf("\a");
                    break;
                }
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

void LevelEditor::init(Engine* game)
{
    lshift = rshift = false;
    bool loading;
    if (m_lvl_num == -1) {
        m_zone_num = atoi(get_str(game, "zone number").c_str());
        m_lvl_num = atoi(get_str(game, "level number").c_str());
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

        std::vector<std::vector<int> > tiles;
        std::vector<Object> objs;

        // load all the tiles
        int cur_tile;
        for (int i = 0; i < lvl_h; i++) {
            tiles.push_back(std::vector<int>());
            for (int j = 0; j < lvl_w; j++) {
                level_file >> cur_tile;
                tiles[i].push_back(cur_tile);
            }
        }

        // load all level exits
        int num_exits;
        level_file >> num_exits;
        for (int i = 0; i < num_exits; i++) {
            Object new_exit;
            level_file >> new_exit.x;
            level_file >> new_exit.y;
            int dir;
            level_file >> dir;
            new_exit.dir = (ExitDir) dir;
            new_exit.type = PLACING_EXITS;
            objs.push_back(new_exit);
        }
        // TODO: load all objects!
        tileset = new Tileset(lvl_w, lvl_h, tiles, objs);
    }
    placing = PLACING_TILES;

    border = new Border(lvl_w, lvl_h);
    grid = new Gridlines(lvl_w, lvl_h);
    camera = new EditorCamera(game->screen_width, game->screen_height, lvl_w*TILE_WIDTH/2, lvl_h*TILE_WIDTH/2);
}

void LevelEditor::cleanup()
{

}

void LevelEditor::handle_events(Engine* game)
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
            case SDL_SCANCODE_LSHIFT:
                lshift = true;
                break;
            case SDL_SCANCODE_RSHIFT:
                rshift = true;
                break;
            case SDL_SCANCODE_ESCAPE:
                game->pop_state();
                break;
            case SDL_SCANCODE_RETURN:
                write_level(game);
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
        case SDL_KEYUP:
            switch (e.key.keysym.scancode)
            {
            case SDL_SCANCODE_LSHIFT:
                lshift = false;
                break;
            case SDL_SCANCODE_RSHIFT:
                rshift = false;
                break;
            default:
                break;
            }
        default:
            break;
        }
        camera->handle_event(e);
        tileset->handle_event(game, e, game->screen_width, game->screen_height, camera->get_rect(), placing, lshift, rshift);
    }
}

void LevelEditor::update(Engine* game)
{
    SDL_SetRenderDrawColor(game->rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(game->rend);

    camera->update(game);
    border->update(lvl_w, lvl_h);
    grid->update(lvl_w, lvl_h);
}

void LevelEditor::draw(Engine* game)
{
    int scr_w = game->screen_width;
    int scr_h = game->screen_height;
    SDL_Rect cam_rect = camera->get_rect();

    tileset->draw(scr_w, scr_h, cam_rect, game->rend);
    border->draw(scr_w, scr_h, cam_rect, game->rend);
    grid->draw(scr_w, scr_h, cam_rect, game->rend);
    draw_UI(game, scr_w, scr_h);

    SDL_RenderPresent(game->rend);
}

void LevelEditor::draw_UI(Engine* game, int scr_w, int scr_h)
{
    SDL_SetRenderDrawColor(game->rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_Color black = {0,0,0};
    std::string placing_str = "placing:\n";
    switch (placing)
    {
    case PLACING_TILES:
        placing_str += "tiles";
        break;
    case PLACING_EXITS:
        placing_str += "exits";
        break;
    case PLACING_CRATES:
        placing_str += "crates";
        break;
    case PLACING_BUTTONS:
        placing_str += "buttons";
        break;
    case PLACING_SPRINGS:
        placing_str += "springs";
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

std::string get_str(Engine* game, std::string prompt, std::string result)
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

bool get_yes_no(Engine* game, std::string prompt)
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

std::vector<std::vector<std::string> > LevelEditor::output_arr(std::vector<std::vector<int> > tiles)
{
    std::vector<std::vector<std::string> > result;
    for (int i = 0; i < tiles.size(); i++) {
        std::vector<std::string> new_row;
        for (int j = 0; j < tiles[0].size(); j++) {
            char new_tile[3];
            snprintf(new_tile, 3, "%02d", tiles[i][j]);
            std::string new_tile_str(new_tile);
            new_row.push_back(new_tile_str);
        }
        result.push_back(new_row);
    }
    return result;
}

void LevelEditor::write_level(Engine* game)
{
    std::vector<std::vector<std::string> > result = output_arr(tileset->tiles);

    if (m_lvl_num == -1) {
        m_lvl_num = atoi(get_str(game, "level number", std::to_string(m_lvl_num)).c_str());
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
    std::vector<Object> objs = tileset->objs;
    std::vector<Object> exits;
    int num_exits = 0;
    for (int i = 0; i < objs.size(); i++) {
        if (objs[i].type == PLACING_EXITS) {
            num_exits++;
            exits.push_back(objs[i]);
        }
    }
    level_file << num_exits << std::endl;
    for (int i = 0; i < num_exits; i++) {
        level_file << exits[i].x << " " << exits[i].y << " " << exits[i].dir << std::endl;
    }
    level_file.close();
    game->pop_state();
}
