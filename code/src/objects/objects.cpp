
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <levels.hpp>
#include <objects.hpp>
#include <utils.hpp>

/**************/
/*   SPRING   */
/**************/

Spring::Spring(int x, int y, bool color, float y_vel, SDL_Color palette)
{
    m_color = color;
    m_yvel = y_vel;
    if (m_color == 0) {
        m_tex.load_object(16, 16, "spring-sheet-black.png", &palette);
    } else {
        m_tex.load_object(16, 16, "spring-sheet-white.png", &palette);
    }
    m_status = SPRING_IDLE;

    m_type = OBJECT_SPRING;
    m_rect.w = 8;
    m_rect.h = 4;
    m_rect.x = x + (m_tex.get_width()-m_rect.w)/2;
    m_rect.y = y + (m_color == 0)*(m_tex.get_height()-m_rect.h);
}

void Spring::render_bg(Camera* cam, bool active_color)
{
    int frame;
    int animation_length = 12;
    double animation_speed = 60;
    if (m_status == SPRING_IDLE || m_status == SPRING_INACTIVE) {
        frame = 0;
    } else {
        frame = ((int) (((float) SDL_GetTicks() - (float) spring_start)
                / animation_speed));
        if (frame >= animation_length) {
            m_status = SPRING_IDLE;
            frame = 0;
        }
    }
    int tex_w = m_tex.get_width();
    int tex_h = m_tex.get_height();
    int render_x = m_rect.x - (tex_w-m_rect.w)/2;
    int render_y = m_rect.y - (m_color == 0)*(m_tex.get_height()-m_rect.h);
    SDL_Rect frame_clip = {frame*tex_w, m_status*tex_h, tex_w, tex_h};
    m_tex.render(render_x, render_y, &frame_clip, cam);
}

void Spring::spring()
{
    spring_start = SDL_GetTicks();
    m_status = SPRING_SPRUNG;
}

/***********************/
/*   MOVING PLATFORM   */
/***********************/
InvisibleWall::InvisibleWall(int x, int y, int w, int h, InvisibleWallSide side)
{
    m_rect.x = x;
    m_rect.y = y;
    m_rect.w = w;
    m_rect.h = h;
    m_side = side;
}

MovingPlatform::MovingPlatform(int x1, int y1, int x2, int y2, int w, int h, bool color, bool automatic, int move_length, int pause_length, SDL_Color palette)
{
    m_type = OBJECT_MOVING_PLATFORM;
    m_status = PLATFORM_PAUSE_A;
    m_move_length = move_length;
    m_pause_length = pause_length;
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
    m_auto = automatic;
    m_timestep = 0;
    m_color = color;

    m_rect.x = x1;
    m_rect.y = y1;
    m_rect.w = w*TILE_WIDTH;
    m_rect.h = h*TILE_WIDTH;

    // m_tex.create_square(color, w, h, &palette);
}

void MovingPlatform::update_x(SDL_Rect black_player, SDL_Rect white_player)
{
    SDL_Rect player = (m_color == 0) ? white_player : black_player;
    m_timestep++;
    if (m_auto && (m_status == PLATFORM_PAUSE_A || m_status == PLATFORM_PAUSE_B) && (m_timestep >= m_pause_length)) {
        if (check_in_platform(player, m_rect) && !check_full_overlap(m_rect, player)) {
            m_timestep--;
        } else {
            m_status = (PlatformStatus) (((int) m_status + 1) % 4);
            m_timestep = 0;
        }
    }
    if ((m_status == PLATFORM_MOVETO_A || m_status == PLATFORM_MOVETO_B) && (m_timestep >= m_move_length)) {
        if (check_in_platform(player, m_rect) && !check_full_overlap(m_rect, player)) {
            m_timestep--;
        } else {
            m_status = (PlatformStatus) (((int) m_status + 1) % 4);
            m_timestep = 0;
        }
    }
    if (m_status == PLATFORM_PAUSE_A) {
        m_rect.x = m_x1;
    } else if (m_status == PLATFORM_PAUSE_B) {
        m_rect.x = m_x2;
    } else {
        float t = (float) m_timestep / (float) m_move_length;
        if (m_status == PLATFORM_MOVETO_A) {
            t = 1 - t;
        }
        float s = 1 - t;
        m_rect.x = round(pow(s, 2.0) * (1.0 + 2.0*t) * (float) m_x1 + pow(t, 2.0) * (1.0 + 2.0*s) * (float) m_x2);
    }
}

