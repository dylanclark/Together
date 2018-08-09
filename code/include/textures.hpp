
#ifndef textures_hpp
#define textures_hpp

// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int START = 0;
const int END = 1;

class Engine;

class Texture
{
public:
    Texture();
    ~Texture();
    void free();

    bool load_object(int w, int h, std::string path, SDL_Color* palette = NULL);
    bool load_tile_sheet(std::string path, SDL_Color* palette = NULL);
    void create_square(bool color, int w, int h, SDL_Color* palette = NULL);

    void render(int x, int y, SDL_Rect* clip, SDL_Rect* camera, int dir = 0, int flip = 0, float angle = 0.0);
    void render_tile(int x, int y, SDL_Rect* active_clip, SDL_Rect* camera, int alpha = SDL_ALPHA_OPAQUE);
    void render_button(SDL_Rect* button, SDL_Rect* clip);
    void angle_render(int x, int y, SDL_Rect *clip, SDL_Rect *camera, double angle, SDL_Point *center, SDL_RendererFlip flip);

    void set_blend_mode(SDL_BlendMode blending);
    void set_alpha (Uint8 alpha);

    int get_width() {return width;}
    int get_height() {return height;}

    bool load_message(int w, int h, std::string path);
    void render_message(SDL_Rect* message, SDL_Rect* clip);

private:
    SDL_Texture* tex;
    int width, height;
};

#endif /* textures_hpp */
