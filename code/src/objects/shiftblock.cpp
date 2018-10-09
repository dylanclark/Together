
#include <objects.hpp>

/******************/
/*   SHIFTBLOCK   */
/******************/

ShiftBlock::ShiftBlock(int x, int y, int w, int h, SDL_Color palette)
{
    m_rect.x = x;
    m_has_light = false;
    m_rect.y = y;
    m_rect.w = w*TILE_WIDTH;
    m_rect.h = h*TILE_WIDTH;
    m_status = SHIFTBLOCK_IDLE;
    m_type = OBJECT_SHIFTBLOCK;

    int scr_w = w*TILE_WIDTH*8;
    int scr_h = h*TILE_WIDTH*4;
    Texture idle_tex = ResourceManager::get_texture("shiftblock_idle");
    Sprite idle_sprite(idle_tex, idle_tex, TILE_WIDTH, TILE_WIDTH, &palette);
    Texture active_tex = ResourceManager::get_texture("shiftblock_active");
    Sprite active_sprite(active_tex, active_tex, TILE_WIDTH, TILE_WIDTH, &palette);

    FBO m_fbo(scr_w, scr_h);
    m_fbo.bind();

    // idle black
    SDL_Rect clip_rect, render_rect, border_rect;
    clip_rect.y = 0;
    clip_rect.w = clip_rect.h = TILE_WIDTH;
    render_rect.w = render_rect.h = TILE_WIDTH;
    for (int i = 0; i < 8; i++) {
        clip_rect.x = i*TILE_WIDTH;
        for (int j = 0; j < w; j++) {
            for (int k = 0; k < h; k++) {
                render_rect.x = i*(w*TILE_WIDTH) + j*TILE_WIDTH;
                render_rect.y = k*TILE_WIDTH;
                idle_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
            }
        }
        // TODO
        /*
        // borders
        SDL_SetRenderDrawColor(game->rend, 0, 0, 0, SDL_ALPHA_OPAQUE);
        // top
        border_rect.w = w*TILE_WIDTH;
        border_rect.h = 1;
        border_rect.x = i*w*TILE_WIDTH;
        border_rect.y = 0;
        SDL_RenderFillRect(game->rend, &border_rect);
        // bottom
        border_rect.y = h*TILE_WIDTH-1;
        SDL_RenderFillRect(game->rend, &border_rect);
        // left
        border_rect.w = 1;
        border_rect.h = h*TILE_WIDTH;
        border_rect.x = i*w*TILE_WIDTH;
        border_rect.y = 0;
        SDL_RenderFillRect(game->rend, &border_rect);
        // right
        border_rect.x = i*w*TILE_WIDTH + w*TILE_WIDTH - 1;
        SDL_RenderFillRect(game->rend, &border_rect);
        */
    }

    // idle white
    for (int i = 0; i < 8; i++) {
        clip_rect.x = i*TILE_WIDTH;
        for (int j = 0; j < w; j++) {
            for (int k = 0; k < h; k++) {
                render_rect.x = i*(w*TILE_WIDTH) + j*TILE_WIDTH;
                render_rect.y = h*TILE_WIDTH + k*TILE_WIDTH;
                idle_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
            }
        }
        // TODO
        /*
        // borders
        SDL_SetRenderDrawColor(game->rend, palette.r, palette.g, palette.b, SDL_ALPHA_OPAQUE);
        // top
        border_rect.w = w*TILE_WIDTH;
        border_rect.h = 1;
        border_rect.x = i*w*TILE_WIDTH;
        border_rect.y = h*TILE_WIDTH;
        // SDL_RenderFillRect(game->rend, &border_rect);
        // bottom
        border_rect.y = h*TILE_WIDTH + h*TILE_WIDTH-1;
        // SDL_RenderFillRect(game->rend, &border_rect);
        // left
        border_rect.w = 1;
        border_rect.h = h*TILE_WIDTH;
        border_rect.x = i*w*TILE_WIDTH;
        border_rect.y = h*TILE_WIDTH;
        // SDL_RenderFillRect(game->rend, &border_rect);
        // right
        border_rect.x = i*w*TILE_WIDTH + w*TILE_WIDTH - 1;
        // SDL_RenderFillRect(game->rend, &border_rect);
        */
    }

    // triggered black
    int render_base_x;
    int render_base_y = h * TILE_WIDTH * 2;
    for (int i = 0; i < 8; i++) {
        render_base_x = i*w*TILE_WIDTH;
        // top left
        clip_rect.x = i*TILE_WIDTH*3;
        clip_rect.y = 0;
        render_rect.x = render_base_x;
        render_rect.y = render_base_y;
        active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // top right
        clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
        clip_rect.y = 0;
        render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
        render_rect.y = render_base_y;
        active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // bottom left
        clip_rect.x = i*TILE_WIDTH*3;
        clip_rect.y = 2*TILE_WIDTH;
        render_rect.x = render_base_x;
        render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
        active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // bottom right
        clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
        clip_rect.y = 2*TILE_WIDTH;
        render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
        render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
        active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // sides
        clip_rect.x = i*TILE_WIDTH*3 + TILE_WIDTH;
        for (int j = 1; j < w - 1; j++) {
            render_rect.x = render_base_x + j*TILE_WIDTH;
            // top
            clip_rect.y = 0;
            render_rect.y = render_base_y;
            active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
            // bottom
            clip_rect.y = 2*TILE_WIDTH;
            render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
            active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        }
        clip_rect.y = TILE_WIDTH;
        for (int k = 1; k < h - 1; k++) {
            render_rect.y = render_base_y + TILE_WIDTH*k;
            // left
            clip_rect.x = i*TILE_WIDTH*3;
            render_rect.x = render_base_x;
            active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
            // right
            clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
            render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
            active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        }
        // middle
        clip_rect.x = TILE_WIDTH;
        clip_rect.y = TILE_WIDTH;
        for (int j = 1; j < w - 1; j++) {
            for (int k = 1; k < h - 1; k++) {
                render_rect.x = render_base_x + j*TILE_WIDTH;
                render_rect.y = render_base_y + k*TILE_WIDTH;
                active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
            }
        }
    }

    // triggered white
    render_base_y = h * TILE_WIDTH * 3;
    int clip_base_y = TILE_WIDTH*3;
    for (int i = 0; i < 8; i++) {
        render_base_x = i*w*TILE_WIDTH;
        // top left
        clip_rect.x = i*TILE_WIDTH*3;
        clip_rect.y = clip_base_y;
        render_rect.x = render_base_x;
        render_rect.y = render_base_y;
        active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // top right
        clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
        clip_rect.y = clip_base_y;
        render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
        render_rect.y = render_base_y;
        active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // bottom left
        clip_rect.x = i*TILE_WIDTH*3;
        clip_rect.y = clip_base_y + 2*TILE_WIDTH;
        render_rect.x = render_base_x;
        render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
        active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // bottom right
        clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
        clip_rect.y = clip_base_y + 2*TILE_WIDTH;
        render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
        render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
        active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // sides
        clip_rect.x = i*TILE_WIDTH*3 + TILE_WIDTH;
        for (int j = 1; j < w - 1; j++) {
            render_rect.x = render_base_x + j*TILE_WIDTH;
            // top
            clip_rect.y = clip_base_y;
            render_rect.y = render_base_y;
            active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
            // bottom
            clip_rect.y = clip_base_y + 2*TILE_WIDTH;
            render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
            active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        }
        clip_rect.y = clip_base_y + TILE_WIDTH;
        for (int k = 1; k < h - 1; k++) {
            render_rect.y = render_base_y + k*TILE_WIDTH;
            // left
            clip_rect.x = i*TILE_WIDTH*3;
            render_rect.x = render_base_x;
            active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
            // right
            clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
            render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
            active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        }
        // middle
        clip_rect.x = TILE_WIDTH;
        clip_rect.y = TILE_WIDTH*4;
        for (int j = 1; j < w - 1; j++) {
            for (int k = 1; k < h - 1; k++) {
                render_rect.x = render_base_x + j*TILE_WIDTH;
                render_rect.y = render_base_y + k*TILE_WIDTH;
                active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
            }
        }
    }
    m_fbo.unbind();
    GLuint m_tex_id = m_fbo.get_texture();
    Texture m_tex(m_tex_id, w*TILE_WIDTH*8, h*TILE_WIDTH*4);

    m_sprite = Sprite(m_tex, m_tex, w*TILE_WIDTH, h*TILE_WIDTH, &palette);
}

void ShiftBlock::render_bg(Camera* cam, std::vector<Light> lights, bool active_color)
{
    int animation_length = 8;
    double animation_speed = 70.0;
    int frame = ((int) ((float) SDL_GetTicks() / animation_speed)) % animation_length;
    int clip_y = m_status + (m_status != SHIFTBLOCK_IDLE || active_color == 1);
    SDL_Rect clip_rect = {frame * m_rect.w, clip_y * m_rect.h, m_rect.w, m_rect.h};
    m_sprite.render(m_rect.x, m_rect.y, &clip_rect, cam, lights);
}

void ShiftBlock::render_fg(Camera* cam, std::vector<Light> lights, bool active_color)
{
}

void ShiftBlock::update_x(SDL_Rect black_player, SDL_Rect white_player)
{
    if (!check_touching_vert(black_player, m_rect) && !check_touching_vert(white_player, m_rect)) {
        unshift();
    }
}


