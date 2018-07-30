
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header file
#include <char.hpp>
#include <engine.hpp>
#include <textures.hpp>
#include <levels.hpp>
#include <objects.hpp>
#include <tiles.hpp>
#include <editor.hpp>

static const int DEAD_ZONE = 8000;

static const int DOT_VEL = 3;
static const int JUMP_VEL = 6;
static const float DOT_ACC = .5;
static const int PUSH_VEL = 2;

Dot::Dot(int x, int y, bool color, SDL_Renderer* rend, SDL_Color* palette)
{
    // initialize velocity
    m_xvel = 0;
    m_yvel = 0;

    // initialize controllers
    up = false;
    down = false;
    right = false;
    left = false;
    dir = DIR_RIGHT;
    ready = false;
    snapped = false;
    exiting = exited = entering = false;

    // initiliaze gamepad
    controller = new class Controller;

    m_color = color;
    if (m_color == 0) {
        m_status = CHAR_IDLE;
        if (!m_tex.load_object(16, 16, "char-sheet-black.png", rend, palette))
        {
            printf("Failed to load black dot texture!\n");
            return;
        }
    } else {
        m_status = CHAR_INACTIVE;
        if (!m_tex.load_object(16, 16, "char-sheet-white.png", rend, palette))
        {
            printf("Failed to load white dot texture!\n");
            return;
        }
    }

    // initialize collision rectangle
    col_rect.w = 12;
    col_rect.h = 15;
    col_rect.x = x*TILE_WIDTH;
    col_rect.y = y*TILE_WIDTH - (m_color == 0)*(col_rect.h-TILE_WIDTH);;
    true_y = col_rect.y;
}

bool Dot::handle_event(SDL_Event &e, Zonestate* zone, Engine* game)
{
    if (exiting || entering) {
        return true;
    }
    // handle those events, duder
    switch (e.type)
    {
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode)
            {
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_SPACE:
                    up = true;
                    break;
                case SDL_SCANCODE_DOWN:
                case SDL_SCANCODE_S:
                    down = true;
                    break;
                case SDL_SCANCODE_LEFT:
                case SDL_SCANCODE_A:
                    left = true;
                    break;
                case SDL_SCANCODE_RIGHT:
                case SDL_SCANCODE_D:
                    right = true;
                    break;
                case SDL_SCANCODE_Q:
                case SDL_SCANCODE_LSHIFT:
                case SDL_SCANCODE_RSHIFT:
                    if (zone->shiftable) {
                        Mix_PlayChannel(-1, game->sound->level_switch_snd, 0);
                        up = left = down = right = 0;
                        m_xvel = 0;
                        zone->shift();
                    }
                    break;
                case SDL_SCANCODE_R:
                    game->restart_state();
                    break;
                case SDL_SCANCODE_E:
                    game->change_state(new ZoneEditor());
                    break;
                case SDL_SCANCODE_ESCAPE:
                    zone->pause(game);
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (e.key.keysym.scancode)
            {
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_SPACE:
                    up = false;
                    break;
                case SDL_SCANCODE_DOWN:
                case SDL_SCANCODE_S:
                    down = false;
                    break;
                case SDL_SCANCODE_LEFT:
                case SDL_SCANCODE_A:
                    left = false;
                    break;
                case SDL_SCANCODE_RIGHT:
                case SDL_SCANCODE_D:
                    right = false;
                    break;
                default:
                    break;
            }
            break;
        case SDL_JOYAXISMOTION:
            if (e.jaxis.which == 0)
            {
                switch (e.jaxis.axis)
                {
                    case 0:
                        if (e.jaxis.value > DEAD_ZONE)
                        {
                            right = true;
                            left = false;
                            break;
                        }
                        else if (e.jaxis.value < -(DEAD_ZONE))
                        {
                            left = true;
                            right = false;
                            break;
                        }
                        else
                        {
                            left = false;
                            right = false;
                            break;
                        }
                        break;
                    case 1:
                        if (e.jaxis.value > DEAD_ZONE)
                        {
                            down = true;
                            break;
                        }
                        else if (e.jaxis.value < -(DEAD_ZONE))
                        {
                            // up = true;
                            break;
                        }
                        else
                        {
                            // up = false;
                            down = false;
                            break;
                        }
                        break;
                }
            }
            break;
        case SDL_CONTROLLERBUTTONDOWN:
            switch (e.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_A:
                    up = true;
                    break;
                case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                case SDL_CONTROLLER_BUTTON_X:
                    if (zone->shiftable)
                    {
                        Mix_PlayChannel(-1, game->sound->level_switch_snd, 0);
                        up = left = down = right = 0;
                        zone->shift();
                    }
                    break;
                case SDL_CONTROLLER_BUTTON_Y:
                    game->restart_state();
                    break;
                case SDL_CONTROLLER_BUTTON_START:
                    return false;
                    break;
            }
            break;
        case SDL_CONTROLLERBUTTONUP:
            switch (e.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_A:
                    up = false;
                    break;
            }
        default:
            break;
    }
    // success! (no quitting)
    return true;
}

