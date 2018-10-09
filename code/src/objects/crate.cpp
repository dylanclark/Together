
#include <objects.hpp>

/*************/
/*   CRATE   */
/*************/

static const float CRATE_ACC = 0.2;

Crate::Crate(int x, int y, int w, int h, bool color, SDL_Color palette)
{
    m_rect.x = x;
    true_x = m_rect.x;
    m_has_light = false;
    m_rect.y = y;
    m_rect.w = w*TILE_WIDTH;
    m_rect.h = h*TILE_WIDTH;
    m_color = color;
    m_type = OBJECT_CRATE;
    pushed = false;

    Texture active_tex = ResourceManager::get_texture("crate");
    Sprite active_sprite(active_tex, active_tex, TILE_WIDTH, TILE_WIDTH, &palette);
    FBO crate_fbo(w*TILE_WIDTH*8, h*TILE_WIDTH*2);
    crate_fbo.bind();

    // active (1 frame)
    SDL_Rect clip_rect, render_rect, border_rect;
    int scr_w = w*TILE_WIDTH*8;
    int scr_h = h*TILE_WIDTH*2;
    clip_rect.y = 0;
    clip_rect.w = clip_rect.h = TILE_WIDTH;
    render_rect.w = render_rect.h = TILE_WIDTH;
    int clip_base_x = (m_color == 0) ? 0 : TILE_WIDTH*3;
    // corners
    // top left
    clip_rect.x = clip_base_x;
    clip_rect.y = 0;
    render_rect.x = 0;
    render_rect.y = 0;
    active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
    // top right
    clip_rect.x = clip_base_x + 2*TILE_WIDTH;
    render_rect.x = (w-1)*TILE_WIDTH;
    active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
    // bottom left
    clip_rect.x = clip_base_x;
    clip_rect.y = 2*TILE_WIDTH;
    render_rect.x = 0;
    render_rect.y = (h-1)*TILE_WIDTH;
    active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
    // bottom right
    clip_rect.x = clip_base_x + 2*TILE_WIDTH;
    render_rect.x = (w-1)*TILE_WIDTH;
    active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
    // sides
    for (int j = 1; j < w - 1; j++) {
        // top
        clip_rect.x = clip_base_x + TILE_WIDTH;
        clip_rect.y = 0;
        render_rect.x = j*TILE_WIDTH;
        render_rect.y = 0;
        active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // bottom
        clip_rect.x = clip_base_x + TILE_WIDTH;
        clip_rect.y = 2*TILE_WIDTH;
        render_rect.x = j*TILE_WIDTH;
        render_rect.y = (h-1)*TILE_WIDTH;
        active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
    }
    for (int i = 1; i < h - 1; i++) {
        // left
        clip_rect.x = clip_base_x;
        clip_rect.y = TILE_WIDTH;
        render_rect.x = 0;
        render_rect.y = i*TILE_WIDTH;
        active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // right
        clip_rect.x = clip_base_x + 2*TILE_WIDTH;
        clip_rect.y = TILE_WIDTH;
        render_rect.x = (w-1)*TILE_WIDTH;
        render_rect.y = i*TILE_WIDTH;
        active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
    }
    // middle
    clip_rect.x = clip_base_x + TILE_WIDTH;
    clip_rect.y = TILE_WIDTH;
    for (int i = 1; i < w - 1; i++) {
        for (int j = 1; j < h - 1; j++) {
            render_rect.x = i*TILE_WIDTH;
            render_rect.y = j*TILE_WIDTH;
            active_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        }
    }

    Texture inactive_tex = ResourceManager::get_texture("shiftblock_active");
    Sprite inactive_sprite(inactive_tex, inactive_tex, TILE_WIDTH, TILE_WIDTH, &palette);

    // inactive (8 frames)
    int render_base_x;
    int render_base_y = h * TILE_WIDTH;
    int clip_base_y = (m_color == 0) ? 0 : TILE_WIDTH*3;
    for (int i = 0; i < 8; i++) {
        render_base_x = i*w*TILE_WIDTH;
        // top left
        clip_rect.x = i*TILE_WIDTH*3;
        clip_rect.y = clip_base_y;
        render_rect.x = render_base_x;
        render_rect.y = render_base_y;
        inactive_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // top right
        clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
        clip_rect.y = clip_base_y;
        render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
        render_rect.y = render_base_y;
        inactive_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // bottom left
        clip_rect.x = i*TILE_WIDTH*3;
        clip_rect.y = clip_base_y + 2*TILE_WIDTH;
        render_rect.x = render_base_x;
        render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
        inactive_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // bottom right
        clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
        clip_rect.y = clip_base_y + 2*TILE_WIDTH;
        render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
        render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
        inactive_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // sides
        clip_rect.x = i*TILE_WIDTH*3 + TILE_WIDTH;
        for (int j = 1; j < w - 1; j++) {
            render_rect.x = render_base_x + j*TILE_WIDTH;
            // top
            clip_rect.y = clip_base_y;
            render_rect.y = render_base_y;
            inactive_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
            // bottom
            clip_rect.y = clip_base_y + 2*TILE_WIDTH;
            render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
            inactive_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        }
        clip_rect.y = clip_base_y + TILE_WIDTH;
        for (int k = 1; k < h - 1; k++) {
            render_rect.y = render_base_y + k*TILE_WIDTH;
            // left
            clip_rect.x = i*TILE_WIDTH*3;
            render_rect.x = render_base_x;
            inactive_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
            // right
            clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
            render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
            inactive_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        }
        // middle
        clip_rect.x = clip_base_x + TILE_WIDTH;
        clip_rect.y = clip_base_y + TILE_WIDTH;
        for (int j = 1; j < w - 1; j++) {
            for (int k = 1; k < h - 1; k++) {
                render_rect.x = render_base_x + j*TILE_WIDTH;
                render_rect.y = render_base_y + k*TILE_WIDTH;
                inactive_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
            }
        }
    }
    crate_fbo.unbind();
    GLuint m_tex_id = crate_fbo.get_texture();
    Texture m_tex(m_tex_id, w*TILE_WIDTH*8, h*TILE_WIDTH*2);

    FBO normal_fbo(scr_w, scr_h);

    Texture active_normal = ResourceManager::get_texture("crate_normal");
    Sprite act_norm_sprite(active_normal, active_normal, TILE_WIDTH, TILE_WIDTH, &palette);
    normal_fbo.bind();

    // active (1 frame)
    clip_rect.y = 0;
    clip_base_x = (m_color == 0) ? 0 : TILE_WIDTH*3;
    // corners
    // top left
    clip_rect.x = clip_base_x;
    clip_rect.y = 0;
    render_rect.x = 0;
    render_rect.y = 0;
    act_norm_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
    // top right
    clip_rect.x = clip_base_x + 2*TILE_WIDTH;
    render_rect.x = (w-1)*TILE_WIDTH;
    act_norm_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
    // bottom left
    clip_rect.x = clip_base_x;
    clip_rect.y = 2*TILE_WIDTH;
    render_rect.x = 0;
    render_rect.y = (h-1)*TILE_WIDTH;
    act_norm_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
    // bottom right
    clip_rect.x = clip_base_x + 2*TILE_WIDTH;
    render_rect.x = (w-1)*TILE_WIDTH;
    act_norm_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
    // sides
    for (int j = 1; j < w - 1; j++) {
        // top
        clip_rect.x = clip_base_x + TILE_WIDTH;
        clip_rect.y = 0;
        render_rect.x = j*TILE_WIDTH;
        render_rect.y = 0;
        act_norm_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // bottom
        clip_rect.x = clip_base_x + TILE_WIDTH;
        clip_rect.y = 2*TILE_WIDTH;
        render_rect.x = j*TILE_WIDTH;
        render_rect.y = (h-1)*TILE_WIDTH;
        act_norm_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
    }
    for (int i = 1; i < h - 1; i++) {
        // left
        clip_rect.x = clip_base_x;
        clip_rect.y = TILE_WIDTH;
        render_rect.x = 0;
        render_rect.y = i*TILE_WIDTH;
        act_norm_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        // right
        clip_rect.x = clip_base_x + 2*TILE_WIDTH;
        clip_rect.y = TILE_WIDTH;
        render_rect.x = (w-1)*TILE_WIDTH;
        render_rect.y = i*TILE_WIDTH;
        act_norm_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
    }
    // middle
    clip_rect.x = clip_base_x + TILE_WIDTH;
    clip_rect.y = TILE_WIDTH;
    for (int i = 1; i < w - 1; i++) {
        for (int j = 1; j < h - 1; j++) {
            render_rect.x = i*TILE_WIDTH;
            render_rect.y = j*TILE_WIDTH;
            act_norm_sprite.render(render_rect.x, render_rect.y, scr_w, scr_h, &clip_rect);
        }
    }

    Texture inactive_normal = ResourceManager::get_texture("normal");
    Sprite inact_norm_sprite(inactive_normal, inactive_normal, TILE_WIDTH, TILE_WIDTH, &palette);

    clip_rect.x = clip_rect.y = 0;
    for (int i = 0; i < w*8; i++) {
        for (int j = 0; j < h; j++) {
            inact_norm_sprite.render(i*TILE_WIDTH, (h+j)*TILE_WIDTH, scr_w, scr_h, &clip_rect);
        }
    }

    normal_fbo.unbind();
    GLuint norm_tex_id = normal_fbo.get_texture();
    Texture norm_tex(norm_tex_id, w*TILE_WIDTH*8, h*TILE_WIDTH*2);

    m_sprite = Sprite(m_tex, norm_tex, w*TILE_WIDTH, h*TILE_WIDTH, &palette);
}

