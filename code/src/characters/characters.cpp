
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

// reinitialize character textures
extern Texture b_end_animate;
extern Texture w_end_animate;

static const int DOT_VEL = 3;
static const int JUMP_VEL = 6;
static const float DOT_ACC = .5;
static const float GRAVITY = .25;
static const int PUSH_VEL = 2;

typedef enum _char_dir {
    DIR_RIGHT = 0,
    DIR_LEFT
} char_dir;

typedef enum _char_status {
    CHAR_IDLE = 0,
    CHAR_RUN,
    CHAR_JUMP,
    CHAR_INACTIVE
} char_status;

Dot::Dot(int x, int y, bool color, SDL_Renderer* rend, SDL_Color* palette)
{
    // initialize velocity
    x_vel = 0;
    y_vel = 0;

    // initialize controllers
    up = false;
    down = false;
    right = false;
    left = false;
    dir = 0;
    snapped = false;

    // initiliaze gamepad
    controller = new class Controller;

    my_color = color;
    if (my_color == 0) {
        status = CHAR_IDLE;
        if (!tex.load_tile_sheet("char-sheet-black.png", rend, palette))
        {
            printf("Failed to load black dot texture!\n");
            return;
        }
    } else {
        status = CHAR_INACTIVE;
        if (!tex.load_tile_sheet("char-sheet-white.png", rend, palette))
        {
            printf("Failed to load white dot texture!\n");
            return;
        }
    }

    // initialize collision rectangle
    col_rect.w = TILE_WIDTH;
    col_rect.h = TILE_WIDTH;
    col_rect.x = x*TILE_WIDTH;
    col_rect.y = y*TILE_WIDTH;
    true_y = col_rect.y;
}

bool Dot::handle_event_old(SDL_Event &e, Levelstate* level, Engine* game)
{
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
                    if (level->shiftable) {
                        Mix_PlayChannel(-1, game->sound->level_switch_snd, 0);
                        up = left = down = right = 0;
                        level->shift();
                    }
                    break;
                case SDL_SCANCODE_R:
                    game->restart_state();
                    break;
                case SDL_SCANCODE_E:
                    game->change_state(new ZoneEditor());
                    break;
                case SDL_SCANCODE_ESCAPE:
                    level->pause(game);
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
                    if (level->shiftable)
                    {
                        Mix_PlayChannel(-1, game->sound->level_switch_snd, 0);
                        status = (status + 1) % 4;
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
    }
    // success! (no quitting)
    return true;
}

bool Dot::handle_event(SDL_Event &e, Zonestate* zone, Engine* game)
{
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
                        status = (status + 1) % 4;
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
    }
    // success! (no quitting)
    return true;
}


void Dot::update_old(Levelstate* level, Engine* game)
{
    if (level->active_color != my_color) {
        status = CHAR_INACTIVE;
        return;
    }

    if (status != CHAR_JUMP) {
        short_hop = -1;
    }

    // update y velocity with gravity
    if (status == CHAR_JUMP) {
        int jump_duration = SDL_GetTicks() - jump_start;
        if (jump_duration > 100 && jump_duration < 150) {
            if (short_hop == -1) {
                short_hop = !up;
            }
        } else if (jump_duration >= 150) {
            short_hop = 0;
        }
    }
    y_vel += (!my_color - my_color) * GRAVITY * ((short_hop > 0)*3.2 + 1);

    // update x velocity
    if (right && !left)
        x_vel += DOT_ACC;
    if (left && !right)
        x_vel -= DOT_ACC;
    if ((!right && !left) || (right && left)) {
        if (x_vel < 0) {
            x_vel += DOT_ACC;
        } else if (x_vel > 0) {
            x_vel -= DOT_ACC;
        }
    }

    // if the char is moving slowly enough, stop it
    if (x_vel < (DOT_ACC) && x_vel > -(DOT_ACC)) {
        x_vel = 0;
        if (status != CHAR_JUMP) {
            status = CHAR_IDLE;
        }
    }

    // limit top speed
    if (x_vel > DOT_VEL) {
        x_vel = DOT_VEL;
    } else if (x_vel < -DOT_VEL)
        x_vel = -DOT_VEL;

    // set direction (for animation)
    if (x_vel < 0) {
        dir = DIR_LEFT;
    } else if (x_vel > 0) {
        dir = DIR_RIGHT;
    }

    if (x_vel != 0 && status != CHAR_JUMP) {
        status = CHAR_RUN;
    }

    // move that Dot
    col_rect.x += x_vel;
    true_y += y_vel;

    // check edges
    if (col_rect.x < 0) {
        col_rect.x = 0;
    }
    if (true_y < 0) {
        true_y = 0;
        y_vel = 0;
    }
    if (col_rect.x + col_rect.w > level->width * TILE_WIDTH) {
        col_rect.x = level->width * TILE_WIDTH - col_rect.w;
    }
    if (true_y + col_rect.h > level->height * TILE_WIDTH) {
        true_y = level->height * TILE_WIDTH - col_rect.h;
        y_vel = 0;
    }

    col_rect.y = (int) true_y;

    // deal with crate collisions using crate_col
    if (!crate_col(level, game)) {
        level->shiftable = tile_col(level->tileset, level->width * level->height, game);
    }
}