void MovingPlatform::update_y()
{
    if (m_status == PLATFORM_PAUSE_A) {
        m_rect.y = m_y1;
    } else if (m_status == PLATFORM_PAUSE_B) {
        m_rect.y = m_y2;
    } else {
        float t = (float) m_timestep / (float) m_move_length;
        if (m_status == PLATFORM_MOVETO_A) {
            t = 1 - t;
        }
        float s = 1 - t;
        m_rect.y = round(pow(s, 2.0) * (1.0 + 2.0*t) * (float) m_y1 + pow(t, 2.0) * (1.0 + 2.0*s) * (float) m_y2);
    }
}

void MovingPlatform::render_bg(Camera* cam, bool active_color)
{
    m_tex.render(m_rect.x, m_rect.y, NULL, cam);
}

void MovingPlatform::trigger()
{
    m_status = PLATFORM_MOVETO_B;
}

void MovingPlatform::untrigger()
{
    m_status = PLATFORM_MOVETO_A;
}

std::vector<InvisibleWall> MovingPlatform::get_walls(std::vector<Tile> &tiles)
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

/******************/
/*   SHIFTBLOCK   */
/******************/

ShiftBlock::ShiftBlock(int x, int y, int w, int h, SDL_Color palette)
{
    m_rect.x = x;
    m_rect.y = y;
    m_rect.w = w*TILE_WIDTH;
    m_rect.h = h*TILE_WIDTH;
    m_status = SHIFTBLOCK_IDLE;
    m_type = OBJECT_SHIFTBLOCK;

    Uint32 format = SDL_GetWindowPixelFormat(game->window);
    SDL_Texture* tex = SDL_CreateTexture(game->rend, format, SDL_TEXTUREACCESS_TARGET, w*TILE_WIDTH*8, h*TILE_WIDTH*4);
    SDL_SetRenderTarget(game->rend, tex);

    SDL_Surface* surface = IMG_Load("resources/textures/shiftblock-idle.png");
    SDL_Texture* idle_tex = SDL_CreateTextureFromSurface(game->rend, surface);
    SDL_SetTextureColorMod(idle_tex, palette.r, palette.g, palette.b);

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
                SDL_RenderCopy(game->rend, idle_tex, &clip_rect, &render_rect);
            }
        }
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
    }

    // idle white
    for (int i = 0; i < 8; i++) {
        clip_rect.x = i*TILE_WIDTH;
        for (int j = 0; j < w; j++) {
            for (int k = 0; k < h; k++) {
                render_rect.x = i*(w*TILE_WIDTH) + j*TILE_WIDTH;
                render_rect.y = h*TILE_WIDTH + k*TILE_WIDTH;
                SDL_RenderCopy(game->rend, idle_tex, &clip_rect, &render_rect);
            }
        }
        // borders
        SDL_SetRenderDrawColor(game->rend, palette.r, palette.g, palette.b, SDL_ALPHA_OPAQUE);
        // top
        border_rect.w = w*TILE_WIDTH;
        border_rect.h = 1;
        border_rect.x = i*w*TILE_WIDTH;
        border_rect.y = h*TILE_WIDTH;
        SDL_RenderFillRect(game->rend, &border_rect);
        // bottom
        border_rect.y = h*TILE_WIDTH + h*TILE_WIDTH-1;
        SDL_RenderFillRect(game->rend, &border_rect);
        // left
        border_rect.w = 1;
        border_rect.h = h*TILE_WIDTH;
        border_rect.x = i*w*TILE_WIDTH;
        border_rect.y = h*TILE_WIDTH;
        SDL_RenderFillRect(game->rend, &border_rect);
        // right
        border_rect.x = i*w*TILE_WIDTH + w*TILE_WIDTH - 1;
        SDL_RenderFillRect(game->rend, &border_rect);
    }

    SDL_Surface* active_surface = IMG_Load("resources/textures/shiftblock-active.png");
    SDL_Texture* active_tex = SDL_CreateTextureFromSurface(game->rend, active_surface);
    SDL_SetTextureColorMod(active_tex, palette.r, palette.g, palette.b);

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
        SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        // top right
        clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
        clip_rect.y = 0;
        render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
        render_rect.y = render_base_y;
        SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        // bottom left
        clip_rect.x = i*TILE_WIDTH*3;
        clip_rect.y = 2*TILE_WIDTH;
        render_rect.x = render_base_x;
        render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
        SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        // bottom right
        clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
        clip_rect.y = 2*TILE_WIDTH;
        render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
        render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
        SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        // sides
        clip_rect.x = i*TILE_WIDTH*3 + TILE_WIDTH;
        for (int j = 1; j < w - 1; j++) {
            render_rect.x = render_base_x + j*TILE_WIDTH;
            // top
            clip_rect.y = 0;
            render_rect.y = render_base_y;
            SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
            // bottom
            clip_rect.y = 2*TILE_WIDTH;
            render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
            SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        }
        clip_rect.y = TILE_WIDTH;
        for (int k = 1; k < h - 1; k++) {
            render_rect.y = render_base_y + TILE_WIDTH*k;
            // left
            clip_rect.x = i*TILE_WIDTH*3;
            render_rect.x = render_base_x;
            SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
            // right
            clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
            render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
            SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        }
        // middle
        clip_rect.x = TILE_WIDTH;
        clip_rect.y = TILE_WIDTH;
        for (int j = 1; j < w - 1; j++) {
            for (int k = 1; k < h - 1; k++) {
                render_rect.x = render_base_x + j*TILE_WIDTH;
                render_rect.y = render_base_y + k*TILE_WIDTH;
                SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
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
        SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        // top right
        clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
        clip_rect.y = clip_base_y;
        render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
        render_rect.y = render_base_y;
        SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        // bottom left
        clip_rect.x = i*TILE_WIDTH*3;
        clip_rect.y = clip_base_y + 2*TILE_WIDTH;
        render_rect.x = render_base_x;
        render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
        SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        // bottom right
        clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
        clip_rect.y = clip_base_y + 2*TILE_WIDTH;
        render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
        render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
        SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        // sides
        clip_rect.x = i*TILE_WIDTH*3 + TILE_WIDTH;
        for (int j = 1; j < w - 1; j++) {
            render_rect.x = render_base_x + j*TILE_WIDTH;
            // top
            clip_rect.y = clip_base_y;
            render_rect.y = render_base_y;
            SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
            // bottom
            clip_rect.y = clip_base_y + 2*TILE_WIDTH;
            render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
            SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        }
        clip_rect.y = clip_base_y + TILE_WIDTH;
        for (int k = 1; k < h - 1; k++) {
            render_rect.y = render_base_y + k*TILE_WIDTH;
            // left
            clip_rect.x = i*TILE_WIDTH*3;
            render_rect.x = render_base_x;
            SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
            // right
            clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
            render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
            SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        }
        // middle
        clip_rect.x = TILE_WIDTH;
        clip_rect.y = TILE_WIDTH*4;
        for (int j = 1; j < w - 1; j++) {
            for (int k = 1; k < h - 1; k++) {
                render_rect.x = render_base_x + j*TILE_WIDTH;
                render_rect.y = render_base_y + k*TILE_WIDTH;
                SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
            }
        }
    }
    SDL_SetRenderTarget(game->rend, NULL);

    // m_tex.create_texture(tex, w, h);
}

