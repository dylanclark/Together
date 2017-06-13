
#ifndef textures_hpp
#define textures_hpp

// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include headers
#include <engine/engine.hpp>

const int START = 0;
const int END = 1;

class texture
{
public:
    // initialization
    texture();

    // deallocation
    ~texture();

    // free the texture
    void free();

    // load the texture at the specified path
    bool load_object(int w, int h, std::string path, SDL_Renderer* rend);

    // load a tile
    bool load_tile_sheet(std::string path, SDL_Renderer* rend);

    // various render functions
    void render(int x, int y, SDL_Rect* clip, SDL_Rect* camera, engine* game);
    void render_tile(int x, int y, SDL_Rect* type_clip, SDL_Rect* active_clip, SDL_Rect* camera, engine* game);
    void render_button(SDL_Rect* button, SDL_Rect* clip, SDL_Renderer* rend);
    void angle_render(int x, int y, SDL_Rect *clip, SDL_Rect *camera, engine* game, double angle, SDL_Point *center, SDL_RendererFlip flip);


    // set blending
    void set_blend_mode(SDL_BlendMode blending);

    // set alpha
    void set_alpha (Uint8 alpha);


    // return width and height
    int get_width() {return width;}
    int get_height() {return height;}

    // load level messages
    bool load_message(int w, int h, std::string path, SDL_Renderer* rend);
    void render_message(SDL_Rect* message, SDL_Rect* clip, SDL_Renderer* rend);
private:

    // the texture itself
    SDL_Texture* tex = NULL;

    // dimensions
    int width, height;
};

#endif /* textures_hpp */
