
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

// reinitialize character textures
extern Texture b_char_tex;
extern Texture w_char_tex;
extern Texture b_end_animate;
extern Texture w_end_animate;

Dot::Dot()
{
    // initialize velocity
    x_vel = 0;
    y_vel = 0;

    // initialize controllers
    up = false;
    down = false;
    right = false;
    left = false;

    // initiliaze gamepad
    controller = new class Controller;

    // initialize active
    status = TILE_ACTIVE;

    // initialize animation
    frame = 0;

    // initialize collision rectangle
    col_rect.w = DOT_W;
    col_rect.h = DOT_H;
    col_rect.x = (SCREEN_WIDTH - col_rect.w) / 2;
    col_rect.y = (SCREEN_HEIGHT - col_rect.h) / 2;
}

bool Dot::handle_event(SDL_Event &e, Levelstate* level, Engine* game)
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
                    if (level->shiftable)
                    {
                        Mix_PlayChannel(-1, game->sound->level_switch_snd, 0);
                        status = (status + 1) % 4;
                    }
                    break;
                case SDL_SCANCODE_R:
                    game->restart_state();
                    break;
                case SDL_SCANCODE_ESCAPE:
                    return false;
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
            }
            break;
        case SDL_JOYAXISMOTION:
            if (e.jaxis.which == 0)
            {
                switch (e.jaxis.axis)
                {
                    case 0:
                        if (e.jaxis.value > controller->DEAD_ZONE)
                        {
                            right = true;
                            left = false;
                            break;
                        }
                        else if (e.jaxis.value < -(controller->DEAD_ZONE))
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
                        if (e.jaxis.value > controller->DEAD_ZONE)
                        {
                            down = true;
                            break;
                        }
                        else if (e.jaxis.value < -(controller->DEAD_ZONE))
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

void Dot::move(Levelstate* level, Engine* game)
{
    if (status != CHAR_ACTIVE)
        return;

    // update y velocity with gravity
    black ? y_vel += GRAVITY : y_vel -= GRAVITY;

    // update x velocity
    if (right && !left)
        x_vel += DOT_ACC;
    if (left && !right)
        x_vel -= DOT_ACC;
    if ((!right && !left) || (right && left))
    {
        if (x_vel < 0)
            x_vel += DOT_ACC;
        else if (x_vel > 0)
            x_vel -= DOT_ACC;
    }

    // if the square is moving slowly enough, stop him
    if (x_vel < (DOT_ACC) && x_vel > -(DOT_ACC))
        x_vel = 0;

    // limit top speed
    if (x_vel > DOT_VEL)
        x_vel = DOT_VEL;
    if (x_vel < -DOT_VEL)
        x_vel = -DOT_VEL;
    if (y_vel > 1.5 * JUMP_VEL)
        y_vel = 1.5 * JUMP_VEL;
    if (y_vel < 1.5 * -JUMP_VEL)
        y_vel = 1.5 * -JUMP_VEL;

    // dont move inactive chars
    if (status != CHAR_ACTIVE)
    {
        x_vel = 0;
        y_vel = 0;
    }

    // move that Dot
    col_rect.x += x_vel;
    col_rect.y += y_vel;


    // deal with crate collisions using crate_col
    if (!crate_col(level, game))
    {
        level->shiftable = tile_col(level->tileset, level->width * level->height, game);
    }

    // check edges
    if (col_rect.x < 0)
    {
        col_rect.x = 0;
        x_vel = 0;
    }
    if (col_rect.y < 0)
    {
        col_rect.y = 0;
        y_vel = 0;
    }
    if (col_rect.x + col_rect.w > level->width * TILE_WIDTH)
    {
        col_rect.x = level->width * TILE_WIDTH - col_rect.w;
        x_vel = 0;
    }
    if (col_rect.y + col_rect.h > level->height * TILE_WIDTH)
    {
        col_rect.y = level->height * TILE_WIDTH - col_rect.h;
        y_vel = 0;
    }
}


// deal with tile collisions
bool Dot::tile_col(Tile* tileset[], int size, Engine* game)
{
    Vector repos;

    bool shiftable;

    // depends on color
    if (black)
    {
        // for every tile
        for (int i = 0, n = size; i < n; i++)
        {
            // store reposition vector
            if (check_collision(col_rect, tileset[i]->get_col_rect(), &repos))
            {
                // black wall
                if (tileset[i]->wall_b && !tileset[i]->floor_b && !tileset[i]->ceiling_b)
                {
                    // halt
                    col_rect.x += repos.x;
                    x_vel = 0;

                    shiftable = false;
                }
                // black floor
                else if (tileset[i]->floor_b && !tileset[i]->wall_b)
                {
                    if (col_rect.y + col_rect.h / 2 >= tileset[i]->get_col_rect().y + tileset[i]->get_col_rect().h / 2) continue;

                    // halt
                    col_rect.y += repos.y;
                    y_vel = 0;

                    shiftable = true;

                    // jump?
                    if (up && status == CHAR_ACTIVE)
                    {
                        y_vel -= JUMP_VEL;
                        Mix_PlayChannel(-1, game->sound->level_b_jump_snd, 0);
                        shiftable = false;
                    }
                }
                // black floor edge
                else if (tileset[i]->floor_b && tileset[i]->wall_b)
                {
                    // determine which is smaller, and use that one!
                    if (abs(repos.x) <= abs(repos.y))
                    {
                        //adjust x pos
                        col_rect.x += repos.x;
                        x_vel = 0;

                        shiftable = false;
                    }
                    else if (y_vel > 0)
                    {
                        if (col_rect.y + col_rect.h / 2 >= tileset[i]->get_col_rect().y + tileset[i]->get_col_rect().h / 2)
                            continue;

                        // adjust y pos
                        col_rect.y += repos.y;
                        y_vel = 0;

                        shiftable = true;

                        // jump! (if you want)
                        if (up && status == CHAR_ACTIVE)
                        {
                            y_vel -= JUMP_VEL;
                            Mix_PlayChannel(-1, game->sound->level_b_jump_snd, 0);
                            shiftable = false;
                        }
                    }
                }

                // black ceiling
                else if (tileset[i]->ceiling_b && !tileset[i]->wall_b)
                {
                    // adjust y pos
                    col_rect.y += repos.y;
                    y_vel = 1;

                    shiftable = false;
                }

                // black ceiling edge
                else if (tileset[i]->ceiling_b && tileset[i]->wall_b)
                {
                    // determine which is smaller, and use that one!
                    if (abs(repos.x) <= abs(repos.y))
                    {
                        //adjust x pos
                        col_rect.x += repos.x;
                        x_vel = 0;

                        shiftable = false;
                    }
                    else if (y_vel < 0)
                    {
                        // adjust y pos
                        col_rect.y += repos.y;
                        y_vel = fabsf(y_vel);

                        shiftable = false;
                    }
                }
            }
        }
        return shiftable;
    }

    // check collisions with white character
    else if (!black)
    {
        // iterate over all tiles
        for (int i = 0, n = size; i < n; i++)
        {
            // store reposition vector
            if (check_collision(col_rect, tileset[i]->get_col_rect(), &repos))
            {
                // white wall
                if (tileset[i]->wall_w && !tileset[i]->floor_w && !tileset[i]->ceiling_w)
                {
                    // halt
                    col_rect.x += repos.x;
                    x_vel = 0;

                    shiftable = false;
                }

                // white floor
                else if (tileset[i]->floor_w && !tileset[i]->wall_w)
                {
                    if (col_rect.y + col_rect.h / 2 <= tileset[i]->get_col_rect().y + tileset[i]->get_col_rect().h / 2)
                        continue;

                    // halt
                    col_rect.y += repos.y;
                    y_vel = 0;

                    shiftable = true;

                    // jump
                    if (up && status == CHAR_ACTIVE)
                    {
                        y_vel += JUMP_VEL;
                        Mix_PlayChannel(-1, game->sound->level_w_jump_snd, 0);
                        shiftable = false;
                    }
                }

                // white floor edge
                else if (tileset[i]->floor_w && tileset[i]->wall_w)
                {
                    // determine which is smaller, and use that one!
                    if (abs(repos.x) <= abs(repos.y))
                    {
                        //adjust x pos (lol expos)
                        col_rect.x += repos.x;
                        x_vel = 0;

                        shiftable = false;
                    }
                    else if (y_vel < 0)
                    {
                        if (col_rect.y + col_rect.h / 2 <= tileset[i]->get_col_rect().y + tileset[i]->get_col_rect().h / 2)
                            continue;

                        // adjust y pos
                        col_rect.y += repos.y;
                        y_vel = 0;

                        shiftable = true;

                        // jump! (if you want)
                        if (up && status == CHAR_ACTIVE)
                        {
                            y_vel += JUMP_VEL;
                            Mix_PlayChannel(-1, game->sound->level_w_jump_snd, 0);
                            shiftable = false;
                        }
                    }
                }

                // white ceiling
                else if (tileset[i]->ceiling_w && !tileset[i]->wall_w)
                {
                    // adjust y pos
                    col_rect.y += repos.y;
                    y_vel = -1;

                    shiftable = false;
                }

                // white ceiling edge
                else if (tileset[i]->ceiling_w && tileset[i]->wall_w)
                {
                    // determine which is smaller, and use that one!
                    if (abs(repos.x) <= abs(repos.y))
                    {
                        //adjust x pos (lol expos)
                        col_rect.x += repos.x;
                        x_vel = 0;

                        shiftable = false;
                    }
                    else if (y_vel < 0)
                    {
                        // adjust y pos
                        col_rect.y += repos.y;
                        y_vel = -fabsf(y_vel);

                        shiftable = false;
                    }
                }
            }
        }
    }
    return shiftable;
}


// deal with crate collisions
bool Dot::crate_col(Levelstate* level, Engine* game)
{
    Vector repos;

    // check all crates
    for (int i = 0; i < level->crates.size(); i++)
    {
        if (black)
        {
            level->crates[i]->pushed = false;

            // if theres a collision
            if (check_collision(col_rect, level->crates[i]->get_col_rect(), &repos))
            {
                if (!level->crates[i]->black)
                {
                    level->shiftable = false;
                    tile_col(level->crates[i]->tileset, MAX_BORDER, game);

                    return true;
                }
                else
                {
                    // push and move the crate
                    if (abs(repos.x) <= abs(repos.y))
                    {
                        // tell the crate its being pushed
                        level->crates[i]->pushed = true;

                        // adjust Dot speed
                        if (x_vel > PUSH_VEL || x_vel < -PUSH_VEL)
                            x_vel = (x_vel > 0) ? PUSH_VEL : -PUSH_VEL;

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
                        col_rect.y += repos.y;
                        y_vel = 0;

                        level->shiftable = true;

                        if (col_rect.y > level->crates[i]->get_col_rect().y)
                        {
                            y_vel = 1;
                        }

                        // jump! (if you want)
                        if (up && status == CHAR_ACTIVE)
                        {
                            y_vel -= JUMP_VEL;
                            Mix_PlayChannel(-1, game->sound->level_b_jump_snd, 0);
                        }
                    }
                }
            }
        }
        else if (!black)
        {
            level->crates[i]->pushed = false;

            if (check_collision(col_rect, level->crates[i]->get_col_rect(), &repos))
            {
                if (level->crates[i]->black)
                {
                    level->shiftable = false;
                    tile_col(level->crates[i]->tileset, MAX_BORDER, game);

                    return true;
                }
                else
                {
                    // push and move the crate
                    if (abs(repos.x) <= abs(repos.y))
                    {
                        // tell the crate its being pushed
                        level->crates[i]->pushed = true;

                        if (!Mix_Playing(2))
                        {
                            Mix_PlayChannel(2, game->sound->level_crate_snd, 0);
                        }

                        // adjust Dot speed
                        if (x_vel > PUSH_VEL || x_vel < -PUSH_VEL)
                            x_vel = (x_vel > 0) ? PUSH_VEL : -PUSH_VEL;

                        // move crate
                        level->crates[i]->col_rect.x += x_vel;
                        level->crates[i]->x_vel = 0;
                        level->crates[i]->check_col(level->crates[i]->get_col_rect(), level, &repos);

                        // correct Dot position
                        check_collision(col_rect, level->crates[i]->get_col_rect(), &repos);
                        col_rect.x += repos.x;

                        level->shiftable = false;
                    }
                    // land on crate
                    else
                    {
                        col_rect.y += repos.y;
                        y_vel = 0;

                        if (col_rect.y < level->crates[i]->get_col_rect().y)
                        {
                            y_vel = -1;
                        }

                        level->shiftable = true;

                        // jump! (if you want)
                        if (up && status == CHAR_ACTIVE && col_rect.y > level->crates[i]->get_col_rect().y)
                        {
                            y_vel += JUMP_VEL;
                            Mix_PlayChannel(-1, game->sound->level_w_jump_snd, 0);
                        }
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
    // relevant clips
    SDL_Rect active_clip = {0, 0, 16, 16};
    SDL_Rect inactive_clip = {16 * ANIMATION_LENGTH, 0, 16, 16};

    switch (status)
    {
        // different cases render appropriate clips
        case CHAR_ACTIVE:
            tex.render(col_rect.x, col_rect.y, &active_clip, camera, game);
            break;
        case CHAR_INACTIVE:
            tex.render(col_rect.x, col_rect.y, &inactive_clip, camera, game);
            break;
        case CHAR_INACTIVATE:
        {
            // next frame!
            frame++;

            // sprite sheet clipper
            SDL_Rect inactivate_clip = {16 * frame, 0, 16, 16};

            // render that
            tex.render(col_rect.x, col_rect.y, &inactivate_clip, camera, game);

            // change the status if animation is over!
            if (frame == ANIMATION_LENGTH - 1)
            {
                frame = 0;
                status = (status + 1) % 4;
            }
            break;
        }
        case CHAR_ACTIVATE:
        {
            // next frame!
            frame++;

            // sprite sheet clipper
            SDL_Rect activate_clip = {16 * (frame + 8), 0, 16, 16};

            // render that
            tex.render(col_rect.x, col_rect.y, &activate_clip, camera, game);

            // change the status if animation is over!
            if (frame == ANIMATION_LENGTH - 1)
            {
                frame = 0;
                status = (status + 1) % 4;
            }
            break;
        }
    }
};

// spring when sprung
void Dot::spring(int x, int y, int direction)
{
    // become active if not
    status = CHAR_ACTIVE;

    // spring the right way
    y_vel = 0;
    black ? y_vel -= y * 2 : y_vel += y * 2;

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
    // center Dot on an object
    while (col_rect.x < end_rect->x )
        col_rect.x++;
    while (col_rect.x > end_rect->x )
        col_rect.x--;
    while (col_rect.y < end_rect->y )
        col_rect.y++;
    while (col_rect.y > end_rect->y )
        col_rect.y--;

    return true;
}

// return y_vel
float Dot::get_y_vel()
{
    return y_vel;
}