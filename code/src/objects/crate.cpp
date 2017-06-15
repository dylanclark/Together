
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

// include headers
#include <objects.hpp>
#include <levels.hpp>
#include <char.hpp>

crate::crate(int x, int y, int type)
{
    // init position
    col_rect.x = x;
    col_rect.y = y;

    // init type
    crate_type = type;

    // init pushed flag
    pushed = false;

    // initialize tile generation if that's chill
    generating = true;

    // determine width and height based on crate type
    switch (type)
    {
        case FOUR_BY_TWO:
            col_rect.w = 4 * TILE_WIDTH;
            col_rect.h = 2 * TILE_WIDTH;
            break;
        case FOUR_BY_ONE:
            col_rect.w = 4 * TILE_WIDTH;
            col_rect.h = 1 * TILE_WIDTH;
            break;
        case TWO_BY_TWO:
            col_rect.w = 2 * TILE_WIDTH;
            col_rect.h = 2 * TILE_WIDTH;
            break;
        case THREE_BY_TWO:
            col_rect.w = 3 * TILE_WIDTH;
            col_rect.h = 2 * TILE_WIDTH;
            break;
    }
}

void crate::update()
{
    // only update if the crate isn't being pushed
    if (!pushed)
    {
        // determine grid-aligned destination
        int target_x = floor(((float) col_rect.x / TILE_WIDTH) + 0.5) * TILE_WIDTH;

        // set velocity so that it'll close the gap
        x_vel = sqrt(abs((target_x - col_rect.x))) * ((target_x > col_rect.x) ? 1 : -1);

        // if the gap is small enough, just put the crate there
        if (x_vel <= 0.1 && x_vel >= -0.1)
        {
            x_vel = 0;
            col_rect.x = floor(((float) col_rect.x / TILE_WIDTH) + 0.5) * TILE_WIDTH;
        }
    }

    // update the position
    col_rect.x += x_vel;
}

void crate::render(int b_status, SDL_Rect* camera, engine* game, levelstate* level)
{
    // determine which chunk of the texture to render
    SDL_Rect active_clip = {0, 0, 64, 32};
    SDL_Rect inactive_clip = {64 * 9, 0, 64, 32};

    // render based on char status
    if ((b_status == CHAR_ACTIVE && black) || (b_status == CHAR_INACTIVE && !black))
    {
        tex.render(col_rect.x, col_rect.y, &active_clip, camera, game);
        if (!generating)
            generating = true;
    }
    else
    {
        tex.render(col_rect.x, col_rect.y, &inactive_clip, camera, game);

        if (generating)
        {
            create_tiles(b_status, level);
            generating = false;
        }

        for (int i = 0; i < MAX_BORDER; i++)
        {
            tileset[i]->render(b_status, camera, game, &level->tile_tex);
        }
    }
}

bool crate::check_col(SDL_Rect crate, levelstate* level, vector *repos)
{
    for (int i = 0; i < level->width * level->height; i++)
    {
        bool iswall_b = (level->tileset[i]->wall_b && !level->tileset[i]->floor_b && !level->tileset[i]->ceiling_b);
        bool iswall_w = (level->tileset[i]->wall_w && !level->tileset[i]->floor_w && !level->tileset[i]->ceiling_w);

        if (iswall_b && black)
        {
            check_collision(col_rect, level->tileset[i]->get_col_rect(), repos);
            col_rect.x += repos->x;
            return true;
        }
        else if (iswall_w && !black)
        {
            check_collision(col_rect, level->tileset[i]->get_col_rect(), repos);
            col_rect.x += repos->x;
            return true;
        }
    }

    return false;
}

