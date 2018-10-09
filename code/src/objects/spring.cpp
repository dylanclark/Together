
#include <objects.hpp>

/**************/
/*   SPRING   */
/**************/

Spring::Spring(int x, int y, bool color, float y_vel, SDL_Color palette)
{
    m_color = color;
    m_yvel = y_vel;
    m_has_light = false;
    Texture m_tex;
    if (m_color == 0) {
        m_tex = ResourceManager::get_texture("black_spring");
    } else {
        m_tex = ResourceManager::get_texture("white_spring");
    }
    Texture m_normalmap = ResourceManager::get_texture("spring_normal");
    m_sprite = Sprite(m_tex, m_normalmap, 32, 16, &palette);
    m_status = SPRING_IDLE;

    m_type = OBJECT_SPRING;
    m_rect.w = 8;
    m_rect.h = 4;
    m_rect.x = x + (m_sprite.get_width()-m_rect.w)/2;
    m_rect.y = y + (m_color == 0)*(m_sprite.get_height()-m_rect.h);
}

void Spring::render_bg(Camera* cam, std::vector<Light> lights, bool active_color)
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
    int tex_w = m_sprite.get_width();
    int tex_h = m_sprite.get_height();
    int render_x = m_rect.x - (tex_w-m_rect.w)/2;
    int render_y = m_rect.y - (m_color == 0)*(m_sprite.get_height()-m_rect.h);
    SDL_Rect frame_clip = {frame*tex_w, m_status*tex_h, tex_w, tex_h};
    m_sprite.render(render_x, render_y, &frame_clip, cam, lights);
}

void Spring::spring()
{
    spring_start = SDL_GetTicks();
    m_status = SPRING_SPRUNG;
}