void Dot::update(Zonestate* zone, Engine* game)
{
    if (zone->active_color != m_color) {
        m_status = CHAR_INACTIVE;
        return;
    }

    if (exited) {
        zone->check_exit();
        if (exited) {
            enter();
        }
    }

    // update x velocity
    if (right && !left)
        m_xvel += DOT_ACC;
    if (left && !right)
        m_xvel -= DOT_ACC;
    if ((!right && !left) || (right && left)) {
        if (m_xvel < 0) {
            m_xvel += DOT_ACC;
        } else if (m_xvel > 0) {
            m_xvel -= DOT_ACC;
        }
    }

    // if the char is moving slowly enough, stop it
    if (m_xvel < (DOT_ACC) && m_xvel > -(DOT_ACC)) {
        m_xvel = 0;
        if (m_status != CHAR_JUMP) {
            m_status = CHAR_IDLE;
        }
    }

    // limit top speed
    if (m_xvel > DOT_VEL) {
        m_xvel = DOT_VEL;
    } else if (m_xvel < -DOT_VEL)
        m_xvel = -DOT_VEL;

    // set direction (for animation)
    if (m_xvel < 0) {
        dir = DIR_LEFT;
    } else if (m_xvel > 0) {
        dir = DIR_RIGHT;
    }

    if (m_xvel != 0 && m_status != CHAR_JUMP) {
        m_status = CHAR_RUN;
    }

    if (m_status != CHAR_JUMP) {
        short_hop = -1;
    }

    Level* level = zone->get_active_level();
    std::vector<Tile> tileset = level->get_tileset();

    // move that Dot (x)
    col_rect.x += m_xvel;

    if (entering && (exit_dir == EXIT_LEFT || exit_dir == EXIT_RIGHT)) {
        int lvl_x = level->get_x();
        int lvl_y = level->get_y();
        int lvl_w = level->get_w()*TILE_WIDTH;
        int lvl_h = level->get_h()*TILE_WIDTH;
        if (col_rect.x > lvl_x && col_rect.x + col_rect.w < lvl_x + lvl_w &&
            col_rect.y > lvl_y && col_rect.y + col_rect.h < lvl_y + lvl_h) {
            entering = false;
            up = down = left = right = false;
        } else {
            return;
        }
    }

    if (exiting) {
        int lvl_x = level->get_x();
        int lvl_y = level->get_y();
        int lvl_w = level->get_w()*TILE_WIDTH;
        int lvl_h = level->get_h()*TILE_WIDTH;
        if (col_rect.x + col_rect.w < lvl_x || col_rect.x > lvl_x + lvl_w) {
            exiting = false;
            exited = true;
        }
        return;
    }

    // resolve x collisions
    Vector repos;
    int size = level->get_w()*level->get_h();
    for (int i = 0; i < size; i++) {
        TileType type = tileset[i].get_type();
        if (type == TILE_BLACK_SOLID + !m_color ||  type == TILE_CLEAR) {
            continue;
        }
        if (check_collision(col_rect, tileset[i].get_col_rect(), &repos)) {
            if (type == TILE_BLACK_PLATFORM + m_color) {
                platform_drop = true;
                continue;
            }
            col_rect.x += repos.x;
            SDL_Rect tile_rect = tileset[i].get_col_rect();
            if ((repos.x > 0 && m_xvel < 0) ||
                (repos.x < 0 && m_xvel > 0)) {
                m_xvel = 0;
            }
        }
    }

    // update y velocity with gravity
    if (m_status == CHAR_JUMP) {
        int jump_duration = SDL_GetTicks() - jump_start;
        if (jump_duration > 100 && jump_duration < 150) {
            if (short_hop == -1) {
                short_hop = !up;
            }
        } else if (jump_duration >= 150) {
            short_hop = 0;
        }
    }
    m_yvel += (!m_color - m_color) * GRAVITY * ((short_hop > 0)*3.2 + 1);

    // move that Dot (y)
    true_y += m_yvel;
    col_rect.y = (int) true_y + (m_color == 0);

    if (entering) {
        int lvl_x = level->get_x();
        int lvl_y = level->get_y();
        int lvl_w = level->get_w()*TILE_WIDTH;
        int lvl_h = level->get_h()*TILE_WIDTH;
        if (col_rect.x > lvl_x && col_rect.x + col_rect.w < lvl_x + lvl_w &&
            col_rect.y > lvl_y && col_rect.y + col_rect.h < lvl_y + lvl_h) {
            entering = false;
            up = down = left = right = false;
        } else {
            return;
        }
    }

    // resolve y collisions
    bool shiftable;
    bool airborne = true;
    bool platform_col = false;
    for (int i = 0; i < size; i++) {
        TileType type = tileset[i].get_type();
        if (type == TILE_BLACK_SOLID + !m_color || type == TILE_CLEAR) {
            continue;
        }
        if (check_collision(col_rect, tileset[i].get_col_rect(), &repos)) {
            if (type == TILE_BLACK_PLATFORM + m_color) {
                platform_col = true;
                if (platform_drop || (!m_color - m_color)*m_yvel < 0) {
                    continue;
                }
            }
            if ((m_color == 0 && repos.y < 0) ||
                (m_color == 1 && repos.y > 0)) {
                // land!
                if (m_status == CHAR_JUMP) {
                    m_status = CHAR_IDLE;
                }
                col_rect.y += repos.y;
                true_y = col_rect.y;
                m_yvel = 0;
                shiftable = true;
                // jump!
                if (up) {
                    m_status = CHAR_JUMP;
                    jump_start = SDL_GetTicks();
                    m_yvel = (m_color - !m_color) * JUMP_VEL;
                    shiftable = false;
                } else if (down) {
                    platform_drop = true;
                } else {
                    airborne = false;
                }
            } else {
                // ceiling
                if ((m_color == 0 && m_yvel < 0) || (m_color == 1 && m_yvel > 0)) {
                    // adjust y pos
                    col_rect.y += repos.y;
                    true_y = col_rect.y;
                    m_yvel = ((m_color == 0) - (m_color == 1)) * .1;
                    short_hop = 0;
                    shiftable = false;
                    up = false;
                }
            }
            if (airborne && check_grounded(col_rect, tileset[i].get_col_rect(), m_color)) {
                airborne = false;
            }
        }
    }
    if (airborne && m_status != CHAR_JUMP) {
        m_status = CHAR_JUMP;
        jump_start = 0;
    }
    if (!platform_col) {
        platform_drop = false;
    }

    std::vector<Object*> objects = level->get_objects();
    Spring* spring;
    for (int i = 0; i < objects.size(); i++) {
        if (check_collision(col_rect, objects[i]->get_rect(), &repos)) {
            switch (objects[i]->get_type())
            {
            case OBJECT_SPRING:
                spring = (Spring*) objects[i];
                spring->spring();
                spring_me(spring->get_yvel());
                break;
            default:
                break;
            }
        }
    }

    int lvl_x = level->get_x();
    int lvl_y = level->get_y();
    int lvl_w = level->get_w()*TILE_WIDTH;
    int lvl_h = level->get_h()*TILE_WIDTH;
    if (col_rect.x < lvl_x) {
        exit(EXIT_LEFT);
    } else if (col_rect.x + col_rect.w > lvl_x + lvl_w) {
        exit(EXIT_RIGHT);
    } else if (col_rect.y + col_rect.h < lvl_y) {
        exit(EXIT_UP);
    } else if (col_rect.y > lvl_y + lvl_h) {
        exit(EXIT_DOWN);
    }

    zone->shiftable = shiftable;
}

