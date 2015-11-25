
#ifndef textures_hpp
#define textures_hpp

// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

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
    bool load_from_file(std::string path, SDL_Renderer* rend);
    
    // load a tile
    bool load_tile_sheet(std::string path, SDL_Renderer* rend);
    
    // render the texture at the given point
    void render(int x, int y, SDL_Rect* clip, SDL_Rect* camera, SDL_Renderer* rend);
    
    // render a tile at the given point
    void render_tile(int x, int y, SDL_Rect* type_clip, SDL_Rect* active_clip, SDL_Rect* camera, SDL_Renderer* rend);
    
private:
    
    // the texture itself
    SDL_Texture* tex = NULL;
    
    // dimensions
    int width, height;
};

#endif /* textures_hpp */