void Crate::render_bg(Camera* cam, std::vector<Light> lights, bool active_color)
{
    int animation_length;
    double animation_speed;
    if (m_color == active_color) {
        animation_length = 1;
        animation_speed = 100;
    } else {
        animation_length = 8;
        animation_speed = 70.0;
    }
    int frame = ((int) ((float) SDL_GetTicks() / animation_speed)) % animation_length;
    int clip_y = (m_color != active_color);
    SDL_Rect clip_rect = {frame * m_rect.w, clip_y * m_rect.h, m_rect.w, m_rect.h};
    m_sprite.render(m_rect.x, m_rect.y, &clip_rect, cam, lights);
}

void Crate::update_x(SDL_Rect black_player, SDL_Rect white_player)
{
    // snap to grid if not being pushed
    if (!pushed) {
        // find closest tile spot
        int nearest_x = round(true_x / (float) TILE_WIDTH) * TILE_WIDTH;
        true_x += ((float) nearest_x - true_x) * CRATE_ACC;
    }
    m_rect.x = round(true_x);
    pushed = false;
}

std::vector<InvisibleWall> Crate::get_walls(std::vector<Tile> &tiles)
{
    std::vector<InvisibleWall> result;
    int platform_w = TILE_WIDTH;
    int platform_h = TILE_WIDTH;
    InvisibleWall left_wall(m_rect.x - platform_w, m_rect.y - platform_h, platform_w, m_rect.h + 2*platform_h, INVISIBLE_WALL_LEFT);
    InvisibleWall right_wall(m_rect.x + m_rect.w, m_rect.y - platform_h, platform_w, m_rect.h + 2*platform_h, INVISIBLE_WALL_RIGHT);
    InvisibleWall top_wall(m_rect.x - platform_w, m_rect.y - platform_h, m_rect.w + 2*platform_w, platform_h, INVISIBLE_WALL_TOP);
    InvisibleWall bottom_wall(m_rect.x - platform_w, m_rect.y + m_rect.h, m_rect.w + 2*platform_w, platform_h, INVISIBLE_WALL_BOTTOM);

    bool left_flag, right_flag, top_flag, bottom_flag;
    left_flag = right_flag = top_flag = bottom_flag = true;

    for (int i = 0; i < tiles.size(); i++) {
        if (m_color == 0 && tiles[i].get_type() != TILE_BLACK_SOLID) {
            continue;
        }
        if (m_color == 1 && tiles[i].get_type() != TILE_WHITE_SOLID) {
            continue;
        }
        SDL_Rect left_rect = left_wall.get_rect();
        left_rect.y += platform_h;
        left_rect.h -= platform_h*2;
        if (check_full_overlap(left_rect, tiles[i].get_rect())) {
            left_flag = false;
        }
        SDL_Rect right_rect = right_wall.get_rect();
        right_rect.y += platform_h;
        right_rect.h -= platform_h*2;
        if (check_full_overlap(right_rect, tiles[i].get_rect())) {
            right_flag = false;
        }
        SDL_Rect top_rect = top_wall.get_rect();
        top_rect.x += platform_w;
        top_rect.w -= platform_w*2;
        if (check_full_overlap(top_rect, tiles[i].get_rect())) {
            top_flag = false;
        }
        SDL_Rect bottom_rect = bottom_wall.get_rect();
        bottom_rect.x += platform_w;
        bottom_rect.w -= platform_w*2;
        if (check_full_overlap(bottom_rect, tiles[i].get_rect())) {
            bottom_flag = false;
        }
    }

    if (left_flag) {
        result.push_back(left_wall);
    }
    if (right_flag) {
        result.push_back(right_wall);
    }
    if (top_flag) {
        result.push_back(top_wall);
    }
    if (bottom_flag) {
        result.push_back(bottom_wall);
    }
    return result;
}

void Crate::push(SDL_Rect player_pushing, SDL_Rect other_player)
{
    Vector repos;
    if (check_collision(m_rect, other_player, &repos)) {
        return;
    }
    if (check_collision(m_rect, player_pushing, &repos)) {
        pushed = true;
        m_rect.x += repos.x / 2;
        true_x = m_rect.x;
    }
}


