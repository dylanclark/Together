
// using SDL and standard IO
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header
#include <levels.hpp>

// tile clipper
SDL_Rect tile_clips[TOTAL_TILES];

bool load_level_from_file(Engine* game, Levelstate* lvl, std::string filename)
{
    // success flag
    bool success = true;

    // tile offsets
    int x = 0, y = 0;

    std::string path = "resources/level-files/"+filename;

    std::ifstream map(path.c_str());

    if (!map.is_open()) {
        printf("Could not find level file!\n");
        return false;
    }

    int r, g, b;
    map >> r;
    map >> g;
    map >> b;
    lvl->palette.r = r;
    lvl->palette.g = g;
    lvl->palette.b = b;

    int level_w;
    int level_h;
    map >> level_w;
    map >> level_h;
    lvl->width = level_w;
    lvl->height = level_h;

    for (int i = 0; i < level_w * level_h; i++)
    {
        int type = -1;

        map >> type;

        if (map.fail())
        {
            printf("Error loading map: unexpected end of file!\n");
            success = false;
            break;
        }

        if ((type >= 0) && (type < TOTAL_TILES))
        {
            lvl->tileset[i] = new Tile(x, y, type);
        }
        else
        {
            printf("Error loading map: unknown tile type!\n");
            success = false;
            break;
        }

        // iterate horizontally
        x += TILE_WIDTH;

        if (x >= level_w * TILE_WIDTH)
        {
            x = 0;
            y += TILE_WIDTH;
        }

        if (success)
        {
            tile_clips[B_BACK].x = B_BACK * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_BACK].y = 0;
            tile_clips[B_BACK].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_BACK].h = TILE_WIDTH_TEX;

            tile_clips[B_FLOOR1].x = B_FLOOR1 * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_FLOOR1].y = 0;
            tile_clips[B_FLOOR1].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_FLOOR1].h = TILE_WIDTH_TEX;

            tile_clips[B_FLOOR2].x = B_FLOOR2 * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_FLOOR2].y = 0;
            tile_clips[B_FLOOR2].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_FLOOR2].h = TILE_WIDTH_TEX;

            tile_clips[B_FLOOR3].x = B_FLOOR3 * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_FLOOR3].y = 0;
            tile_clips[B_FLOOR3].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_FLOOR3].h = TILE_WIDTH_TEX;

            tile_clips[B_FLOOREDGE_R].x = B_FLOOREDGE_R * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_FLOOREDGE_R].y = 0;
            tile_clips[B_FLOOREDGE_R].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_FLOOREDGE_R].h = TILE_WIDTH_TEX;

            tile_clips[B_FLOOREDGE_L].x = B_FLOOREDGE_L * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_FLOOREDGE_L].y = 0;
            tile_clips[B_FLOOREDGE_L].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_FLOOREDGE_L].h = TILE_WIDTH_TEX;

            tile_clips[B_CEILING].x = B_CEILING * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_CEILING].y = 0;
            tile_clips[B_CEILING].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_CEILING].h = TILE_WIDTH_TEX;

            tile_clips[B_CEILINGEDGE_R].x = B_CEILINGEDGE_R * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_CEILINGEDGE_R].y = 0;
            tile_clips[B_CEILINGEDGE_R].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_CEILINGEDGE_R].h = TILE_WIDTH_TEX;

            tile_clips[B_CEILINGEDGE_L].x = B_CEILINGEDGE_L * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_CEILINGEDGE_L].y = 0;
            tile_clips[B_CEILINGEDGE_L].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_CEILINGEDGE_L].h = TILE_WIDTH_TEX;

            tile_clips[B_WALL_R].x = B_WALL_R * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_WALL_R].y = 0;
            tile_clips[B_WALL_R].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_WALL_R].h = TILE_WIDTH_TEX;

            tile_clips[B_WALL_L].x = B_WALL_L * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_WALL_L].y = 0;
            tile_clips[B_WALL_L].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_WALL_L].h = TILE_WIDTH_TEX;

            tile_clips[B_CORNER_BL].x = B_CORNER_BL * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_CORNER_BL].y = 0;
            tile_clips[B_CORNER_BL].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_CORNER_BL].h = TILE_WIDTH_TEX;

            tile_clips[B_CORNER_BR].x = B_CORNER_BR * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_CORNER_BR].y = 0;
            tile_clips[B_CORNER_BR].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_CORNER_BR].h = TILE_WIDTH_TEX;

            tile_clips[B_CORNER_TL].x = B_CORNER_TL * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_CORNER_TL].y = 0;
            tile_clips[B_CORNER_TL].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_CORNER_TL].h = TILE_WIDTH_TEX;

            tile_clips[B_CORNER_TR].x = B_CORNER_TR * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_CORNER_TR].y = 0;
            tile_clips[B_CORNER_TR].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[B_CORNER_TR].h = TILE_WIDTH_TEX;

            tile_clips[W_BACK].x = W_BACK * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_BACK].y = 0;
            tile_clips[W_BACK].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_BACK].h = TILE_WIDTH_TEX;

            tile_clips[W_FLOOR1].x = W_FLOOR1 * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_FLOOR1].y = 0;
            tile_clips[W_FLOOR1].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_FLOOR1].h = TILE_WIDTH_TEX;

            tile_clips[W_FLOOR2].x = W_FLOOR2 * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_FLOOR2].y = 0;
            tile_clips[W_FLOOR2].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_FLOOR2].h = TILE_WIDTH_TEX;

            tile_clips[W_FLOOR3].x = W_FLOOR3 * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_FLOOR3].y = 0;
            tile_clips[W_FLOOR3].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_FLOOR3].h = TILE_WIDTH_TEX;

            tile_clips[W_FLOOREDGE_R].x = W_FLOOREDGE_R * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_FLOOREDGE_R].y = 0;
            tile_clips[W_FLOOREDGE_R].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_FLOOREDGE_R].h = TILE_WIDTH_TEX;

            tile_clips[W_FLOOREDGE_L].x = W_FLOOREDGE_L * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_FLOOREDGE_L].y = 0;
            tile_clips[W_FLOOREDGE_L].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_FLOOREDGE_L].h = TILE_WIDTH_TEX;

            tile_clips[W_CEILING].x = W_CEILING * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_CEILING].y = 0;
            tile_clips[W_CEILING].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_CEILING].h = TILE_WIDTH_TEX;

            tile_clips[W_CEILINGEDGE_R].x = W_CEILINGEDGE_R * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_CEILINGEDGE_R].y = 0;
            tile_clips[W_CEILINGEDGE_R].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_CEILINGEDGE_R].h = TILE_WIDTH_TEX;

            tile_clips[W_CEILINGEDGE_L].x = W_CEILINGEDGE_L * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_CEILINGEDGE_L].y = 0;
            tile_clips[W_CEILINGEDGE_L].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_CEILINGEDGE_L].h = TILE_WIDTH_TEX;

            tile_clips[W_WALL_R].x = W_WALL_R * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_WALL_R].y = 0;
            tile_clips[W_WALL_R].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_WALL_R].h = TILE_WIDTH_TEX;

            tile_clips[W_WALL_L].x = W_WALL_L * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_WALL_L].y = 0;
            tile_clips[W_WALL_L].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_WALL_L].h = TILE_WIDTH_TEX;

            tile_clips[W_CORNER_BL].x = W_CORNER_BL * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_CORNER_BL].y = 0;
            tile_clips[W_CORNER_BL].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_CORNER_BL].h = TILE_WIDTH_TEX;

            tile_clips[W_CORNER_BR].x = W_CORNER_BR * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_CORNER_BR].y = 0;
            tile_clips[W_CORNER_BR].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_CORNER_BR].h = TILE_WIDTH_TEX;

            tile_clips[W_CORNER_TL].x = W_CORNER_TL * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_CORNER_TL].y = 0;
            tile_clips[W_CORNER_TL].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_CORNER_TL].h = TILE_WIDTH_TEX;

            tile_clips[W_CORNER_TR].x = W_CORNER_TR * TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_CORNER_TR].y = 0;
            tile_clips[W_CORNER_TR].w = TILE_WIDTH_TEX * TILE_FRAMES;
            tile_clips[W_CORNER_TR].h = TILE_WIDTH_TEX;
        }
    }

    int num_chars;
    map >> num_chars;
    int char_x, char_y, lvl_end_x, lvl_end_y;
    for (int i = 0; i < num_chars; i++) {
        map >> char_x;
        map >> char_y;
        map >> lvl_end_x;
        map >> lvl_end_y;
        lvl->chars.push_back(new Dot(char_x, char_y, i, game->rend, &lvl->palette));
        lvl->level_ends.push_back(new LevelEnd(lvl_end_x, lvl_end_y, i, game->rend));
    }

    // close the file
    map.close();
    printf("level file closed\n");

    // return whether tiles were loaded
    return success;
}

