
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

// include headers
#include <objects.hpp>
#include <levels.hpp>
#include <char.hpp>
#include <tiles.hpp>

Crate::Crate(int x, int y, int type, bool color, SDL_Renderer* rend)
{
    col_rect.x = x * TILE_WIDTH;
    col_rect.y = y * TILE_WIDTH;
    crate_type = type;
    my_color = color;
    pushed = false;
    generating = true;

    // determine width and height based on crate type
    switch (type) {
        case FOUR_BY_TWO:
            col_rect.w = 4 * TILE_WIDTH;
            col_rect.h = 2 * TILE_WIDTH;
            if (my_color == 0) {
                if (!tex.load_object(TILE_WIDTH * 4, TILE_WIDTH * 2, "black/crates/b_crate.png", rend)) {
                    printf("Failed to load black crate (4x2) texture!\n");
                    return;
                }
            }
            else {
                if (!tex.load_object(TILE_WIDTH * 4, TILE_WIDTH * 2, "white/crates/w_crate.png", rend)) {
                    printf("Failed to load white crate (4x2) texture!\n");
                    return;
                }
            }
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

void Crate::update()
{
    // only update if the crate isn't being pushed
    if (!pushed)
    {
        // determine grid-aligned destination
        int target_x = floor(((float) col_rect.x / TILE_WIDTH) + 0.5) * TILE_WIDTH;

        // set velocity so that it'll close the gap
        x_vel = sqrt(abs((target_x - col_rect.x))) * ((target_x > col_rect.x) ? 1 : -1);

        // if the gap is small enough, just put the crate there
        if (x_vel <= 0.1 && x_vel >= -0.1) {
            x_vel = 0;
            col_rect.x = floor(((float) col_rect.x / TILE_WIDTH) + 0.5) * TILE_WIDTH;
        }
    }

    // update the position
    col_rect.x += x_vel;
}

void Crate::render(int status, SDL_Rect* camera, Engine* game, Levelstate* level)
{
    // determine which chunk of the texture to render
    SDL_Rect active_clip = {0, 0, 64, 32};
    SDL_Rect inactive_clip = {64 * 9, 0, 64, 32};

    // render based on char status
    if (my_color == 0) {
        tex.render(col_rect.x, col_rect.y, &active_clip, camera, game);
        if (!generating)
            generating = true;
    }
    else {
        tex.render(col_rect.x, col_rect.y, &inactive_clip, camera, game);

        if (generating) {
            generating = false;
        }

        for (int i = 0; i < MAX_BORDER; i++) {
            tileset[i]->render_bg(status, camera, game, &level->tile_tex);
        }
    }
}

bool Crate::check_col(SDL_Rect crate, Levelstate* level, Vector* repos)
{
    for (int i = 0; i < level->width * level->height; i++)
    {
        bool is_wall = (level->tileset[i]->wall && !level->tileset[i]->floor && !level->tileset[i]->ceiling);

        if (is_wall && my_color == 0)
        {
            check_collision(col_rect, level->tileset[i]->get_col_rect(), repos);
            col_rect.x += repos->x;
            return true;
        }
        else if (is_wall && my_color == 1)
        {
            check_collision(col_rect, level->tileset[i]->get_col_rect(), repos);
            col_rect.x += repos->x;
            return true;
        }
    }

    return false;
}