void Dot::exit(ExitDir dir)
{
    exiting = true;
    exit_dir = dir;
    up = down = false;
    switch (exit_dir)
    {
    case EXIT_LEFT:
        left = true;
        right = false;
        break;
    case EXIT_RIGHT:
        right = true;
        left = false;
        break;
    case EXIT_UP:
    case EXIT_DOWN:
        exited = true;
        m_yvel = m_xvel = 0;
        left = right = false;
        break;
    }
}

void Dot::enter()
{
    exiting = exited = false;
    entering = true;
    switch (exit_dir)
    {
    case EXIT_LEFT:
        right = true;
        left = up = down = false;
        break;
    case EXIT_RIGHT:
        left = true;
        right = up = down = false;
        break;
    case EXIT_DOWN:
        m_yvel = (m_color == 0)*-5;
        break;
    case EXIT_UP:
        m_yvel = (m_color == 1)*5;
        break;
    }
}

void Dot::good_exit()
{
    exiting = exited = entering = false;
    up = down = left = right = false;
    return;
}

bool Dot::in_level(Level* lvl)
{
    int lvl_x = lvl->get_x();
    int lvl_y = lvl->get_y();
    int lvl_w = lvl->get_w()*TILE_WIDTH;
    int lvl_h = lvl->get_h()*TILE_WIDTH;
    return (col_rect.x + col_rect.w > lvl_x && col_rect.x < lvl_x + lvl_w &&
            col_rect.y + col_rect.h > lvl_y && col_rect.y < lvl_y + lvl_h);
}