void ShiftBlock::render_bg(Camera* cam, bool active_color)
{
    int animation_length = 8;
    double animation_speed = 70.0;
    int frame = ((int) ((float) SDL_GetTicks() / animation_speed)) % animation_length;
    int clip_y = m_status + (m_status != SHIFTBLOCK_IDLE || active_color == 1);
    SDL_Rect clip_rect = {frame * m_rect.w, clip_y * m_rect.h, m_rect.w, m_rect.h};
    m_tex.render(m_rect.x, m_rect.y, &clip_rect, cam);
}

void ShiftBlock::render_fg(Camera* cam, bool active_color)
{
}

void ShiftBlock::update_x(SDL_Rect black_player, SDL_Rect white_player)
{
    if (!check_touching_vert(black_player, m_rect) && !check_touching_vert(white_player, m_rect)) {
        unshift();
    }
}

/*************/
/*   CRATE   */
/*************/
Crate::Crate(int x, int y, int w, int h, bool color, SDL_Color palette)
{
    m_rect.x = x;
    m_rect.y = y;
    m_rect.w = w*TILE_WIDTH;
    m_rect.h = h*TILE_WIDTH;
    m_color = color;
    m_type = OBJECT_CRATE;
    pushed = false;

    // create texture
    Uint32 format = SDL_GetWindowPixelFormat(game->window);
    SDL_Texture* tex = SDL_CreateTexture(game->rend, format, SDL_TEXTUREACCESS_TARGET, w*TILE_WIDTH*8, h*TILE_WIDTH*2);
    SDL_SetRenderTarget(game->rend, tex);

    SDL_Surface* active_surface = IMG_Load("resources/textures/crate.png");
    SDL_Texture* active_tex = SDL_CreateTextureFromSurface(game->rend, active_surface);
    SDL_FreeSurface(active_surface);
    SDL_SetTextureColorMod(active_tex, palette.r, palette.g, palette.b);

    // active (1 frame)
    SDL_Rect clip_rect, render_rect, border_rect;
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
    SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
    // top right
    clip_rect.x = clip_base_x + 2*TILE_WIDTH;
    render_rect.x = (w-1)*TILE_WIDTH;
    SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
    // bottom left
    clip_rect.x = clip_base_x;
    clip_rect.y = 2*TILE_WIDTH;
    render_rect.x = 0;
    render_rect.y = (h-1)*TILE_WIDTH;
    SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
    // bottom right
    clip_rect.x = clip_base_x + 2*TILE_WIDTH;
    render_rect.x = (w-1)*TILE_WIDTH;
    SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
    // sides
    for (int j = 1; j < w - 1; j++) {
        // top
        clip_rect.x = clip_base_x + TILE_WIDTH;
        clip_rect.y = 0;
        render_rect.x = j*TILE_WIDTH;
        render_rect.y = 0;
        SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        // bottom
        clip_rect.x = clip_base_x + TILE_WIDTH;
        clip_rect.y = 2*TILE_WIDTH;
        render_rect.x = j*TILE_WIDTH;
        render_rect.y = (h-1)*TILE_WIDTH;
        SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
    }
    for (int i = 1; i < h - 1; i++) {
        // left
        clip_rect.x = clip_base_x;
        clip_rect.y = TILE_WIDTH;
        render_rect.x = 0;
        render_rect.y = i*TILE_WIDTH;
        SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        // right
        clip_rect.x = clip_base_x + 2*TILE_WIDTH;
        clip_rect.y = TILE_WIDTH;
        render_rect.x = (w-1)*TILE_WIDTH;
        render_rect.y = i*TILE_WIDTH;
        SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
    }
    // middle
    clip_rect.x = clip_base_x + TILE_WIDTH;
    clip_rect.y = TILE_WIDTH;
    for (int i = 1; i < w - 1; i++) {
        for (int j = 1; j < h - 1; j++) {
            render_rect.x = i*TILE_WIDTH;
            render_rect.y = j*TILE_WIDTH;
            SDL_RenderCopy(game->rend, active_tex, &clip_rect, &render_rect);
        }
    }

    SDL_Surface* inactive_surface = IMG_Load("resources/textures/shiftblock-active.png");
    SDL_Texture* inactive_tex = SDL_CreateTextureFromSurface(game->rend, inactive_surface);
    SDL_SetTextureColorMod(inactive_tex, palette.r, palette.g, palette.b);

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
        SDL_RenderCopy(game->rend, inactive_tex, &clip_rect, &render_rect);
        // top right
        clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
        clip_rect.y = clip_base_y;
        render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
        render_rect.y = render_base_y;
        SDL_RenderCopy(game->rend, inactive_tex, &clip_rect, &render_rect);
        // bottom left
        clip_rect.x = i*TILE_WIDTH*3;
        clip_rect.y = clip_base_y + 2*TILE_WIDTH;
        render_rect.x = render_base_x;
        render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
        SDL_RenderCopy(game->rend, inactive_tex, &clip_rect, &render_rect);
        // bottom right
        clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
        clip_rect.y = clip_base_y + 2*TILE_WIDTH;
        render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
        render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
        SDL_RenderCopy(game->rend, inactive_tex, &clip_rect, &render_rect);
        // sides
        clip_rect.x = i*TILE_WIDTH*3 + TILE_WIDTH;
        for (int j = 1; j < w - 1; j++) {
            render_rect.x = render_base_x + j*TILE_WIDTH;
            // top
            clip_rect.y = clip_base_y;
            render_rect.y = render_base_y;
            SDL_RenderCopy(game->rend, inactive_tex, &clip_rect, &render_rect);
            // bottom
            clip_rect.y = clip_base_y + 2*TILE_WIDTH;
            render_rect.y = render_base_y + (h-1)*TILE_WIDTH;
            SDL_RenderCopy(game->rend, inactive_tex, &clip_rect, &render_rect);
        }
        clip_rect.y = clip_base_y + TILE_WIDTH;
        for (int k = 1; k < h - 1; k++) {
            render_rect.y = render_base_y + k*TILE_WIDTH;
            // left
            clip_rect.x = i*TILE_WIDTH*3;
            render_rect.x = render_base_x;
            SDL_RenderCopy(game->rend, inactive_tex, &clip_rect, &render_rect);
            // right
            clip_rect.x = i*TILE_WIDTH*3 + 2*TILE_WIDTH;
            render_rect.x = render_base_x + (w-1)*TILE_WIDTH;
            SDL_RenderCopy(game->rend, inactive_tex, &clip_rect, &render_rect);
        }
        // middle
        clip_rect.x = clip_base_x + TILE_WIDTH;
        clip_rect.y = clip_base_y + TILE_WIDTH;
        for (int j = 1; j < w - 1; j++) {
            for (int k = 1; k < h - 1; k++) {
                render_rect.x = render_base_x + j*TILE_WIDTH;
                render_rect.y = render_base_y + k*TILE_WIDTH;
                SDL_RenderCopy(game->rend, inactive_tex, &clip_rect, &render_rect);
            }
        }
    }
    SDL_SetRenderTarget(game->rend, NULL);
    // m_tex.create_texture(tex, w, h);
}

