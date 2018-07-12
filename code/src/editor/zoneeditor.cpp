#include <editor.hpp>

LevelThumbnail::LevelThumbnail(Engine* game, int zone_num, int lvl_num, int x, int y)
{
    char lvl_cstr[5];
    snprintf(lvl_cstr, 5, "%d-%02d", zone_num, lvl_num);
    std::string lvl_str(lvl_cstr);
    lvl_cstr[1] = '\0';
    std::string zone_str(lvl_cstr);
    std::string path = "resources/level-files/"+zone_str+"/level"+lvl_str+".lvl";
    std::ifstream level_file(path.c_str());

    m_x = x;
    m_y = y;
    level_file >> m_w;
    level_file >> m_h;

    Uint32 format = SDL_GetWindowPixelFormat(game->win);
    m_tex = SDL_CreateTexture(game->rend, format, SDL_TEXTUREACCESS_TARGET, m_w*TILE_WIDTH, m_h*TILE_WIDTH);
    SDL_SetRenderTarget(game->rend, m_tex);
    printf("%d,%d\n", m_w, m_h);
    for (int i = 0; i < m_w*m_h; i++) {
        int cur_tile;
        level_file >> cur_tile;
        int color = (cur_tile >= W_BACK) ? 255 : 0;
        SDL_SetRenderDrawColor(game->rend, color, color, color, SDL_ALPHA_OPAQUE);
        SDL_Rect to_draw = {(i % m_w)*TILE_WIDTH, (i/m_w)*TILE_WIDTH, TILE_WIDTH, TILE_WIDTH};
        SDL_RenderFillRect(game->rend, &to_draw);
    }
    SDL_SetRenderTarget(game->rend, NULL);

    level_file.close();
}

SDL_Rect LevelThumbnail::get_rect()
{
    SDL_Rect res = {m_x, m_y, m_w*TILE_WIDTH, m_h*TILE_WIDTH};
    return res;
}

void LevelThumbnail::draw(SDL_Renderer* rend, SDL_Rect cam_rect, int scr_w, int scr_h)
{
    int x = (m_x - cam_rect.x) / ((float) cam_rect.w / (float) scr_w);
    int y = (m_y - cam_rect.y) / ((float) cam_rect.h / (float) scr_h);
    int w = m_w*TILE_WIDTH * ((float) scr_w / (float) cam_rect.w);
    int h = m_h*TILE_WIDTH * ((float) scr_h / (float) cam_rect.h);
    SDL_Rect render_rect = {x,y,w,h};
    SDL_RenderCopy(rend, m_tex, NULL, &render_rect);

    int thickness;
    if (selected) {
        SDL_SetRenderDrawColor(rend, 0, 200, 0, SDL_ALPHA_OPAQUE);
        thickness = 5;
    } else {
        SDL_SetRenderDrawColor(rend, 0, 0, 255, SDL_ALPHA_OPAQUE);
        thickness = 3;
    }
    SDL_Rect rect1 = {x,y,w,thickness};
    SDL_Rect rect2 = {x,y+h-thickness,w,thickness};
    SDL_Rect rect3 = {x+w-thickness,y,thickness,h};
    SDL_Rect rect4 = {x,y,thickness,h};
    SDL_RenderFillRect(rend, &rect1);
    SDL_RenderFillRect(rend, &rect2);
    SDL_RenderFillRect(rend, &rect3);
    SDL_RenderFillRect(rend, &rect4);
}

void LevelThumbnail::move(int x, int y, std::vector<LevelThumbnail*> levels)
{
    m_x = (x / TILE_WIDTH) * TILE_WIDTH;
    m_y = (y / TILE_WIDTH) * TILE_WIDTH;
    // TODO: we want to snap the level into place if we can lock exits and entrances
}

void ZoneEditor::init(Engine* game)
{
    bool loading = get_yes_no(game, "load existing zone?");
    camera = new EditorCamera(game->screen_width, game->screen_height, 0, 0);
    mousedown = false;
    selected = -1;
    edited_level = created_level = false;

    if (loading) {
        m_zone_num = atoi(get_str(game, "zone number to load").c_str());
        char zone_num_cstr[2];
        snprintf(zone_num_cstr, 2, "%d", m_zone_num);
        std::string zone_num_str(zone_num_cstr);
        std::string path = "resources/level-files/"+zone_num_str+"/zone-file";
        std::ifstream zone_file(path.c_str());

        int num_levels;
        zone_file >> num_levels;
        int x, y;

        for (int i = 0; i < num_levels; i++) {
            zone_file >> x;
            zone_file >> y;
            LevelThumbnail* new_thumbnail = new LevelThumbnail(game, m_zone_num, i, x, y);
            levels.push_back(new_thumbnail);
        }
        zone_file >> r;
        zone_file >> g;
        zone_file >> b;
    } else {
        r = g = b = 255;
    }
}

void ZoneEditor::cleanup()
{

}