void Dot::render(SDL_Rect* camera, Engine* game)
{
    int animation_length;
    double animation_speed;
    switch (m_status) {
        case CHAR_IDLE:
            animation_speed = 100.0;
            animation_length = 6;
            break;
        case CHAR_RUN:
            animation_speed = 30.0;
            animation_length = 12;
            break;
        case CHAR_JUMP:
            animation_speed = 100.0;
            animation_length = 5;
            break;
        case CHAR_INACTIVE:
            animation_speed = 150.0;
            animation_length = 10;
            break;
        default:
            animation_speed = 100;
            animation_length = 1;
            break;
    }

    int frame;
    if (m_status != CHAR_JUMP) {
        frame = ((int) ((float) SDL_GetTicks() / animation_speed)) % animation_length;
    } else {
        frame = ((int) (((float) SDL_GetTicks() - (float) jump_start)
                / animation_speed));
        if (frame >= animation_length) {
            frame = animation_length - 1;
        }
    }

    // relevant clips
    SDL_Rect frame_clip = {frame * 16, m_status * 16, 16, 16};
    int render_x = col_rect.x + (col_rect.w - m_tex.get_width()) / 2;
    int render_y = col_rect.y + (m_color == 0) * (col_rect.h - m_tex.get_height());
    m_tex.render(render_x, render_y, &frame_clip, camera, game, dir, m_color);
};

void Dot::move(int x, int y)
{
    up = down = right = left = false;
    m_xvel = m_yvel = 0;
    m_status = CHAR_IDLE;
    col_rect.x += x;
    col_rect.y += y;
    true_y = col_rect.y;
}

void Dot::spring_me(float yvel)
{
    jump_start = SDL_GetTicks();
    m_yvel = yvel;
}