void crate::create_tiles(int b_status, levelstate* level)
{
    vector repos;
    int counter = 0;

    // top of crate
    for (int i = 0; i < (col_rect.w / TILE_WIDTH); i++)
    {
        SDL_Rect current_tile = {col_rect.x + i * TILE_WIDTH + TILE_WIDTH / 2, col_rect.y - TILE_WIDTH / 2, 1, 1};
        for (int j = 0; j < level->width * level->height; j++)
        {
            if (check_collision(current_tile, level->tileset[j]->get_col_rect(), &repos))
            {
                int new_type = tile_type_top(level->tileset[j]->get_type());

                tileset[counter] = new tile(level->tileset[j]->get_col_rect().x, level->tileset[j]->get_col_rect().y, new_type);
                tileset[counter]->status = (tileset[counter]->get_type() < 15) ? (b_status) % 4 : (b_status + 2) % 4;
                tileset[counter]->frame = (tileset[counter]->status == TILE_ACTIVE || tileset[counter]->status == TILE_INACTIVATE) ? 0 : TILE_FRAMES - 1;
                counter++;
                break;
            }
        }
    }
    // bottom of crate
    for (int i = 0; i < (col_rect.w / TILE_WIDTH); i++)
    {
        SDL_Rect current_tile = {col_rect.x + i * TILE_WIDTH + TILE_WIDTH / 2, col_rect.y + col_rect.h + TILE_WIDTH / 2, 1, 1};
        for (int j = 0; j < level->width * level->height; j++)
        {
            if (check_collision(current_tile, level->tileset[j]->get_col_rect(), &repos))
            {
                int new_type = tile_type_bottom(level->tileset[j]->get_type());

                tileset[counter] = new tile(level->tileset[j]->get_col_rect().x, level->tileset[j]->get_col_rect().y, new_type);
                tileset[counter]->status = (tileset[counter]->get_type() < 15) ? (b_status) % 4 : (b_status + 2) % 4;
                tileset[counter]->frame = (tileset[counter]->status == TILE_ACTIVE || tileset[counter]->status == TILE_INACTIVATE) ? 0 : TILE_FRAMES - 1;
                counter++;
                break;
            }
        }
    }
    // left of crate
    for (int i = 0; i < (col_rect.h / TILE_WIDTH); i++)
    {
        SDL_Rect current_tile = {col_rect.x - TILE_WIDTH / 2, col_rect.y + i * TILE_WIDTH + TILE_WIDTH / 2, 1, 1};
        for (int j = 0; j < level->width * level->height; j++)
        {
            if (check_collision(current_tile, level->tileset[j]->get_col_rect(), &repos))
            {
                int new_type = tile_type_left(level->tileset[j]->get_type());

                tileset[counter] = new tile(level->tileset[j]->get_col_rect().x, level->tileset[j]->get_col_rect().y, new_type);
                tileset[counter]->status = (tileset[counter]->get_type() < 15) ? (b_status) % 4 : (b_status + 2) % 4;
                tileset[counter]->frame = (tileset[counter]->status == TILE_ACTIVE || tileset[counter]->status == TILE_INACTIVATE) ? 0 : TILE_FRAMES - 1;
                counter++;
                break;
            }
        }
    }
    // right of crate
    for (int i = 0; i < (col_rect.h / TILE_WIDTH); i++)
    {
        SDL_Rect current_tile = {col_rect.x + col_rect.w + TILE_WIDTH / 2, col_rect.y + i * TILE_WIDTH + TILE_WIDTH / 2, 1, 1};
        for (int j = 0; j < level->width * level->height; j++)
        {
            if (check_collision(current_tile, level->tileset[j]->get_col_rect(), &repos))
            {
                int new_type = tile_type_right(level->tileset[j]->get_type());

                tileset[counter] = new tile(level->tileset[j]->get_col_rect().x, level->tileset[j]->get_col_rect().y, new_type);
                tileset[counter]->status = (tileset[counter]->get_type() < 15) ? (b_status) % 4 : (b_status + 2) % 4;
                tileset[counter]->frame = (tileset[counter]->status == TILE_ACTIVE || tileset[counter]->status == TILE_INACTIVATE) ? 0 : TILE_FRAMES - 1;
                counter++;
                break;
            }
        }
    }
}

