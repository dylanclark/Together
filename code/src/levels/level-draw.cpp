
// using SDL and standard IO
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header
#include <levels.hpp>

bool load_level_from_file(Engine* game, Levelstate* lvl, std::string filename)
{
    // success flag
    bool success = true;

    // tile offsets
    int x = 0, y = 0;

    std::string path = "resources/level-files/"+filename;
    std::ifstream level_file(path.c_str());

    if (!level_file.is_open()) {
        printf("Could not find level file!\n");
        return false;
    }

    int r, g, b;
    level_file >> r;
    level_file >> g;
    level_file >> b;
    lvl->palette.r = r;
    lvl->palette.g = g;
    lvl->palette.b = b;

    int level_w;
    int level_h;
    level_file >> level_w;
    level_file >> level_h;
    lvl->width = level_w;
    lvl->height = level_h;

    for (int i = 0; i < level_w * level_h; i++)
    {
        int type = -1;

        level_file >> type;

        if (level_file.fail())
        {
            printf("Error loading level_file: unexpected end of file!\n");
            success = false;
            break;
        }

        if ((type >= 0) && (type < TOTAL_TILES))
        {
            lvl->tileset[i] = new Tile(x, y, type);
        }
        else
        {
            printf("Error loading level_file: unknown tile type!\n");
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
    }

    int num_chars;
    level_file >> num_chars;
    int char_x, char_y, lvl_end_x, lvl_end_y;
    for (int i = 0; i < num_chars; i++) {
        level_file >> char_x;
        level_file >> char_y;
        level_file >> lvl_end_x;
        level_file >> lvl_end_y;
        lvl->chars.push_back(new Dot(char_x, char_y, i, game->rend, &lvl->palette));
        lvl->level_ends.push_back(new LevelEnd(lvl_end_x, lvl_end_y, i, game->rend, &lvl->palette));
    }

    // close the file
    level_file.close();

    // return whether tiles were loaded
    return success;
}