void Dot::update(Zonestate* zone, Engine* game)
{
    if (zone->active_color != my_color) {
        status = CHAR_INACTIVE;
        return;
    }

    if (status != CHAR_JUMP) {
        short_hop = -1;
    }

    // update y velocity with gravity
    if (status == CHAR_JUMP) {
        int jump_duration = SDL_GetTicks() - jump_start;
        if (jump_duration > 100 && jump_duration < 150) {
            if (short_hop == -1) {
                short_hop = !up;
            }
        } else if (jump_duration >= 150) {
            short_hop = 0;
        }
    }
    y_vel += (!my_color - my_color) * GRAVITY * ((short_hop > 0)*3.2 + 1);

    // update x velocity
    if (right && !left)
        x_vel += DOT_ACC;
    if (left && !right)
        x_vel -= DOT_ACC;
    if ((!right && !left) || (right && left)) {
        if (x_vel < 0) {
            x_vel += DOT_ACC;
        } else if (x_vel > 0) {
            x_vel -= DOT_ACC;
        }
    }

    // if the char is moving slowly enough, stop it
    if (x_vel < (DOT_ACC) && x_vel > -(DOT_ACC)) {
        x_vel = 0;
        if (status != CHAR_JUMP) {
            status = CHAR_IDLE;
        }
    }

    // limit top speed
    if (x_vel > DOT_VEL) {
        x_vel = DOT_VEL;
    } else if (x_vel < -DOT_VEL)
        x_vel = -DOT_VEL;

    // set direction (for animation)
    if (x_vel < 0) {
        dir = DIR_LEFT;
    } else if (x_vel > 0) {
        dir = DIR_RIGHT;
    }

    if (x_vel != 0 && status != CHAR_JUMP) {
        status = CHAR_RUN;
    }

    // move that Dot
    col_rect.x += x_vel;
    true_y += y_vel;

    col_rect.y = (int) true_y;

    Level* level = zone->get_active_level();
    // TODO crate collisions
    zone->shiftable = tile_col(level->get_tileset(), level->get_w() * level->get_h(), game);
}


// deal with tile collisions, return whether the dot can shift
bool Dot::tile_col(Tile* tileset[], int size, Engine* game)
{
    Vector repos;
    bool shiftable;
    bool airborne = true;

    for (int i = 0, n = size; i < n; i++) {
        if (tileset[i]->my_color != my_color) {
            continue;
        }
        // store reposition vector
        if (check_collision(col_rect, tileset[i]->get_col_rect(), &repos)) {
            // wall
            if (tileset[i]->wall && !tileset[i]->floor && !tileset[i]->ceiling) {
                col_rect.x += repos.x;
                SDL_Rect tile_rect = tileset[i]->get_col_rect();
                if ((repos.x > 0 && x_vel < 0 && col_rect.x > tile_rect.x) ||
                    (repos.x < 0 && x_vel > 0 && col_rect.x < tile_rect.x)) {
                    x_vel = 0;
                }
            }
            // floor
            else if (tileset[i]->floor && !tileset[i]->wall) {
                // land!
                if (status == CHAR_JUMP) {
                    status = CHAR_IDLE;
                }
                col_rect.y += repos.y;
                true_y = col_rect.y;
                y_vel = 0;
                shiftable = true;
                // jump!
                if (up) {
                    status = CHAR_JUMP;
                    jump_start = SDL_GetTicks();
                    y_vel += (my_color - !my_color) * JUMP_VEL;
                    shiftable = false;
                } else {
                    airborne = false;
                }
            }
            // floor edge
            else if (tileset[i]->floor && tileset[i]->wall) {
                SDL_Rect tile_rect = tileset[i]->get_col_rect();
                if (abs(repos.x) <= abs(repos.y)) {
                    col_rect.x += repos.x;
                    SDL_Rect tile_rect = tileset[i]->get_col_rect();
                    if ((repos.x > 0 && x_vel < 0 && col_rect.x > tile_rect.x) ||
                        (repos.x < 0 && x_vel > 0 && col_rect.x < tile_rect.x)) {
                        x_vel = 0;
                    }
                } else if ((my_color == 0 && y_vel >= 0 && col_rect.y < tile_rect.y) ||
                           (my_color == 1 && y_vel <= 0 && repos.y >= 0)) {
                    // land!
                    if (status == CHAR_JUMP) {
                        status = CHAR_IDLE;
                    }
                    col_rect.y += repos.y;
                    true_y = col_rect.y;
                    y_vel = 0;
                    shiftable = true;
                    // jump!
                    if (up) {
                        status = CHAR_JUMP;
                        jump_start = SDL_GetTicks();
                        y_vel += (my_color - !my_color) * JUMP_VEL;
                        shiftable = false;
                    } else {
                        airborne = false;
                    }
                }
            }
            // ceiling
            else if (tileset[i]->ceiling && !tileset[i]->wall) {
                if ((my_color == 0 && y_vel < 0) || (my_color == 1 && y_vel > 0)) {
                    // adjust y pos
                    col_rect.y += repos.y;
                    true_y = col_rect.y;
                    y_vel = ((my_color == 0) - (my_color == 1)) * .1;
                    short_hop = 0;
                    shiftable = false;
                }
            }
            // ceiling edge
            else if (tileset[i]->ceiling && tileset[i]->wall) {
                if (abs(repos.x) <= abs(repos.y)) {
                    // adjust x pos
                    col_rect.x += repos.x;
                    SDL_Rect tile_rect = tileset[i]->get_col_rect();
                    if ((repos.x > 0 && x_vel < 0 && col_rect.x > tile_rect.x) ||
                        (repos.x < 0 && x_vel > 0 && col_rect.x < tile_rect.x)) {
                        x_vel = 0;
                    }
                    shiftable = false;
                } else if ((my_color == 0 && y_vel < 0) || (my_color == 1 && y_vel > 0)) {
                    // adjust y pos
                    col_rect.y += repos.y;
                    true_y = col_rect.y;
                    y_vel = ((my_color == 0) - (my_color == 1)) * .1;
                    short_hop = 0;
                    shiftable = false;
                }
            }
        }
    }
    if (airborne && status != CHAR_JUMP) {
        status = CHAR_JUMP;
        jump_start = 0;
    }
    return shiftable;
}

