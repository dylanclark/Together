
#include <objects.hpp>

/********************/
/*   CROSS SPRING   */
/********************/

static const float XSPRING_MIN_HEIGHT = 4.2;

XSpring::XSpring(int x, int y, SDL_Color palette)
{
    Texture m_tex = ResourceManager::get_texture("cross_spring");
    Texture m_normalmap = ResourceManager::get_texture("xspring_normal");
    m_sprite = Sprite(m_tex, m_normalmap, 16, 32, &palette);
    m_has_light = false;
    m_rect.w = 12;
    m_rect.h = 10;
    m_rect.x = x + (m_sprite.get_width() - m_rect.w) / 2;
    m_rect.y = y + (m_sprite.get_height() - m_rect.h) / 2;
    m_type = OBJECT_XSPRING;
    m_status = XSPRING_IDLE;
    animation_start = SDL_GetTicks();
    dot_on = NULL;
}

void XSpring::render_bg(Camera* cam, std::vector<Light> lights, bool active_color)
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
    int tex_w = m_sprite.get_width();
    int tex_h = m_sprite.get_height();
    int render_x = m_rect.x - (tex_w-m_rect.w)/2;
    int render_y = m_rect.y - (m_sprite.get_height()-m_rect.h) / 2;
    SDL_Rect frame_clip = {frame*tex_w, m_status*tex_h, tex_w, tex_h};
    m_sprite.render(render_x, render_y, &frame_clip, cam, lights);
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
        float velocity = std::min(white->get_yvel(), -XSPRING_MIN_HEIGHT);
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
        float velocity = std::max(black->get_yvel(), XSPRING_MIN_HEIGHT);
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
        land_rect.y = m_rect.y - (m_sprite.get_height() - m_rect.h) / 2 + 6;
        break;
    case XSPRING_WHITE_ON:
        land_rect.y = m_rect.y - (m_sprite.get_height() - m_rect.h) / 2 + 1;
        land_rect.h += 5;
        break;
    case XSPRING_BLACK_ON:
        land_rect.y = m_rect.y - (m_sprite.get_height() - m_rect.h) / 2 + 6;
        land_rect.h += 5;
        break;
    default:
        break;
    }
    return land_rect;
}

SmallLamp::SmallLamp(int x, int y, bool color, float strength, SDL_Color palette)
{
    m_color = color;
    m_rect.w = 16;
    m_rect.h = 16;
    m_rect.x = x - TILE_WIDTH/2;
    m_rect.y = y - TILE_WIDTH/2;
    m_has_light = true;
    m_light = Light(m_rect.x + m_rect.w / 2, m_rect.y + m_rect.h / 2, strength, palette);

    Texture m_tex = ResourceManager::get_texture("small_lamp");
    Texture m_normalmap = ResourceManager::get_texture("small_lamp_normal");
    m_sprite = Sprite(m_tex, m_normalmap, 16, 16, &palette);
    m_type = OBJECT_SMALL_LAMP;
}

void SmallLamp::render_bg(Camera* cam, std::vector<Light> lights, bool active_color)
{
    SDL_Rect clip_rect;
    clip_rect.x = (m_color == 1)*m_rect.w;
    clip_rect.y = 0;
    clip_rect.w = m_rect.w;
    clip_rect.h = m_rect.h;
    m_sprite.render(m_rect.x, m_rect.y, &clip_rect, cam, lights, 0, m_color, true);
}


