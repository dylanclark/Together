
// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header file
#include <textures.hpp>
#include <levels.hpp>
#include <engine.hpp>

// initialize
Texture::Texture()
{
    tex = NULL;
    width = 0;
    height = 0;
};

// deallocate
Texture::~Texture()
{
    free();
};

bool Texture::load_object(int w, int h, std::string path, SDL_Renderer* rend, SDL_Color* palette)
{
    // ditch the last texture
    free();

    // new texture!
    tex = NULL;

    SDL_Surface* surface = IMG_Load(("resources/textures/"+path).c_str());
    if (surface == NULL)
    {
        printf("Unable to load image %s! SDL error: %s\n",
               ("resources/textures/"+path).c_str(), SDL_GetError());
        return false;
    }
    tex = SDL_CreateTextureFromSurface(rend, surface);
    if (tex == NULL) {
        printf("Unable to create texture from image! SDL error: %s\n", SDL_GetError());
        return false;
    }
    if (palette != NULL) {
        SDL_SetTextureColorMod(tex, palette->r, palette->g, palette->b);
    }
    // get image dimensions
    width = w;
    height = h;

    SDL_FreeSurface(surface);

    return true;
};

bool Texture::load_tile_sheet(std::string path, SDL_Renderer* rend, SDL_Color* palette)
{
    // ditch the last texture
    free();

    // new texture!
    tex = NULL;

    SDL_Surface* surface = IMG_Load(("resources/textures/"+path).c_str());
    if (surface == NULL) {
        printf("Unable to load image %s! SDL error: %s\n",
              ("resources/textures/"+path).c_str(), SDL_GetError());
        return false;
    }
    tex = SDL_CreateTextureFromSurface(rend, surface);
    if (tex == NULL) {
        printf("Unable to create texture from image! SDL error: %s\n", SDL_GetError());
        return false;
    }
    if (palette != NULL) {
        SDL_SetTextureColorMod(tex, palette->r, palette->g, palette->b);
    }
    // tile dimensions
    width = TILE_WIDTH;
    height = TILE_WIDTH;
    SDL_FreeSurface(surface);

    return true;
};

void Texture::set_blend_mode(SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(tex, blending);
}

void Texture::set_alpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(tex, alpha);
}

void Texture::free()
{
    // deallocate the texture if it exists
    if (tex != NULL)
    {
        SDL_DestroyTexture(tex);
        tex = NULL;
        width = 0;
        height = 0;
    }
}

void Texture::render(int x, int y, SDL_Rect* clip, SDL_Rect* camera, Engine* game, int dir)
{
    int render_x = (x - camera->x) / ( (float) camera->w / (float) game->screen_width);
    int render_y = (y - camera->y) / ((float) camera->h / (float) game->screen_height);
    int render_w = width * ((float) game->screen_width / (float) camera->w) + 1;
    int render_h = height * ((float) game->screen_height / (float) camera->h) + 1;

    // rendering rectangle
    SDL_Rect render_rect = {render_x, render_y, render_w, render_h};

    // render to the screen
    if (dir == 0) {
        SDL_RenderCopy(game->rend, tex, clip, &render_rect);
    } else {
        SDL_RenderCopyEx(game->rend, tex, clip, &render_rect, 0.0, NULL, SDL_FLIP_HORIZONTAL);
    }
}

void Texture::render_tile(int x, int y, SDL_Rect* type_clip, SDL_Rect* active_clip, SDL_Rect* camera, Engine* game)
{
    int render_x = (x - camera->x) / ((float) camera->w / (float) game->screen_width);
    int render_y = (y - camera->y) / ((float) camera->h / (float) game->screen_height);
    int render_w = TILE_WIDTH * ((float) game->screen_width / (float) camera->w) + 1;
    int render_h = TILE_WIDTH * ((float) game->screen_height / (float) camera->h) + 1;

    SDL_Rect render_rect = {render_x, render_y, render_w, render_h};
    SDL_Rect clip_rect = {type_clip->x + active_clip->x,
                          type_clip->y + active_clip->y,
                          TILE_WIDTH_TEX,
                          TILE_WIDTH_TEX};

    SDL_RenderCopy(game->rend, tex, &clip_rect, &render_rect);
}

void Texture::render_button(SDL_Rect* button, SDL_Rect* clip, SDL_Renderer* rend)
{
    // rendering rectangle
    SDL_Rect render_rect = {button->x - button->w / 2, button->y - button->h / 2, button->w, button->h};

    // render to the screen
    SDL_RenderCopy(rend, tex, clip, &render_rect);
}

void Texture::angle_render(int x, int y, SDL_Rect *clip, SDL_Rect *camera, Engine* game, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    int render_x = (x - camera->x) / ( (float) camera->w / (float) game->screen_width);
    int render_y = (y - camera->y) / ((float) camera->h / (float) game->screen_height);
    int render_w = width / ((float) camera->w / (float) game->screen_width) + 1;
    int render_h = height / ((float) camera->h / (float) game->screen_height) + 1;

    // rendering rectangle
    SDL_Rect render_rect = {render_x, render_y, render_w, render_h};

    // render to the screen
    SDL_RenderCopyEx(game->rend, tex, clip, &render_rect, angle, center, flip);
};

bool Texture::load_message(int w, int h, std::string path, SDL_Renderer* rend)
{
        // ditch the last texture
        free();

        // new texture!
        tex = NULL;

        SDL_Surface* surface = IMG_Load(path.c_str());
        if (surface == NULL)
        {
            printf("Unable to load image %s! SDL error: %s\n", path.c_str(), SDL_GetError());
            return false;
        }
        else
        {
            tex = SDL_CreateTextureFromSurface(rend, surface);
            if (tex == NULL)
            {
                printf("Unable to create texture from image! SDL error: %s\n", SDL_GetError());
                return false;
            }
            else
            {
                // tile dimensions
                width = w;
                height = h;
            }

            SDL_FreeSurface(surface);
        }

        return true;
};

void Texture::render_message(SDL_Rect* message, SDL_Rect* clip, SDL_Renderer* rend)
{
    // rendering rectangle
    SDL_Rect render_rect = {message->x, message->y, message->w, message->h};

    // render to the screen
    SDL_RenderCopy(rend, tex, NULL, &render_rect);
}
