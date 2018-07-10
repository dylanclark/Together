#include <editor.hpp>
#include <cstring>

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

    level_file >> m_width;
    level_file >> m_height;

    for (int i = 0; i < m_width*m_height; i++) {
        // TODO: create a texture
    }

    level_file.close();
}

void LevelThumbnail::draw(SDL_Renderer* rend, SDL_Rect cam_rect, int scr_w, int scr_h)
{
    int x = (m_x - cam_rect.x) / ((float) cam_rect.w / (float) scr_w);
    int y = (m_y - cam_rect.y) / ((float) cam_rect.h / (float) scr_h);
    int w = m_width*TILE_WIDTH * ((float) scr_w / (float) cam_rect.w);
    int h = m_height*TILE_WIDTH * ((float) scr_h / (float) cam_rect.h);
    SDL_Rect render_rect = {x,y,w,h};
    SDL_RenderCopy(rend, m_tex, NULL, &render_rect);
}

void ZoneEditor::init(Engine* game)
{
    bool loading = get_yes_no(game, "load existing zone?");

    // TODO: if there's a folder with our zone_num, we're loading. otherwise, creating new

    if (loading) {
        int num_levels = 0;
        // TODO: count how many level files are in our zone folder
        for (int i = 0; i < num_levels; i++) {
            LevelThumbnail* new_thumbnail = new LevelThumbnail(m_zone_num, i);
            // TODO: read level file for tiles. create level texture
        }
    }
}

void ZoneEditor::cleanup()
{

}

void ZoneEditor::update(Engine* game)
{
    SDL_RenderClear(game->rend);
}

void ZoneEditor::handle_events(Engine* game)
{
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        switch (e.type)
        {
        case SDL_QUIT:
            game->quit();
            break;
        default:
            break;
        }
        camera->handle_event(e);
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
