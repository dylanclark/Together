#include <editor.hpp>

LevelThumbnail::LevelThumbnail(int zone_num, int lvl_num)
{
    char lvl_cstr[5];
    snprintf(lvl_cstr, 5, "%d-%02d", zone_num, lvl_num);
    std::string lvl_str(lvl_cstr);
    lvl_cstr[1] = '\0';
    std::string zone_str(lvl_cstr);
    std::string path = "resources/levels/"+zone_str+"/level"+lvl_str+".lvl";
    std::ifstream level_file(path.c_str());

    level_file >> m_x;
    level_file >> m_y;

    level_file >> m_w;
    level_file >> m_h;

    for (int i = 0; i < m_w*m_h; i++) {
        // TODO: create a texture
    }

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
}

void ZoneEditor::init(Engine* game)
{
    bool loading = get_yes_no(game, "load existing zone?");
    // TODO: init camera

    if (loading) {
        m_zone_num = atoi(get_str(game, "zone number to load").c_str());
        char zone_num_cstr[2];
        snprintf(zone_num_cstr, 2, "%d", m_zone_num);
        std::string zone_num_str(zone_num_cstr);
        std::string path = "resources/levels/"+zone_num_str+"/zone_info";
        std::ifstream zone_file(path.c_str());

        int num_levels;
        zone_file >> num_levels;
        zone_file >> r;
        zone_file >> g;
        zone_file >> b;

        for (int i = 0; i < num_levels; i++) {
            LevelThumbnail* new_thumbnail = new LevelThumbnail(m_zone_num, i);
        }
    } else {
        r = g = b = 255;
    }
}

void ZoneEditor::cleanup()
{

}

void ZoneEditor::update(Engine* game)
{
    SDL_RenderClear(game->rend);
    if (mousedown) {
        for (int i = 0; i < levels.size(); i++) {
            levels[i]->selected = (selected == i);
        }
        int mouse_x, mouse_y;
        levels[selected]->move(mouse_x - x_offset, mouse_y - y_offset);
    }
}

void ZoneEditor::handle_events(Engine* game)
{
    SDL_Event e;
    int scr_w = game->screen_width;
    int scr_h = game->screen_height;
    int true_x, true_y;
    SDL_Rect cam_rect = camera->get_rect();

    while (SDL_PollEvent(&e)) {
        switch (e.type)
        {
        case SDL_QUIT:
            game->quit();
            break;
        case SDL_MOUSEBUTTONDOWN:
            mousedown = true;
            // get mouse position
            true_x = (e.button.x * ((float) cam_rect.w / (float) scr_w) + cam_rect.x) / TILE_WIDTH;
            true_y = (e.button.y * ((float) cam_rect.h / (float) scr_h) + cam_rect.y) / TILE_WIDTH;
            for (int i = 0; i < levels.size(); i++) {
                SDL_Rect rect = levels[i]->get_rect();
                if (true_x > rect.x && true_x < rect.x + rect.w &&
                    true_y > rect.y && true_y < rect.y + rect.h) {
                    selected = i;
                    break;
                }
            }
            break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode)
            {
            case SDL_SCANCODE_BACKSPACE:
                // TODO: delete level
                break;
            case SDL_SCANCODE_S:
                // TODO: save zone
                break;
            case SDL_SCANCODE_N:
                // TODO: new level
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

    // vertical gridlines
    int x = (cam_rect.x / (5*TILE_WIDTH)) * (5*TILE_WIDTH);
    while (x < cam_rect.x + cam_rect.w) {
        if (x % 20 == 0) {
            // thick gridlines
            int y1 = 0;
            int y2 = game->screen_height;
            SDL_SetRenderDrawColor(game->rend, 150, 150, 150, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(game->rend, x, y1, x, y2);
        } else {
            // thin gridlines
            int y1 = 0;
            int y2 = game->screen_height;
            SDL_SetRenderDrawColor(game->rend, 230, 230, 230, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(game->rend, x, y1, x, y2);
        }
        x += 5*TILE_WIDTH;
    }
    // horizontal gridlines
    int y = (cam_rect.y / (5*TILE_WIDTH)) * (5*TILE_WIDTH);
    while (y < cam_rect.y + cam_rect.h) {
        if (y % 20 == 0) {
            // thick gridlines
            int x1 = 0;
            int x2 = game->screen_width;
            SDL_SetRenderDrawColor(game->rend, 150, 150, 150, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(game->rend, x1, y, x2, y);
        } else {
            // thin gridlines
            int x1 = 0;
            int x2 = game->screen_height;
            SDL_SetRenderDrawColor(game->rend, 230, 230, 230, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(game->rend, x1, y, x2, y);
        }
        y += 5*TILE_WIDTH;
    }

    for (int i = 0; i < levels.size(); i++) {
        levels[i]->draw(game->rend, cam_rect, game->screen_width, game->screen_height);
    }

    SDL_RenderPresent(game->rend);
}
