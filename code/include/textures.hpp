
#ifndef textures_hpp
#define textures_hpp

// using SDL and standard IO
#include <glad.h>
#include <SDL2/SDL.h>
#include <string>

class Camera;

class Texture
{
public:
    Texture();
    ~Texture();

    bool load_object(int w, int h, std::string path, SDL_Color* palette = NULL);

    void render(int x, int y, SDL_Rect* clip, Camera* cam, int dir = 0, int flip = 0);

    int get_width() { return width; }
    int get_height() { return height; }

private:
    SDL_Color m_palette;
    int width, height;
    int tex_width, tex_height;

    GLuint m_tex;
    GLuint m_vao;
};

#endif /* textures_hpp */
