
#ifndef textures_hpp
#define textures_hpp

// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Texture
{
public:
    Texture();
    ~Texture();

    bool load_object(int w, int h, std::string path);

    void render(int x, int y, SDL_Rect* clip, SDL_Rect* camera);

    int get_width() { return width; }
    int get_height() { return height; }

private:
    int width, height;

    unsigned int m_tex;
    GLuint m_vao;
};

#endif /* textures_hpp */