int crate::tile_type_top(int type)
{
    int new_type = black ? W_BACK : B_BACK;

    switch (type)
    {
        case B_BACK:
            new_type = black ? B_BACK : B_CEILING;
            break;
        case B_FLOOR1:
        case B_FLOOR2:
        case B_FLOOR3:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_FLOOREDGE_L:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_FLOOREDGE_R:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_CEILING:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_CEILINGEDGE_L:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_CEILINGEDGE_R:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_WALL_L:
            new_type = black ? B_BACK : B_CEILINGEDGE_L;
            break;
        case B_WALL_R:
            new_type = black ? B_BACK : B_CEILINGEDGE_R;
            break;
        case B_CORNER_BL:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_CORNER_BR:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_CORNER_TL:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_CORNER_TR:
            new_type = black ? B_BACK : B_BACK;
            break;
        case W_BACK:
            new_type = black ? W_FLOOR1 : W_BACK;
            break;
        case W_FLOOR1:
        case W_FLOOR2:
        case W_FLOOR3:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_FLOOREDGE_L:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_FLOOREDGE_R:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CEILING:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CEILINGEDGE_L:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CEILINGEDGE_R:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_WALL_L:
            new_type = black ? W_FLOOREDGE_L : W_BACK;
            break;
        case W_WALL_R:
            new_type = black ? W_FLOOREDGE_R : W_BACK;
            break;
        case W_CORNER_BL:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CORNER_BR:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CORNER_TL:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CORNER_TR:
            new_type = black ? W_BACK : W_BACK;
            break;
        default:
            new_type = black ? W_BACK : B_BACK;
            break;
    }

    return new_type;
}

int crate::tile_type_bottom(int type)
{
    int new_type = black ? W_BACK : B_BACK;

    switch (type)
    {
        case B_BACK:
            new_type = black ? B_BACK : B_FLOOR1;
            break;
        case B_FLOOR1:
        case B_FLOOR2:
        case B_FLOOR3:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_FLOOREDGE_L:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_FLOOREDGE_R:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_CEILING:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_CEILINGEDGE_L:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_CEILINGEDGE_R:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_WALL_L:
            new_type = black ? B_BACK : B_FLOOREDGE_L;
            break;
        case B_WALL_R:
            new_type = black ? B_BACK : B_FLOOREDGE_R;
            break;
        case B_CORNER_BL:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_CORNER_BR:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_CORNER_TL:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_CORNER_TR:
            new_type = black ? B_BACK : B_BACK;
            break;
        case W_BACK:
            new_type = black ? W_CEILING : W_BACK;
            break;
        case W_FLOOR1:
        case W_FLOOR2:
        case W_FLOOR3:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_FLOOREDGE_L:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_FLOOREDGE_R:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CEILING:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CEILINGEDGE_L:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CEILINGEDGE_R:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_WALL_L:
            new_type = black ? W_CEILINGEDGE_L : W_BACK;
            break;
        case W_WALL_R:
            new_type = black ? W_CEILINGEDGE_R : W_BACK;
            break;
        case W_CORNER_BL:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CORNER_BR:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CORNER_TL:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CORNER_TR:
            new_type = black ? W_BACK : W_BACK;
            break;
        default:
            new_type = black ? W_BACK : B_BACK;
            break;
    }

    return new_type;
}