void Crate::render_bg(Camera* cam, bool active_color)
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
    m_tex.render(m_rect.x, m_rect.y, &clip_rect, cam);
}

void Crate::update_x(SDL_Rect black_player, SDL_Rect white_player)
{
    // TODO snap to grid if not being pushed
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
    }
}

/********************/
/*   CROSS SPRING   */
/********************/

XSpring::XSpring(int x, int y, SDL_Color palette)
{
    m_tex.load_object(16, 32, "cross-spring.png", &palette);
    m_rect.w = 12;
    m_rect.h = 10;
    m_rect.x = x + (m_tex.get_width() - m_rect.w) / 2;
    m_rect.y = y + (m_tex.get_height() - m_rect.h) / 2;
    m_type = OBJECT_XSPRING;
    m_status = XSPRING_IDLE;
    animation_start = SDL_GetTicks();
    dot_on = NULL;
}

void XSpring::render_bg(Camera* cam, bool active_color)
{
    int animation_length;
    int animation_speed;
    switch (m_status)
    {
    case XSPRING_IDLE:
    case XSPRING_WHITE_ON:
    case XSPRING_BLACK_ON:
        animation_length = 1;
        animation_speed = 30;
        break;
    case XSPRING_BLACK_LAND:
    case XSPRING_WHITE_LAND:
    case XSPRING_BLACK_SPRING_WHITE:
    case XSPRING_WHITE_SPRING_BLACK:
        animation_length = 2;
        animation_speed = 30;
        break;
    case XSPRING_BLACK_TO_IDLE:
    case XSPRING_WHITE_TO_IDLE:
        animation_length = 5;
        animation_speed = 30;
        break;
    }
    int frame = ((SDL_GetTicks() - animation_start) / animation_speed);
    if (frame >= animation_length) {
        switch (m_status) {
            case XSPRING_BLACK_TO_IDLE:
            case XSPRING_WHITE_TO_IDLE:
                m_status = XSPRING_IDLE;
                break;
            case XSPRING_BLACK_LAND:
            case XSPRING_BLACK_SPRING_WHITE:
                m_status = XSPRING_BLACK_ON;
                break;
            case XSPRING_WHITE_LAND:
            case XSPRING_WHITE_SPRING_BLACK:
                m_status = XSPRING_WHITE_ON;
                break;
            default:
                break;
        }
        animation_start = SDL_GetTicks();
    }
    frame = ((SDL_GetTicks() - animation_start) / animation_speed);
    int tex_w = m_tex.get_width();
    int tex_h = m_tex.get_height();
    int render_x = m_rect.x - (tex_w-m_rect.w)/2;
    int render_y = m_rect.y - (m_tex.get_height()-m_rect.h) / 2;
    SDL_Rect frame_clip = {frame*tex_w, m_status*tex_h, tex_w, tex_h};
    m_tex.render(render_x, render_y, &frame_clip, cam);
}