void ZoneEditor::update(Engine* game)
{
    SDL_SetRenderDrawColor(game->rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(game->rend);
    if (edited_level) {
        int x, y;
        x = levels[selected]->m_x;
        y = levels[selected]->m_y;
        levels.erase(levels.begin() + selected);
        levels.insert(levels.begin() + selected, new LevelThumbnail(game, m_zone_num, selected, x, y));
        edited_level = false;
    }
    if (created_level) {
        levels.push_back(new LevelThumbnail(game, m_zone_num, selected, 0, 0));
        created_level = false;
    }
    if (mousedown) {
        for (int i = 0; i < levels.size(); i++) {
            levels[i]->selected = (selected == i);
        }
        if (selected != -1) {
            int mouse_x, mouse_y;
            SDL_GetMouseState(&mouse_x, &mouse_y);
            int scr_w = game->screen_width;
            int scr_h = game->screen_height;
            int target_x, target_y;
            SDL_Rect cam_rect = camera->get_rect();
            target_x = mouse_x * ((float) cam_rect.w / (float) scr_w) + cam_rect.x;
            target_y = mouse_y * ((float) cam_rect.h / (float) scr_h) + cam_rect.y;
            levels[selected]->move(target_x - x_offset, target_y - y_offset, levels);
        }
    }
    camera->update(game);
}

void ZoneEditor::handle_events(Engine* game)
{
    SDL_Event e;
    int scr_w = game->screen_width;
    int scr_h = game->screen_height;
    int true_x, true_y;
    bool select_valid;
    SDL_Rect cam_rect = camera->get_rect();

    while (SDL_PollEvent(&e)) {
        switch (e.type)
        {
        case SDL_QUIT:
            write_zone();
            game->quit();
            break;
        case SDL_MOUSEBUTTONDOWN:
            mousedown = true;
            // get mouse position
            true_x = (e.button.x * ((float) cam_rect.w / (float) scr_w) + cam_rect.x);
            true_y = (e.button.y * ((float) cam_rect.h / (float) scr_h) + cam_rect.y);
            select_valid = false;
            for (int i = 0; i < levels.size(); i++) {
                SDL_Rect rect = levels[i]->get_rect();
                if (true_x > rect.x && true_x < rect.x + rect.w &&
                    true_y > rect.y && true_y < rect.y + rect.h) {
                    selected = i;
                    select_valid = true;
                    x_offset = true_x - rect.x;
                    y_offset = true_y - rect.y;
                    break;
                }
            }
            if (!select_valid) {
                selected = -1;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            mousedown = false;
            break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode)
            {
            case SDL_SCANCODE_BACKSPACE:
                levels.erase(levels.begin() + selected);
                selected = -1;
                break;
            case SDL_SCANCODE_S:
                write_zone();
                break;
            case SDL_SCANCODE_N:
                selected = levels.size();
                game->push_state(new LevelEditor(m_zone_num, selected));
                created_level = true;
                break;
            case SDL_SCANCODE_E:
                if (selected != -1) {
                    game->push_state(new LevelEditor(m_zone_num, selected));
                    edited_level = true;
                    // TODO: edit selected level
                }
                break;
            case SDL_SCANCODE_L:
                // TODO: load level
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        // you can't move the camera if you're dragging something bc that's confusing lol
        if (!mousedown) {
            camera->handle_event(e);
        }
    }
}

void ZoneEditor::draw(Engine* game)
{
    SDL_Rect cam_rect = camera->get_rect();
    int scr_w = game->screen_width;
    int scr_h = game->screen_height;

    // vertical gridlines
    int x = (cam_rect.x / TILE_WIDTH) * TILE_WIDTH;
    while (x < cam_rect.x + cam_rect.w) {
        int color;
        if (x % (16*TILE_WIDTH) == 0) {
            // thick gridlines
            color = 20;
        } else if (x % (4*TILE_WIDTH) == 0) {
            // thin gridlines
            color = 200;
        } else {
            color = 240;
        }
        int y1 = 0;
        int y2 = game->screen_height;
        int screen_x = (x - cam_rect.x) / ((float) cam_rect.w / (float) scr_w);
        SDL_SetRenderDrawColor(game->rend, color, color, color, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(game->rend, screen_x, y1, screen_x, y2);

        x += TILE_WIDTH;
    }
    // horizontal gridlines
    int y = (cam_rect.y / TILE_WIDTH) * TILE_WIDTH;
    while (y < cam_rect.y + cam_rect.h) {
        int color;
        if (y % (16*TILE_WIDTH) == 0) {
            // thick gridlines
            color = 20;
        } else if (y % (4*TILE_WIDTH) == 0) {
            // thin gridlines
            color = 200;
        } else {
            color = 240;
        }
        int x1 = 0;
        int x2 = game->screen_width;
        int screen_y = (y - cam_rect.y) / ((float) cam_rect.h / (float) scr_h);
        SDL_SetRenderDrawColor(game->rend, color, color, color, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(game->rend, x1, screen_y, x2, screen_y);

        y += TILE_WIDTH;
    }

    for (int i = 0; i < levels.size(); i++) {
        levels[i]->draw(game->rend, cam_rect, game->screen_width, game->screen_height);
    }

    SDL_RenderPresent(game->rend);
}

void ZoneEditor::write_zone()
{
    char zone_num_cstr[2];
    snprintf(zone_num_cstr, 2, "%d", m_zone_num);
    std::string zone_num_str(zone_num_cstr);
    std::string path = "resources/level-files/"+zone_num_str+"/zone-file";
    std::ofstream zone_file(path.c_str());

    zone_file << levels.size() << std::endl;
    for (int i = 0; i < levels.size(); i++) {
        zone_file << levels[i]->m_x << " " << levels[i]->m_y << std::endl;
    }
    zone_file << r << " " << g << " " << b << std::endl;
}