// deal with crate collisions
bool Dot::crate_col(Levelstate* level, Engine* game)
{
    Vector repos;

    // check all crates
    for (int i = 0; i < level->crates.size(); i++) {
        level->crates[i]->pushed = false;

        // if theres a collision
        if (check_collision(col_rect, level->crates[i]->get_col_rect(), &repos)) {
            if (my_color != level->crates[i]->my_color) {
                level->shiftable = false;
                tile_col(level->crates[i]->tileset, MAX_BORDER, game);
                return true;
            } else {
                // push and move the crate
                if (abs(repos.x) <= abs(repos.y)) {
                    // tell the crate its being pushed
                    level->crates[i]->pushed = true;

                    // adjust Dot speed
                    if (x_vel > PUSH_VEL || x_vel < -PUSH_VEL) {
                        x_vel = (x_vel > 0) ? PUSH_VEL : -PUSH_VEL;
                    }

                    // move crate
                    level->crates[i]->col_rect.x += x_vel;
                    level->crates[i]->check_col(level->crates[i]->get_col_rect(), level, &repos);
                    level->crates[i]->x_vel = 0;

                    // correct Dot position
                    check_collision(col_rect, level->crates[i]->get_col_rect(), &repos);
                    col_rect.x += repos.x;

                    level->shiftable = false;
                }
                // land on crate
                else
                {
                    // halt
                    if (status == CHAR_JUMP) {
                        status = CHAR_IDLE;
                    }
                    true_y += repos.y;
                    y_vel = 0;
                    level->shiftable = true;

                    if (true_y > level->crates[i]->get_col_rect().y) {
                        y_vel = !my_color - my_color;
                        short_hop = 0;
                    }
                    // jump! (if you want)
                    if (up) {
                        status = CHAR_JUMP;
                        jump_start = SDL_GetTicks();
                        y_vel -= JUMP_VEL;
                    }
                }
            }
        }
    }
    // if no collisions were detected, return false
    return false;
}

void Dot::render(SDL_Rect* camera, Engine* game)
{
    int animation_length;
    double animation_speed;
    switch (status) {
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
    }

    int frame;
    if (status != CHAR_JUMP) {
        frame = ((int) ((float) SDL_GetTicks() / animation_speed)) % animation_length;
    } else {
        frame = ((int) (((float) SDL_GetTicks() - (float) jump_start)
                / animation_speed));
        if (frame >= animation_length) {
            frame = animation_length - 1;
        }
    }

    // relevant clips
    SDL_Rect frame_clip = {frame * 16, status * 16, 16, 16};
    tex.render(col_rect.x, col_rect.y, &frame_clip, camera, game, dir, my_color);
};

void Dot::move(int x, int y)
{
    col_rect.x += x;
    col_rect.y += y;
}

void Dot::snap(SDL_Rect target)
{
    snapped = true;
    col_rect.x = target.x;
    col_rect.y = target.y;
}

// spring when sprung
void Dot::spring(int x, int y, int direction)
{
    // spring the right way
    y_vel = 0;
    my_color ? y_vel -= y * 2 : y_vel += y * 2;

    // deal with any horizontal springing
    if (direction == FLIP_RIGHT)
    {
        x_vel += x;
    }
    else
    {
        x_vel -= x;
    }
    return;
}

// center Dot on an object
bool Dot::center(SDL_Rect* end_rect)
{
    col_rect.x = end_rect->x;
    col_rect.y = end_rect->y;

    return true;
}