void XSpring::update_y()
{
    // check if the chars are still here
    bool still_on = false;
    if (dot_on) {
        if (check_touching_vert(dot_on->get_rect(), get_land_rect())) {
            still_on = true;
        }
    }
    if (!still_on) {
        switch (m_status)
        {
        case XSPRING_BLACK_ON:
            m_status = XSPRING_BLACK_TO_IDLE;
            break;
        case XSPRING_WHITE_ON:
            m_status = XSPRING_WHITE_TO_IDLE;
            break;
        default:
            break;
        }
        dot_on = NULL;
    }
}

void XSpring::white_land(Zonestate* zone, Dot* white)
{
    animation_start = SDL_GetTicks();
    if (dot_on && dot_on != white) {
        float velocity = std::min(white->get_yvel(), (float) -3.7);
        dot_on->spring_me(velocity);
        zone->shift();
        m_status = XSPRING_WHITE_SPRING_BLACK;
        animation_start = SDL_GetTicks();
    } else {
        if (m_status == XSPRING_IDLE) {
            m_status = XSPRING_WHITE_LAND;
            animation_start = SDL_GetTicks();
        }
    }
    dot_on = white;
}

void XSpring::black_land(Zonestate* zone, Dot* black)
{
    animation_start = SDL_GetTicks();
    if (dot_on && dot_on != black) {
        float velocity = std::max(black->get_yvel(), (float) 3.7);
        dot_on->spring_me(velocity);
        zone->shift();
        m_status = XSPRING_BLACK_SPRING_WHITE;
        animation_start = SDL_GetTicks();
    } else {
        if (m_status == XSPRING_IDLE) {
            m_status = XSPRING_BLACK_LAND;
            animation_start = SDL_GetTicks();
        }
    }
    dot_on = black;
}

