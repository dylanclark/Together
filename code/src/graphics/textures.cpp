
// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <glad.h>
#include <OpenGL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header file
#include <textures.hpp>
#include <levels.hpp>
#include <engine.hpp>

void _CheckGLError(const char* file, int line);

#define CheckGLError() _CheckGLError(__FILE__, __LINE__)

void _CheckGLError(const char* file, int line)
{
    GLenum err ( glGetError() );

    while ( err != GL_NO_ERROR )
    {
        std::string error;
        switch ( err )
        {
            case GL_INVALID_OPERATION:  error="INVALID_OPERATION";      break;
            case GL_INVALID_ENUM:       error="INVALID_ENUM";           break;
            case GL_INVALID_VALUE:      error="INVALID_VALUE";          break;
            case GL_OUT_OF_MEMORY:      error="OUT_OF_MEMORY";          break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
        }
        std::cout << "GL_" << error.c_str() <<" - " << file << ":" << line << std::endl;
        err = glGetError();
    }

    return;
}

// initialize
Texture::Texture()
{
    width = 0;
    height = 0;
};

// deallocate
Texture::~Texture()
{
};

bool Texture::load_object(int w, int h, std::string path)
{
    std::cout << path << std::endl;
    // get object dimensions
    width = w;
    height = h;
    int texture_width, texture_height;

    // let's load the image
    SDL_Surface* surface = IMG_Load(("resources/textures/"+path).c_str());
    if (surface == NULL) {
        printf("Unable to load image %s! SDL error: %s\n",
               ("resources/textures/"+path).c_str(), SDL_GetError());
        return false;
    }
    texture_width = surface->w;
    texture_height = surface->h;

    // and turn it into an opengl texture
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    GLint mode = GL_RGB;
    if (surface->format->BytesPerPixel == 4) {
        mode = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, mode, texture_width, texture_height, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
    SDL_FreeSurface(surface);

    float tex_w = (float) width / (float) texture_width;
    float tex_h = (float) height / (float) texture_height;

    float vertices[] = {
        0.0f,          0.0f,            0.0f, 0.0f,
        (float) width, 0.0f,            tex_w, 0.0f,
        (float) width, (float) height,  tex_w, tex_h,
        0.0f,          (float) height,  0.0f, tex_h
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int vertex_buffer_obj;
    unsigned int element_buffer_obj;

    // first, we create the 2d object in model space
    m_vao = 0;
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    CheckGLError();
    glGenBuffers(1, &vertex_buffer_obj);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
    glGenBuffers(1, &element_buffer_obj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // we'll set some parameters for completeness
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return true;
};

void Texture::render(int x, int y, SDL_Rect* clip, SDL_Rect* camera)
{
    game->m_shader->use();
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