// left side tile determinant
int crate::tile_type_left(int type)
{
    int new_type = black ? W_BACK : B_BACK;

    switch (type)
    {
        case B_BACK:
            new_type = black ? B_BACK : B_WALL_R;
            break;
        case B_FLOOR1:
        case B_FLOOR2:
        case B_FLOOR3:
            new_type = black ? B_BACK : B_FLOOREDGE_R;
            break;
        case B_FLOOREDGE_L:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_FLOOREDGE_R:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_CEILING:
            new_type = black ? B_BACK : B_CEILINGEDGE_R;
            break;
        case B_CEILINGEDGE_L:
            new_type = black ? B_BACK : B_CEILINGEDGE_R;
            break;
        case B_CEILINGEDGE_R:
            new_type = black ? B_BACK : B_CEILINGEDGE_R;
            break;
        case B_WALL_L:
            new_type = black ? B_BACK : B_WALL_R;
            break;
        case B_WALL_R:
            new_type = black ? B_BACK : B_BACK;
            break;
        case B_CORNER_BL:
            new_type = black ? B_BACK : B_WALL_R;
            break;
        case B_CORNER_BR:
            new_type = black ? B_BACK : B_WALL_R;
            break;
        case B_CORNER_TL:
            new_type = black ? B_BACK : B_WALL_R;
            break;
        case B_CORNER_TR:
            new_type = black ? B_BACK : B_WALL_R;
            break;
        case W_BACK:
            new_type = black ? W_WALL_R : W_BACK;
            break;
        case W_FLOOR1:
        case W_FLOOR2:
        case W_FLOOR3:
            new_type = black ? W_FLOOREDGE_R : W_BACK;
            break;
        case W_FLOOREDGE_L:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_FLOOREDGE_R:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CEILING:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CEILINGEDGE_L:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CEILINGEDGE_R:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_WALL_L:
            new_type = black ? W_CEILINGEDGE_L : W_BACK;
            break;
        case W_WALL_R:
            new_type = black ? W_CEILINGEDGE_R : W_BACK;
            break;
        case W_CORNER_BL:
            new_type = black ? W_WALL_R : W_BACK;
            break;
        case W_CORNER_BR:
            new_type = black ? W_WALL_R : W_BACK;
            break;
        case W_CORNER_TL:
            new_type = black ? W_WALL_R : W_BACK;
            break;
        case W_CORNER_TR:
            new_type = black ? W_WALL_R : W_BACK;
            break;
        default:
            new_type = black ? W_BACK : B_BACK;
            break;
    }

    return new_type;
}

// right side tile determinant
int crate::tile_type_right(int type)
{
    int new_type = black ? W_BACK : B_BACK;

    switch (type)
    {
        case B_BACK:
            new_type = black ? B_BACK : B_WALL_L;
            break;
        case B_FLOOR1:
        case B_FLOOR2:
        case B_FLOOR3:
            new_type = black ? B_BACK : B_FLOOREDGE_L;
            break;
        case B_FLOOREDGE_L:
            new_type = black ? B_BACK : B_CEILINGEDGE_L;
            break;
        case B_FLOOREDGE_R:
            new_type = black ? B_BACK : B_CEILINGEDGE_L;
            break;
        case B_CEILING:
            new_type = black ? B_BACK : B_CEILINGEDGE_L;
            break;
        case B_CEILINGEDGE_L:
            new_type = black ? B_BACK : B_WALL_L;
            break;
        case B_CEILINGEDGE_R:
            new_type = black ? B_BACK : B_WALL_L;
            break;
        case B_WALL_L:
            new_type = black ? B_BACK : B_WALL_L;
            break;
        case B_WALL_R:
            new_type = black ? B_BACK : B_WALL_L;
            break;
        case B_CORNER_BL:
            new_type = black ? B_BACK : B_WALL_L;
            break;
        case B_CORNER_BR:
            new_type = black ? B_BACK : B_WALL_L;
            break;
        case B_CORNER_TL:
            new_type = black ? B_BACK : B_WALL_L;
            break;
        case B_CORNER_TR:
            new_type = black ? B_BACK : B_WALL_L;
            break;
        case W_BACK:
            new_type = black ? W_WALL_L : W_BACK;
            break;
        case W_FLOOR1:
        case W_FLOOR2:
        case W_FLOOR3:
            new_type = black ? W_FLOOREDGE_L : W_BACK;
            break;
        case W_FLOOREDGE_L:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_FLOOREDGE_R:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CEILING:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CEILINGEDGE_L:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_CEILINGEDGE_R:
            new_type = black ? W_BACK : W_BACK;
            break;
        case W_WALL_L:
            new_type = black ? W_FLOOREDGE_L : W_BACK;
            break;
        case W_WALL_R:
            new_type = black ? W_FLOOREDGE_R : W_BACK;
            break;
        case W_CORNER_BL:
            new_type = black ? W_WALL_L : W_BACK;
            break;
        case W_CORNER_BR:
            new_type = black ? W_WALL_L : W_BACK;
            break;
        case W_CORNER_TL:
            new_type = black ? W_WALL_L : W_BACK;
            break;
        case W_CORNER_TR:
            new_type = black ? W_WALL_L : W_BACK;
            break;
        default:
            new_type = black ? W_BACK : B_BACK;
            break;
    }

    return new_type;
}