SDL_Rect XSpring::get_land_rect()
{
    SDL_Rect land_rect;
    land_rect.x = m_rect.x;
    land_rect.w = m_rect.w;
    land_rect.h = 20;
    switch (m_status)
    {
    case XSPRING_IDLE:
        land_rect.y = m_rect.y - (m_tex.get_height() - m_rect.h) / 2 + 6;
        break;
    case XSPRING_WHITE_ON:
        land_rect.y = m_rect.y - (m_tex.get_height() - m_rect.h) / 2 + 1;
        land_rect.h += 5;
        break;
    case XSPRING_BLACK_ON:
        land_rect.y = m_rect.y - (m_tex.get_height() - m_rect.h) / 2 + 6;
        land_rect.h += 5;
        break;
    default:
        break;
    }
    return land_rect;
}

/*************/
/*   BLOCK   */
/*************/

/*
Block::Block(int x, int y, int w, int h, TileType blocktype, Level* lvl, SDL_Color palette)
{
    m_rect.x = x;
    m_rect.y = y;
    m_rect.w = w;
    m_rect.h = h;
    m_lvl = lvl;
    std::vector<Tile>* tileset = m_lvl->get_tileset();
    m_tiletype = blocktype;

    int lvl_w = m_lvl->get_w();
    int idx;
    for (int i = 0; i < m_rect.w*m_rect.h; i++) {
        idx = (m_rect.y + i / m_rect.w) * lvl_w + (m_rect.x + i % m_rect.w);
        saved_tiles.push_back((*tileset)[idx].get_type());
        (*tileset)[idx].set_type(m_tiletype);
    }
}

Block::~Block()
{
    int lvl_w = m_lvl->get_w();
    std::vector<Tile>* tileset = m_lvl->get_tileset();
    int idx;
    for (int i = 0; i < m_rect.w*m_rect.h; i++) {
        idx = (m_rect.y + i / m_rect.w) * lvl_w + (m_rect.x + i % m_rect.w);
        (*tileset)[idx].set_type(saved_tiles[i]);
    }
}
*/

/**************/
/*   BUTTON   */
/**************/

