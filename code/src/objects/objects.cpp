
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <objects.hpp>

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

void Spring::render(SDL_Rect cam_rect, bool active_color)
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
    m_tex.render(render_x, render_y, &frame_clip, &cam_rect, 0, m_color);
}

void Spring::spring()
{
    spring_start = SDL_GetTicks();
    m_status = SPRING_SPRUNG;
}
